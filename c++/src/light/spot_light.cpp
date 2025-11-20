#include "../math/geometry.hpp"
#include "lights.hpp"

namespace gc
{

std::string SpotLight::to_string() const
{
    std::ostringstream oss;
    oss << "SpotLight: { " << Light::to_string() << ", From: " << from << ", To: " << to << ", Cutoff: " << cutoff
        << ", falloff: " << falloff << " }";
    return oss.str();
}

Spectrum SpotLight::sample_Li(const Surfel &hit /*in*/, Vector3<real_type> *light_dir /*out*/,
                              VisibilityTester *vis /*out*/)
{
    Vector3<real_type> light_direction = get_direction().normalize();
    *light_dir = light_direction;
    *vis = VisibilityTester(hit, Surfel(from));

    // Get intensity

    Vector3<real_type> spot_axis = to - from;
    Vector3<real_type> hit_direction = hit.hit_point - from;

    real_type theta_hit = dot(spot_axis.normalize(), hit_direction.normalize());
    real_type theta_cut = std::cos(radians(cutoff));
    real_type theta_fall = std::cos(radians(falloff));

    if (theta_hit < theta_cut)
        return {0.0f, 0.0f, 0.0f};
    else if (theta_hit < theta_fall)
    {
        real_type atenuation = (theta_hit - theta_cut) / (theta_fall - theta_cut);
        return atenuation * intensity;
    }
    else
    {
        return intensity;
    }
}

} // namespace gc