#ifndef ORTHO_H
#define ORTHO_H

#include "camera.hpp"

namespace gc
{

/// Orthographic camera implementation (parallel projection)
class OrthographicCamera : public Camera
{
  public:
    // === Public interface

    OrthographicCamera(real_type l, real_type r, real_type b, real_type t, Film *film = nullptr,
                       LookAt *look_at = nullptr)
        : Camera{l, r, b, t, film, look_at}
    {
        initialize_camera_frame();
    };

    /// Default constructor (Legacy version)
    OrthographicCamera(Film *film = nullptr, LookAt *look_at = nullptr) : Camera(film, look_at)
    {
    }
    // The other special methods are left as default.
    ~OrthographicCamera() override = default;
    /// Disable copy Ctro.
    OrthographicCamera(const OrthographicCamera &) = delete;
    OrthographicCamera &operator=(const OrthographicCamera &) = delete;

    /// Return a ray based on the camera's set up.
    void generate_ray(real_type s, real_type t, Ray &r) const override;

    inline std::string to_string() const
    {
        std::stringstream ss;
        ss << "OrthographicCamera { e: " << e << ", U: " << U << ", V: " << V << ", W: " << W << ", left: " << l
           << ", right: " << r << ", bottom: " << b << ", top: " << t << " }";
        return ss.str();
    }
};

//=== class related functions.

/// Regular factory pattern function.
// OrthographicCamera* create_orthographic_camera(const ParamSet& ps,
//                                                const Transform& cam2world,
//                                                Film* film);
} // namespace gc
#endif
//-------------------------------------------------------------------------------
