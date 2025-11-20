#include "lights.hpp"

namespace gc
{

std::string DirectionalLight::to_string() const
{
    std::ostringstream oss;
    oss << "DirectionalLight: { " << Light::to_string() // Chama a implementação da classe base
        << ", From: " << from << ", To: " << to << " }";
    return oss.str();
}

Spectrum DirectionalLight::sample_Li(const Surfel &hit /*in*/, Vector3<real_type> *light_dir /*out*/,
                                 VisibilityTester *vis /*out*/)
{
    *light_dir = get_direction().normalize();
    *vis = VisibilityTester(hit, *light_dir);
    return intensity;
}

} // namespace gc