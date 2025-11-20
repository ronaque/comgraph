#include "../msg_system/error.hpp"
#include "shape.hpp"

namespace gc
{

std::string Box::to_string() const
{
    std::ostringstream oss;
    oss << "Sphere: {\n"
        << "  Min: " << min << ",\n"
        << "  Max: " << max << ",\n"
        << "}";
    return oss.str();
}

bool Box::intersect(const Ray &r, Surfel *sf) const
{
    float tmin = (min.x - r.origin.x) / r.direction.x;
    float tmax = (max.x - r.origin.x) / r.direction.x;

    if (tmin > tmax)
        std::swap(tmin, tmax);

    float tymin = (min.y - r.origin.y) / r.direction.y;
    float tymax = (max.y - r.origin.y) / r.direction.y;

    if (tymin > tymax)
        std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    float tzmin = (min.z - r.origin.z) / r.direction.z;
    float tzmax = (max.z - r.origin.z) / r.direction.z;

    if (tzmin > tzmax)
        std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;

    r.t_min = tmin;
    r.t_max = tmax;

    Point3<real_type> hit_point = r(tmin);

    Vector3<real_type> normal;
    const float epsilon = 1e-4f;

    if (std::abs(hit_point.x - min.x) < epsilon)
        normal = Vector3<real_type>{-1.0, 0.0, 0.0}; // Face esquerda
    else if (std::abs(hit_point.x - max.x) < epsilon)
        normal = Vector3<real_type>{1, 0, 0}; // Face direita
    else if (std::abs(hit_point.y - min.y) < epsilon)
        normal = Vector3<real_type>{0, -1, 0}; // Face de baixo
    else if (std::abs(hit_point.y - max.y) < epsilon)
        normal = Vector3<real_type>{0, 1, 0}; // Face de cima
    else if (std::abs(hit_point.z - min.z) < epsilon)
        normal = Vector3<real_type>{0, 0, -1}; // Face de trás
    else if (std::abs(hit_point.z - max.z) < epsilon)
        normal = Vector3<real_type>{0, 0, 1}; // Face da frente

    *sf = Surfel(hit_point, normal, -r.direction, {0.0, 0.0}, nullptr);

    return true;
}

bool Box::intersect_p(const Ray &r) const
{
    // TRACE("Calculating intersection point of a sphere");

    // real_type A = dot(r.directionection, r.directionection);
    // real_type B = dot(2 * (r.originin - center), r.directionection);
    // real_type C = dot(r.originin - center, r.originin - center) - pow(radius, 2);

    // real_type f = pow(B, 2) - (4 * A * C);

    // // DEBUG(FORMAT("f: ", f));

    // if (f >= 0)
    //     return true;
    // else
    return false;
}

Bounds3f Box::get_bounds() const
{
    return {min, max};
}
} // namespace gc