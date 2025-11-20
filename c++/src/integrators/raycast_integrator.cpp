#include "../cameras/camera.hpp"
#include "../core/paramset.hpp"
#include "../core/scene.hpp"
#include "../math/geometry.hpp"
#include "../msg_system/error.hpp"
#include "../primitive/surfel.hpp"
#include "integrators.hpp"

namespace gc
{

// RayCastIntegrator Method Definitions
std::optional<Spectrum> RayCastIntegrator::Li(const Ray &ray, const Scene &scene, int depth) const
{
    Surfel surfel;
    Spectrum color;
    bool hit = false;

    for (const std::shared_ptr<Primitive> &p : scene.primitives->primitive_list)
    {
        if (p->intersect(ray, &surfel)) // Does the ray hit any sphere in the scene?
        {
            surfel.primitive = p.get();
            hit = true;
        }
    }

    if (hit)
    {
        color = surfel.primitive->material->get_color();
        TRACE(FORMAT("Hited a primitive with color: ", color.to_string()));
        return color;
    }
    else
        return {};
}

void RayCastIntegrator::preprocess(const Scene &scene)
{
}

std::shared_ptr<std::vector<byte>> RayCastIntegrator::render(const Scene &scene)
{
    // Renderização
    std::vector<byte> pixels;
    size_t width = scene.camera->film.width;
    size_t height = scene.camera->film.height;

    DEBUG(FORMAT("Starting rendering of pixels. width: ", width));

    for (int row = height - 1; row >= 0; row--)
    {
        for (size_t col = 0; col < width; col++)
        {
            Ray ray;
            scene.camera->generate_ray(row, col, ray);

            auto temp_L = Li(ray, scene, 0);
            Spectrum color = temp_L.has_value() ? temp_L.value()
                                                : scene.background->sampleXYZ(float(col) / width, float(row) / height);

            // Converte para byte e armazena
            pixels.push_back(static_cast<byte>(color.x * 255));
            pixels.push_back(static_cast<byte>(color.y * 255));
            pixels.push_back(static_cast<byte>(color.z * 255));
        }
    }

    return std::make_shared<std::vector<byte>>(pixels);
}

} // namespace gc
