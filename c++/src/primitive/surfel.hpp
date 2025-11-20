#pragma once
#include "../core/common.hpp"
#include "primitive.hpp"

namespace gc
{
class Surfel
{
  public:
    Surfel() {};
    Surfel(const Point3<real_type> &hp) : hit_point{hp} {};
    Surfel(const Point3<real_type> &p, const Vector3<real_type> &n, const Vector3<real_type> &wo,
           const Point2<real_type> &uv, const Primitive *pri)
        : hit_point{p}, normal{n}, out_dir{wo}, uv{uv}, primitive{pri} { /* empty */ };
    Surfel(const Point3<real_type> &p, const Vector3<real_type> &n, const Vector3<real_type> &wo,
           const Point2<real_type> &uv, const Primitive *pri, float time)
        : hit_point(p), normal(n), out_dir(wo), uv(uv), primitive(pri), time(time) {};

    Point3<real_type> hit_point; //!< Contact point.
    Vector3<real_type> normal;   //!< The surface normal.
    Vector3<real_type> out_dir;  //!< Outgoing direction of light, which is -ray.
    Point2<real_type> uv;        //!< Parametric coordinate (u,v) of the hit surface.
    const Primitive *primitive = nullptr;
    //!< Pointer to the primitive.

    std::string to_string() const;

    float time;
};
} // namespace gc
