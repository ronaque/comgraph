#include "lights.hpp"

namespace gc
{

std::string PointLight::to_string() const
{
    std::ostringstream oss;
    oss << "PointLight: { " << Light::to_string() // Chama a implementação da classe base
        << ", Position: " << from << " }";
    return oss.str();
}

Spectrum PointLight::sample_Li(const Surfel &hit /*in*/, Vector3<real_type> *light_dir /*out*/, VisibilityTester *vis)
{
    Surfel light_surfel_pos;
    *light_dir = (from - hit.hit_point).normalize();
    light_surfel_pos.hit_point = from;
    *vis = VisibilityTester(hit, light_surfel_pos);
    return intensity;
}
} // namespace gc