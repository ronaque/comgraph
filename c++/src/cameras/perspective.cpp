#include "perspective.hpp"
#include "../msg_system/error.hpp"
#include <fstream>

namespace gc {

/// Generates a perspective ray.
void PerspectiveCamera::generate_ray(real_type row, real_type col, Ray& ray) const {
    auto [u, v] = film2screen_points(col, row);
    
    Vector3<real_type> direction = (W + (u*U) + (v*V)).normalize();
    Point3<real_type> origin = e;

    # if DEBUG_LEVEL >= 4
    // std::stringstream ss;
    // ss << "e: " << e << ", row: " << row << ", u: " << u << ", U: " << U << ", u*U: " << (u*U) << ", col: " << col << ", v: " << v << ", V: " << V << ", v*V: " << v*V << "\n";

    // std::ofstream pixel_out("debug.out", std::ios::app);
    // pixel_out << ss.str();
    // pixel_out.close();
    # endif

    ray = Ray(origin, direction);
}

// PerspectiveCamera* create_perspective_camera(const ParamSet& ps,
//                                                const Transform& cam2world,
//                                                Film* film) {
//   return new PerspectiveCamera(film);
// }
}  // namespace gc

//==============================[ perspective.cpp ]==============================//
