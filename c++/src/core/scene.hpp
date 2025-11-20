#pragma once
#include "../core/background.hpp"
#include "../primitive/primitive.hpp"
#include "ray.hpp"

namespace gc
{
class Scene
{
  public:
    Scene(std::shared_ptr<AggregatePrimitive> primitives, std::shared_ptr<Camera> camera, std::shared_ptr<Background> background,
          std::vector<std::shared_ptr<Light>> &lights)
        : primitives(std::move(primitives)), background{background}, camera{camera}, lights{std::move(lights)}
    {
    }
    Scene() = default;

    std::shared_ptr<AggregatePrimitive> primitives;
    std::shared_ptr<Background> background;
    std::shared_ptr<Camera> camera;
    std::vector<std::shared_ptr<Light>> lights;

    float scene_max_extent = 0.0f;
    int max_depth = 1;
};
} // namespace gc