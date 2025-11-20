#include "lights.hpp"

namespace gc
{

std::string AmbientLight::to_string() const
{
    std::ostringstream oss;
    oss << "AmbientLight: { " << Light::to_string() // Chama a implementação da classe base
        << " }";
    return oss.str();
}

Spectrum AmbientLight::sample_Li(const Surfel &hit /*in*/, Vector3<real_type> *wi /*out*/, VisibilityTester *vis /* out*/)
{
    return {0.0f, 0.0f, 0.0f};
}
} // namespace gc