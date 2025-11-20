#include "material.hpp"
#include "../core/paramset.hpp"

namespace gc {

std::shared_ptr<Material> Material::create(const ParamSet& ps) {
    // Get color from parameters or use default (white)
    Spectrum color = ps.retrieve("color", Spectrum{1.0f, 1.0f, 1.0f});
    
    // Create and return a new material instance
    return std::make_shared<Material>(color);
}

} // namespace gc