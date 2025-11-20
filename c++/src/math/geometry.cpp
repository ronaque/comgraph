#include "geometry.hpp"

namespace gc
{
float radians(float deg)
{
    constexpr float one_eigthy{180};
    return ((float)M_PI / one_eigthy) * deg;
}

float degrees(float rad)
{
    constexpr float one_eigthy{180};
    return (one_eigthy / (float)M_PI) * rad;
}

bool Bounds3f::hit(const Ray &r, float tmin, float tmax)
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

    if (tmax <= tmin)
        return false;

    return true;
};
} // namespace gc