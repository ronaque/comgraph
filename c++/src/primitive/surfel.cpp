#include "surfel.hpp"

namespace gc
{

std::string Surfel::to_string() const
{
    std::ostringstream oss;
    oss << "Surfel: {\n"
        << "  hit_point: " << hit_point << ", "
        << "  normal: " << normal << ", "
        << "  wo: " << out_dir << " "
        << "  primitive: " << (primitive ? primitive->to_string() : "nullptr") << " "
        << "}";
    return oss.str();
}
} // namespace gc