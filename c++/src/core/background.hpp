#ifndef _BACKGROUND_
#define _BACKGROUND_

#include "common.hpp"
#include <vector>
namespace gc
{
class Background
{
  public:
    Color24 br;
    Color24 bl;
    Color24 tr;
    Color24 tl;
    std::vector<Color24> background;
    void process_background(const ParamSet& params);
    Color24 sample24(float i, float j);
    Spectrum sampleXYZ(float i, float j);

    std::string to_string() const
    {
        std::stringstream ss;
        ss << "Background:\n";
        ss << "  br = " << br << "\n";
        ss << "  bl = " << bl << "\n";
        ss << "  tr = " << tr << "\n";
        ss << "  tl = " << tl << "\n";
        return ss.str();
    }

  private:
    Color24 lerp(const Color24 &A, const Color24 &B, float t);
};

} // namespace gc

#endif