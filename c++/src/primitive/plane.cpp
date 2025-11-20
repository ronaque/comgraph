#include "../msg_system/error.hpp"
#include "shape.hpp"

namespace gc
{

std::string Plane::to_string() const
{
    std::ostringstream oss;
    oss << "Plane: {\n"
        << "  Point: " << point << ",\n"
        << "  Normal: " << normal << ",\n"
        << "}";
    return oss.str();
}

bool Plane::intersect(const Ray &r, Surfel *sf) const
{
    // real_type A = dot(r.direction, r.direction);
    // real_type B = dot(2 * (r.origin - center), r.direction);
    // real_type C = dot(r.origin - center, r.origin - center) - pow(radius, 2);

    // real_type discriminant = B * B - 4 * A * C;

    // if (discriminant < 0)
    // {
    //     return false;
    // }

    // real_type sqrt_discriminant = std::sqrt(discriminant);
    // real_type t1 = (-B - sqrt_discriminant) / (2 * A);
    // real_type t2 = (-B + sqrt_discriminant) / (2 * A);

    // real_type t = -1;

    // if (t1 > r.t_min && t1 < r.t_max)
    //     t = t1;
    // else if (t2 > r.t_min && t2 < r.t_max)
    //     t = t2;

    // if (t < 0)
    // {
    //     return false;
    // }

    // r.t_max = t;

    // Point3<real_type> hit_point = r(t);
    // Vector3<real_type> normal = (hit_point - center) / radius;

    // *sf = Surfel(hit_point, normal, -r.direction, {0.0, 0.0}, nullptr);

    // return true;
    return false;
}

bool Plane::intersect_p(const Ray &r) const
{
    // // TRACE("Calculating intersection point of a sphere");

    // real_type A = dot(r.direction, r.direction);
    // real_type B = dot(2 * (r.origin - center), r.direction);
    // real_type C = dot(r.origin - center, r.origin - center) - pow(radius, 2);

    // real_type f = pow(B, 2) - (4 * A * C);

    // // DEBUG(FORMAT("f: ", f));

    // if (f >= 0)
    //     return true;
    // else
    //     return false;

    return false;
}
} // namespace gc