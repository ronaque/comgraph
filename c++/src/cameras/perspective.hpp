#ifndef PERSP_H
#define PERSP_H

#include "../math/geometry.hpp"
#include "../msg_system/error.hpp"
#include "camera.hpp"
#include <math.h>

namespace gc
{

/// perspective camera implementation (parallel projection)
class PerspectiveCamera : public Camera
{
  public:
    // === Public interface

    PerspectiveCamera(float fovy, Film *film = nullptr, LookAt *look_at = nullptr) : Camera(film, look_at)
    {
        real_type aspect_ratio = real_type(film->width) / film->height;
        real_type fovy_rad = radians(fovy);
        real_type tan_fov = std::tan(fovy_rad / 2);
        TRACE(FORMAT("AR: ", aspect_ratio, ", Fovy: ", fovy, ", H: ", tan_fov));

        this->l = (-1 * aspect_ratio) * tan_fov;
        this->r = aspect_ratio * tan_fov;
        this->b = -tan_fov;
        this->t = tan_fov;

        initialize_camera_frame();
    }

    /// Default constructor (Legacy version)
    PerspectiveCamera(Film *film = nullptr, LookAt *look_at = nullptr) : Camera(film, look_at)
    {
    }
    // The other special methods are left as default.
    ~PerspectiveCamera() override = default;
    /// Disable copy Ctro.
    PerspectiveCamera(const PerspectiveCamera &) = delete;
    PerspectiveCamera &operator=(const PerspectiveCamera &) = delete;

    /// Return a ray based on the camera's set up.
    void generate_ray(real_type s, real_type t, Ray &r) const override;

    inline std::string to_string() const
    {
        std::stringstream ss;
        ss << "PerspectiveCamera { eye: " << e << ", Camera frame: (U: " << U << ", V: " << V << ", W: " << W << "), left: " << l
           << ", right: " << r << ", bottom: " << b << ", top: " << t << " }";
        return ss.str();
    }
};
} // namespace gc

/// Regular factory pattern function.
// PerspectiveCamera* create_perspective_camera(const ParamSet& ps,
//     const Transform& cam2world,
//     Film* film);

// }

#endif