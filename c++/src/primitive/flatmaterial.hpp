#include "common.hpp"
#include "material.hpp"


namespace gc{
    class FlatMaterial : public Material {
public:
    FlatMaterial(Spectrum color) : color(color) {}
    
    Spectrum get_color() const override { return color; }

private:
    Spectrum color;
};

}


