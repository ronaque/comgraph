#include "film.hpp"
#include "paramset.hpp"
#include "../msg_system/error.hpp"

namespace gc
{
void Film::process_film(ParamSet params, const bool quick)
{
    TRACE("Processing Film");
    if (quick) {
        width = params.retrieve("w_res", 800)/4;
        height = params.retrieve("h_res", 600)/4;
    } else {
        width = params.retrieve("w_res", 800);
        height = params.retrieve("h_res", 600);
    }
    type = params.retrieve<std::string>("type", "image");
    filename = params.retrieve<std::string>("filename", "out.png");
    img_type = params.retrieve<std::string>("img_type", "png"); // Default png
    // channels = img_type == 2 ? 4 : 3;
    channels = 3;
    gamma_corrected = params.retrieve("gamma_corrected", false);
    DEBUG(FORMAT("Processed Film: ", to_string()));
}

std::ostream &operator<<(std::ostream &os, const Film &film)
{
    os << "Film(";
    os << "width=" << film.width << ", ";
    os << "height=" << film.height << ", ";
    os << "type=" << film.type << ", ";
    os << "filename=\"" << film.filename << "\", ";
    os << "img_type=" << film.img_type << ", ";
    os << "gamma_corrected=" << std::boolalpha << film.gamma_corrected;
    os << ")";
    return os;
}
} // namespace gc