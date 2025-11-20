#ifndef _GEOMETRY_
#define _GEOMETRY_

#include "../core/common.hpp"
#include "../core/ray.hpp"
#include <math.h>

namespace gc
{
float radians(float deg);
float degrees(float rad);

class Bounds3f
{
  public:
    Bounds3f()
    {
    }
    Bounds3f(Vector3<real_type> min, Vector3<real_type> max) : min(min), max(max)
    {
    }

    bool hit(const Ray &r, float tmin, float tmax);

    Vector3<real_type> min;
    Vector3<real_type> max;
};

Bounds3f b_union(const Bounds3f &a, const Bounds3f &b) 
{
    Vector3<real_type> min, max;
    int minx, miny, minz, maxx, maxy, maxz;

    minx = fmin(a.min.x, b.min.x);
    miny = fmin(a.min.y, b.min.y);
    minz = fmin(a.min.z, b.min.z);

    maxx = fmax(a.max.x, b.max.x);
    maxy = fmax(a.max.y, b.max.y);
    maxz = fmax(a.max.z, b.max.z);
    
    min = {minx, miny, minz};
    max = {maxx, maxy, maxz};

    return {min, max};

}

} // namespace gc

#endif