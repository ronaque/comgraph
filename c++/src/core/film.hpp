#ifndef _FILM_
#define _FILM_

#include "common.hpp"

namespace gc
{
class Film
{
  public:
    size_t width;
    size_t height;
    std::string type;
    std::string filename;
    std::string img_type;
    bool gamma_corrected;
    byte channels;

    void process_film(ParamSet parms, const bool quick);

    inline std::string to_string() {
      std::stringstream ss;
      ss << "Film(";
      ss << "width=" << width << ", ";
      ss << "height=" << height << ", ";
      ss << "type=" << type << ", ";
      ss << "filename=\"" << filename << "\", ";
      ss << "img_type=" << img_type << ", ";
      ss << "gamma_corrected=" << std::boolalpha << gamma_corrected;
      ss << ")";
      return ss.str();
    }
};

std::ostream &operator<<(std::ostream &os, const Film &film);

} // namespace gc

#endif