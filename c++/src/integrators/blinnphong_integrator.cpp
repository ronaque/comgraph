#include "../cameras/camera.hpp"
#include "../core/app.hpp"
#include "../core/paramset.hpp"
#include "../core/scene.hpp"
#include "../light/lights.hpp"
#include "../light/visibility_tester.hpp"
#include "../math/geometry.hpp"
#include "../msg_system/error.hpp"
#include "../primitive/surfel.hpp"
#include "integrators.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>

namespace gc
{
std::optional<Spectrum> BlinnPhongIntegrator::Li(const Ray &ray, const Scene &scene, int depth = 0) const
{
    Surfel surfel;
    Spectrum color;
    bool hit = false;

    /**
     * First, we check if the camera ray intersects with a primitive,
     * and, if so, get the surfel of the closer primitive hitted
     */
    for (const std::shared_ptr<Primitive> &p : scene.primitives->primitive_list)
    {
        if (p->intersect(ray, &surfel))
        {
            surfel.primitive = p.get();
            hit = true;
        }
    }

    /** If the ray didn't hit anything, return empty informing the renderer to render the background */
    if (!hit)
        return {};

    Vector3<real_type> view_direction = -ray.direction.normalize(); // Direction TO the camera
    Vector3<real_type> normal = surfel.normal.normalize();

    /** If the ray hit the back of the primitive, return */
    // if (dot(normal, surfel.out_dir) < 0)
    // {
    //     return {};
    // }

    // std::cout << "surfel intersected: " << surfel.to_string();

    Spectrum light_reflection{0.0f, 0.0f, 0.0f}; // This variable, effectively, will be the color of the pixel
    std::shared_ptr<Light> ambient_light_super = nullptr;
    std::shared_ptr<BlinnPhongMaterial> blinn_material =
        std::dynamic_pointer_cast<BlinnPhongMaterial>(surfel.primitive->material);

    if (!blinn_material)
        ERROR("Encoutered a material that isn't blinn for a blinn integrator");

    int i = 1;

    /** For each light, we will get it's contribution to the pixel hitted by the ray */
    for (std::shared_ptr<Light> light : scene.lights)
    {
        /** Ambient light contribution will be added at the end of the L calculation */
        if (light->flag == light_flag_e::ambient)
        {
            ambient_light_super = light;
            continue;
        }

        VisibilityTester visibility_tester;
        Vector3<real_type> light_direction;
        Surfel light_surfel_pos;

        /** Determines the light_direction and fills the visibility_tester parameters */
        Spectrum intesity = light->sample_Li(surfel, &light_direction, &visibility_tester);
        if (intesity == Spectrum{0.0f, 0.0f, 0.0f})
            continue;
            
        if (!visibility_tester.unoccluded(scene)) // !unoccluded -> occluded
        {
            // if (light->flag == light_flag_e::point)
            //     light_reflection = {0.8f, 0.0f, 0.0f};
            // else if (light->flag == light_flag_e::directional)
            //     light_reflection = {0.0f, 0.8f, 0.0f};
            // else {
            //     real_type blue = i*0.2f;
            //     light_reflection = {0.0f, 0.0f, blue};
            // }
            i += 1;
            continue;
        }

        Spectrum inner_light_reflection{0.0f, 0.0f, 0.0f};

        Vector3<real_type> halfway =
            (view_direction + light_direction) / (view_direction + light_direction).magnitude();

        /**
         * This is the calculation of the impact of the actual light into the material
         * Based on the light intensity, it will apply the material color based on the diffuse variable
         * and the light focus (glossiness) in the material
         */
        Spectrum diffuse_contribution = (std::max(0.0f, dot(normal, light_direction)) * intesity *
                                         blinn_material->kdiffuse); // I * Kd * max(0, n . l)

        bool has_specular = blinn_material->glossiness != 0.0f;
        Spectrum specular_contribution = has_specular
                                             ? (pow(std::max(0.0f, dot(normal, halfway)), blinn_material->glossiness) *
                                                intesity * blinn_material->kspecular) // I * ks * max(0, n . h)^g
                                             : Spectrum{0.0f, 0.0f, 0.0f};

        inner_light_reflection = diffuse_contribution + specular_contribution;

        /** The pixel result will be based on the sum of all lights impacts */
        light_reflection = light_reflection + inner_light_reflection;
    }

    /** Adds the ambient light contribution */
    if (ambient_light_super)
    {
        std::shared_ptr<AmbientLight> ambient_light = std::dynamic_pointer_cast<AmbientLight>(ambient_light_super);

        light_reflection = ambient_light->intensity * blinn_material->kambient + light_reflection;
    }

    /**
     * This will take care about the mirror effect in materials
     * It takes the perfect mirror direction, creates the reflected ray, and recursively calls LI with increased depth
     */
    if (depth < scene.max_depth)
    {
        Vector3<real_type> reflected_direction =
            ray.direction - 2 * (dot(ray.direction.normalize(), surfel.normal) * surfel.normal);

        float ray_epsilon = shadow_epsilon;
        if (dot(surfel.normal, reflected_direction.normalize()) < 0)
            ray_epsilon *= -1;

        Point3<real_type> origin = surfel.hit_point + surfel.normal * ray_epsilon;

        Ray reflected_ray = Ray(origin, reflected_direction.normalize());
        std::optional<Spectrum> temp_L = Li(reflected_ray, scene, depth + 1);
        Spectrum l_recursiveness = temp_L.has_value() ? temp_L.value() : Spectrum{0.0f, 0.0f, 0.0f};
        light_reflection = light_reflection + blinn_material->kmirror * l_recursiveness;
    }

    Spectrum return_color = light_reflection;

    if (depth == 0)
        return_color = Spectrum{clamp(light_reflection.x, 0.0f, 1.0f), clamp(light_reflection.y, 0.0f, 1.0f),
                                clamp(light_reflection.z, 0.0f, 1.0f)};

    return return_color;
}

void BlinnPhongIntegrator::preprocess(const Scene &scene)
{
}

std::shared_ptr<std::vector<byte>> BlinnPhongIntegrator::render(const Scene &scene)
{
    // Renderização
    std::vector<byte> pixels;
    size_t width = scene.camera->film.width;
    size_t height = scene.camera->film.height;
    auto [x_0, x_1, y_0, y_1] = App::get_cropped_values(width, height);

    pixels.reserve(width * height * 3);

    DEBUG(FORMAT("Starting rendering of pixels. \nInitial Width: ", x_0, ", final with: ", x_1,
                 ", initial height: ", y_0, ", final height: ", y_1));

    // long long total_pixels = (long long)(x_1 - x_0) * (y_1 - y_0);
    // long long pixels_done = 0;

    for (int row = y_1 - 1; row >= y_0; row--) // From bottom to top
    {
        for (int col = x_0; col < x_1; col++) // From left
        {

            // pixels_done++;
            // if (total_pixels > 0)
            // {
            //     int percentage = static_cast<int>((pixels_done * 100) / total_pixels);
            //     if (percentage > 100)
            //         percentage = 100;
            //     progress_bar(percentage);
            // }

            Ray ray;
            scene.camera->generate_ray(row, col, ray);

            auto temp_L = Li(ray, scene);
            Spectrum color =
                temp_L.has_value() ? temp_L.value() : scene.background->sampleXYZ(float(col) / x_1, float(row) / y_1);

            if (color.x > 1.0 || color.y > 1.0 || color.z > 1.0)
            {
                DEBUG(FORMAT("Spectrum passed it's limits: ", color.to_string(),
                             " Color is from Li?: ", temp_L.has_value()));
            }
            assert(color.x <= 1.0);
            assert(color.y <= 1.0);
            assert(color.z <= 1.0);

            pixels.push_back(static_cast<byte>(color.x * 255));
            pixels.push_back(static_cast<byte>(color.y * 255));
            pixels.push_back(static_cast<byte>(color.z * 255));
        }
    }

    return std::make_shared<std::vector<byte>>(pixels);
}

} // namespace gc
