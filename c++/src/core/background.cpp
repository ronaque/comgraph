#include "../msg_system/error.hpp"
#include "app.hpp"
#include "film.hpp"
#include "paramset.hpp"

namespace gc
{
Color24 Background::lerp(const Color24 &A, const Color24 &B, float t)
{
    byte rchannel = (1 - t) * A.x + (t * B.x);
    byte gchannel = (1 - t) * A.y + (t * B.y);
    byte bchannel = (1 - t) * A.z + (t * B.z);

    return Color24{rchannel, gchannel, bchannel};
}

Color24 Background::sample24(float i, float j)
{
    Color24 Xt = lerp(tl, tr, i);
    Color24 Xb = lerp(bl, br, i);
    return lerp(Xt, Xb, j);
}

Spectrum Background::sampleXYZ(float i, float j)
{
    // TRACE(FORMAT("Sampling Color XYZ. i:", i, " j:", j));
    Color24 Xt = lerp(tl, tr, i);
    Color24 Xb = lerp(bl, br, i);
    Color24 color = lerp(Xb, Xt, j);
    // TRACE(FORMAT("Color: ", color.to_string()));
    return {float(color.x) / 255, float(color.y) / 255, float(color.z) / 255};
}

void Background::process_background(const ParamSet &params)
{
    TRACE("Processing background parameters");
    std::string type = params.retrieve<std::string>("type", "single_color");

    if (type == "single_color")
    {
        Color24 color = params.retrieve("color", Color24{0, 0, 0});
        bl = color;
        tl = color;
        tr = color;
        br = color;
    }
    else if (type == "4_colors")
    {
        bl = params.retrieve("bl", Color24{0, 0, 0});
        tl = params.retrieve("tl", Color24{0, 0, 0});
        tr = params.retrieve("tr", Color24{0, 0, 0});
        br = params.retrieve("br", Color24{0, 0, 0});
    }
    DEBUG(FORMAT("Parsed Background: ", to_string()));
};

} // namespace gc