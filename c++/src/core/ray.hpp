#pragma once
#include "common.hpp"
#include <cstdint>
#include <vector>

#define ll long long int

namespace gc
{

class Ray
{

  public:
    Ray(const Point3<real_type> &o, const Vector3<real_type> &d, real_type start = 0, real_type end = INFINITY)
        : origin{o}, direction{d}, t_min{start}, t_max{end}
    { /*empty*/
    }
    Ray() : t_min{0.f}, t_max{INFINITY}
    { /*empty*/
    }

    Point3<real_type> origin;       //!< origin
    Vector3<real_type> direction;   //!< direction
    mutable real_type t_min, t_max; //!< parameters

    // ... the methods goes here

    // Vector3 normalize();

    Point3<real_type> operator()(real_type t) const
    {
        return origin + (t * direction);
    }

    std::string to_string()
    {
        std::stringstream ss;
        ss << "Ray: [o=" << origin << ", direction=" << direction << ", tmin: " << t_min
           << ", tmax: " << t_max << "]\n";
        return ss.str();
    }
};

inline std::ostream &operator<<(std::ostream &os, const Ray &ray)
{
    os << "Ray: [o=" << ray.origin << ", direction=" << ray.direction << ", tmin: " << ray.t_min
       << ", tmax: " << ray.t_max << "]\n";
    return os;
}

// Ray r{ Point3{0,0,0}, Vector3{3,2,-4} }; // creating an infinity ray.
// float t{2.3};                            // a parameter for the ray.
// Point3 p = r( t );                       // overloading operator()(), so that it returns o + t*d.

} // namespace gc