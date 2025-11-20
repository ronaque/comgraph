#include "../msg_system/error.hpp"
#include "shape.hpp"

namespace gc
{
std::string Sphere::to_string() const
{
    std::ostringstream oss;
    oss << "Sphere: {\n"
        << "  Center: " << center << ",\n"
        << "  Radius: " << radius << ",\n"
        << "}";
    return oss.str();
}

bool Sphere::intersect(const Ray &r, Surfel *sf) const
{
    real_type A = dot(r.direction, r.direction);
    real_type B = dot(2 * (r.origin - center), r.direction);
    real_type C = dot(r.origin - center, r.origin - center) - pow(radius, 2);

    real_type discriminant = B * B - 4 * A * C;

    if (discriminant < 0)
    {
        return false;
    }

    real_type sqrt_discriminant = std::sqrt(discriminant);
    real_type t1 = (-B - sqrt_discriminant) / (2 * A);
    real_type t2 = (-B + sqrt_discriminant) / (2 * A);

    real_type t = -1;

    if (t1 > r.t_min && t1 < r.t_max)
        t = t1;
    else if (t2 > r.t_min && t2 < r.t_max)
        t = t2;

    if (t < 0)
    {
        return false;
    }

    r.t_max = t;
    Point3<real_type> hit_point = r(t);
    Vector3<real_type> normal = (hit_point - center) / radius;

    *sf = Surfel(hit_point, normal, -r.direction, {0.0, 0.0}, nullptr);

    return true;
}

bool Sphere::intersect_p(const Ray &r) const
{
    real_type A = dot(r.direction, r.direction);
    real_type B = dot(2 * (r.origin - center), r.direction);
    real_type C = dot(r.origin - center, r.origin - center) - pow(radius, 2);

    real_type discriminant = B * B - 4 * A * C;

    if (discriminant < 0)
    {
        return false;
    }

    real_type sqrt_discriminant = std::sqrt(discriminant);
    real_type t1 = (-B - sqrt_discriminant) / (2 * A);
    real_type t2 = (-B + sqrt_discriminant) / (2 * A);

    real_type t = -1;

    if (t1 > r.t_min && t1 < r.t_max)
        return true;
    else if (t2 > r.t_min && t2 < r.t_max)
        return true;

    return false;
}

Bounds3f Sphere::get_bounds() const
{
    Vector3<real_type> min, max, radius_vec;
    radius_vec = {radius, radius, radius};
    min = {center - radius_vec};
    max = {center + radius_vec};

    return {min, max};
}
} // namespace gc