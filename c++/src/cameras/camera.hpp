#ifndef _CAMERA_
#define _CAMERA_

#include "../core/film.hpp"
#include "look_at.hpp"
#include "../core/ray.hpp"

namespace gc
{

class Camera
{
  public:
    real_type l, r, b, t;
    Vector3<real_type> W, U, V; // Camera frame (u, v, w) 
    Point3<real_type> e; // Eye
    Film film;
    LookAt look_at;
    virtual void generate_ray(real_type s, real_type t, Ray &r) const = 0;

    Camera(Film *film, LookAt *look_at) : film{*film}, look_at{*look_at}
    {
    }

   virtual ~Camera() = default; 
    Camera( real_type l, real_type r, real_type b, real_type t, Film *film, LookAt *look_at)
        : l{l}, r{r}, b{b}, t{t}, film{*film}, look_at{*look_at}
    {
    }

    std::pair<real_type, real_type> film2screen_points(real_type col, real_type row) const {
      real_type u, v;
      u = l + (r-l)*(col+0.5f)/film.width;
      v = b + (t-b)*(row+0.5f)/film.height;
      return {u, v};
    }

    void initialize_camera_frame() {
      Vector3<real_type> gaze = look_at.look_at - look_at.look_from;
      this->W = gaze.normalize();
      this->U = cross(look_at.up, W).normalize();
      this->V = cross(W, U).normalize();
      this->e = look_at.look_from;
    }

    
};
} // namespace gc

#endif