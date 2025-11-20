#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "../core/common.hpp"
#include <memory>
#include <string>

namespace gc
{

/**
 * @class Material
 * @brief Base class for all material types in the ray tracer.
 *
 * The basic version supports flat shading by providing a simple color.
 */
class Material
{
  public:
    std::string name;

    /**
     * @brief Construct a new Material with a specific color
     * @param color The base color of the material (RGB)
     */
    explicit Material(const Spectrum &color) : color_(color)
    {
    }

    /**
     * @brief Default constructor creates a white material
     */
    Material() : color_(Spectrum{1.0f, 1.0f, 1.0f})
    {
    }

    Material(const std::string name) : name{name}, color_(Spectrum{1.0f, 1.0f, 1.0f})
    {
    }

    Material(const std::string name, const Spectrum &color) : name{name}, color_(color)
    {
    }

    virtual ~Material() = default;

    /**
     * @brief Get the base color of the material
     * @return Spectrum The material's color
     */
    [[nodiscard]] virtual Spectrum get_color() const
    {
        return color_;
    }

    /**
     * @brief Set the base color of the material
     * @param color The new color to set
     */
    virtual void set_color(const Spectrum &color)
    {
        color_ = color;
    }

    /**
     * @brief Factory method to create materials from parameter sets
     * @param ps Parameter set containing material properties
     * @return std::shared_ptr<Material> Created material
     */
    static std::shared_ptr<Material> create(const ParamSet &ps);

    virtual std::string to_string() const
    {
        std::stringstream ss;
        ss << "Material(";
        ss << "color=" << color_;
        ss << ")";
        return ss.str();
    }

  protected:
    Spectrum color_; // Base color for flat shading
};

class FlatMaterial : public Material
{
  public:
    FlatMaterial() {};
    FlatMaterial(std::string name) : Material(name) {};

    FlatMaterial(Spectrum color) : Material(color) {};

    FlatMaterial(std::string name, const Spectrum &color) : Material(name, color) {};

    void set_color(const Spectrum &color)
    {
        color_ = color;
    };

    Spectrum get_color() const
    {
        return color_;
    }

    std::string to_string() const override
    {
        std::stringstream ss;
        ss << "FlatMaterial(";
        ss << "name=" << name << ", ";
        ss << "color=" << color_;
        ss << ")";
        return ss.str();
    }
};

class BlinnPhongMaterial : public Material
{
  public:
    BlinnPhongMaterial() {};
    BlinnPhongMaterial(std::string name) : Material(name) {};

    BlinnPhongMaterial(Spectrum color) : Material(color) {};

    BlinnPhongMaterial(std::string name, const Spectrum &color) : Material(name, color) {};
    void set_color(const Spectrum &color)
    {
        color_ = color;
    };

    Spectrum get_color() const
    {
        return color_;
    }

    void set_parameters(const Spectrum kd, const Spectrum ks, const Spectrum ka, const Spectrum km, const real_type gloss)
    {
        kdiffuse = kd;
        kspecular = ks;
        kambient = ka;
        kmirror = km;
        glossiness = gloss;
    };

    std::string to_string() const override
    {
        std::stringstream ss;
        ss << "BlinnPhongMaterial(";
        ss << "name=" << name << ", ";
        ss << "color=" << color_ << ", ";
        ss << "kdiffuse=" << kdiffuse<< ", ";
        ss << "kspecular=" << kspecular<< ", ";
        ss << "kambient=" << kambient<< ", ";
        ss << "kmirror=" << kmirror<< ", ";
        ss << "glossiness=" << glossiness;
        ss << ")";
        return ss.str();
    }

    Spectrum kdiffuse;
    Spectrum kspecular;
    Spectrum kambient;
    Spectrum kmirror;
    real_type glossiness;
};

} // namespace gc

#endif // MATERIAL_HPP