#pragma once

#include "paramset.hpp"
#include "../light/lights.hpp"
#include "../primitive/primitive.hpp"
#include "../msg_system/error.hpp"

namespace gc
{
class Builder
{
  public:
    static std::shared_ptr<AmbientLight> build_ambient_light(const ParamSet &params);
    static std::shared_ptr<DirectionalLight> build_directional_light(const ParamSet &params);
    static std::shared_ptr<PointLight> build_point_light(const ParamSet &params);
    static std::shared_ptr<SpotLight> build_spot_light(const ParamSet &params);

    static std::shared_ptr<Shape> build_sphere(const ParamSet &params);
    static std::shared_ptr<Shape> build_box(const ParamSet &params);
    static std::shared_ptr<Shape> build_plane(const ParamSet &params);
    static std::vector<std::shared_ptr<Shape>> build_triangle_mesh(bool flip_normals, const ParamSet &params);

    static std::shared_ptr<Material> build_flat_material(const ParamSet &params, bool isNamed);
    static std::shared_ptr<Material> build_blinn_material(const ParamSet &params, bool isNamed);
};
} // namespace gc