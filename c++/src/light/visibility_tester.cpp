#include "visibility_tester.hpp"
#include "../primitive/primitive.hpp" // Para gc::Primitive (se não vier via scene.hpp)

#include <algorithm> // Para std::max (se usar em algum lugar, bom ter)
#include <limits>    // Para std::numeric_limits<float>::max()

namespace gc
{

// Point
gc::VisibilityTester::VisibilityTester(const Surfel &p0_surfel, const Surfel &p1_surfel) : p0(p0_surfel), p1(p1_surfel)
{

    direction = Vector3<real_type>(0.0f, 0.0f, 0.0f);
}

// Direction
gc::VisibilityTester::VisibilityTester(const Surfel &p0_surfel, const Vector3<real_type> &direction_vector)
    : p0(p0_surfel), direction(direction_vector)
{

    p1 = Surfel();
}

bool VisibilityTester::unoccluded(const Scene &scene)
{
    float ray_epsilon = shadow_epsilon; 
    Ray shadowRay;

    // Directional light
    if (!(direction == Vector3<real_type>{0.0f, 0.0f, 0.0f}))
    {
        if (dot(p0.normal, direction) < 0)
            ray_epsilon *= -1;

        Point3<real_type> origin = p0.hit_point + p0.normal * ray_epsilon;

        shadowRay = Ray(origin, direction.normalize());
    }
    else
    {
        // Point light
        Vector3<real_type> dir_to_light = p1.hit_point - p0.hit_point;
        if (dot(p0.normal, dir_to_light) < 0)
            ray_epsilon *= -1;

        Point3<real_type> origin = p0.hit_point + p0.normal * ray_epsilon;
        Vector3<real_type> target = p1.hit_point - origin;

        shadowRay = Ray(origin, target, 0 + ray_epsilon, 1 - ray_epsilon);
    }

    if (scene.primitives)
    {
        if (scene.primitives->intersect_p(shadowRay))
        {
            return false; // Unoccluded -> !occluded 
        }
    }

    return true; // Unoccluded
}

} // namespace gc