#pragma once

#include "../core/common.hpp"
#include "visibility_tester.hpp"

namespace gc
{

enum class light_flag_e : std::uint8_t
{
    point = 1,
    directional = 2,
    area = 4,
    ambient = 8,
    spot = 16
};

inline std::string light_flag_to_string(light_flag_e flag)
{
    switch (flag)
    {
    case light_flag_e::point:
        return "Point";
    case light_flag_e::directional:
        return "Directional";
    case light_flag_e::area:
        return "Area";
    case light_flag_e::ambient:
        return "Ambient";
    case light_flag_e::spot:
        return "Spot";
    default:
        return "Unknown_(" + std::to_string(static_cast<std::uint8_t>(flag)) + ")";
    }
}

class Light
{
  public:
    virtual ~Light() {};
    Light(light_flag_e flags) : flag{flags} {};
    Light(light_flag_e flags, Spectrum intensity, Spectrum scale) : flag{flags}, intensity{intensity}, scale{scale} {};

    /// Retorna a intensidade da luz, direção e o teste oclusão.
    virtual Spectrum sample_Li(const Surfel &hit /*in*/, Spectrum *light_dir /*out*/ , VisibilityTester *vis /*out*/) = 0;
    virtual void preprocess(const Scene &) {};
    virtual Vector3<real_type> get_direction() = 0;
    virtual std::string to_string() const
    {
        std::ostringstream oss;
        oss << "Flag: " << light_flag_to_string(flag) << ", Intensity: " << intensity << ", Scale: " << scale;
        return oss.str();
    };

    light_flag_e flag;
    Spectrum intensity;
    Spectrum scale;
};

class AmbientLight : public Light
{
  public:
    AmbientLight(light_flag_e flags, Spectrum intensity, Spectrum scale) : Light(flags, intensity, scale) {};

    Spectrum sample_Li(const Surfel &hit /*in*/, Vector3<real_type> *light_dir /*out*/ , VisibilityTester *vis /*out*/) override;
    void preprocess(const Scene &) {};
    std::string to_string() const override;

    Vector3<real_type> get_direction()
    {
        return Vector3<real_type>{0, 0, 0};
    };
};

class DirectionalLight : public Light
{
  public:
    DirectionalLight(light_flag_e flags, Spectrum intensity, Spectrum scale) : Light(flags, intensity, scale) {};

    Spectrum sample_Li(const Surfel &hit /*in*/, Vector3<real_type> *light_dir /*out*/ , VisibilityTester *vis /*out*/) override;
    void preprocess(const Scene &) {};
    std::string to_string() const override;

    Vector3<real_type> get_direction()
    {
        return from - to;
    };

    void set_parameters(Point3<real_type> from, Point3<real_type> to)
    {
        this->from = from;
        this->to = to;
    };

  private:
    Point3<real_type> from;
    Point3<real_type> to;
};

class PointLight : public Light
{
  public:
    PointLight(light_flag_e flags, Spectrum intensity, Spectrum scale) : Light(flags, intensity, scale) {};

    Spectrum sample_Li(const Surfel &hit /*in*/, Vector3<real_type> *light_dir /*out*/ , VisibilityTester *vis /*out*/) override;
    void preprocess(const Scene &) {};
    std::string to_string() const override;

    Vector3<real_type> get_direction()
    {
        return Vector3<real_type>{0, 0, 0};
    };

    void set_parameters(Point3<real_type> from)
    {
        this->from = from;
    };

    Point3<real_type> from;
};

class SpotLight : public Light
{
  public:
    SpotLight(light_flag_e flags, Spectrum intensity, Spectrum scale) : Light(flags, intensity, scale) {};

    Spectrum sample_Li(const Surfel &hit /*in*/, Vector3<real_type> *light_dir /*out*/ , VisibilityTester *vis /*out*/);
    void preprocess(const Scene &) {};
    std::string to_string() const override;

    Vector3<real_type> get_direction()
    {
        return from - to;
    };

    void set_parameters(Point3<real_type> from, Point3<real_type> to, real_type cutoff, real_type falloff)
    {
        this->from = from;
        this->to = to;
        this->cutoff = cutoff;
        this->falloff = falloff;
    };

    Point3<real_type> from;
    Point3<real_type> to;
    real_type cutoff;
    real_type falloff;
};

} // namespace gc