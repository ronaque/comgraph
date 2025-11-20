#include "../core/paramset.hpp"
#include "../core/common.hpp"
#include <vector>
#include <sstream>

namespace gc
{

class LookAt
{
  public:
    Point3<real_type> look_from;
    Point3<real_type> look_at;
    Vector3<real_type> up;

    void process_look_at(ParamSet params);

    inline std::string to_string() {
      std::stringstream ss;
      ss << "Look At(Look From: " << look_from << ", Look At: " << look_at << ", Up: " << up << ")";
      return ss.str();
    }
};
}