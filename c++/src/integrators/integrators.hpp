#pragma once

#include <memory>
#include <utility>
#include <optional>


#include "../core/common.hpp"

namespace gc
{

class SamplerIntegrator
{
  public:
    virtual ~SamplerIntegrator() = default;
    SamplerIntegrator() {};

    virtual std::optional<Spectrum> Li(const Ray &ray, const Scene &scene, int depth) const = 0;
    virtual std::shared_ptr<std::vector<byte>> render(const Scene &scene) = 0;
    virtual void preprocess(const Scene &scene) = 0;

};

class RayCastIntegrator : public SamplerIntegrator
{
  public:
    // RayCastIntegrator Public Methods
    RayCastIntegrator() {};
    ~RayCastIntegrator() override = default;

    std::shared_ptr<std::vector<byte>> render(const Scene &scene);
    void preprocess(const Scene &scene);

    [[nodiscard]] std::optional<Spectrum> Li(const Ray &ray, const Scene &scene, int depth) const override;
};

class BlinnPhongIntegrator : public SamplerIntegrator
{
  public:
    // RayCastIntegrator Public Methods
    BlinnPhongIntegrator() {};
    ~BlinnPhongIntegrator() override = default;

    std::shared_ptr<std::vector<byte>> render(const Scene &scene);
    void preprocess(const Scene &scene);

    [[nodiscard]] std::optional<Spectrum> Li(const Ray &ray, const Scene &scene, int depth) const override;
};

} // namespace gc
