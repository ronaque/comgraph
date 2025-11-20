#include "look_at.hpp"
#include "../msg_system/error.hpp"

namespace gc {
    void LookAt::process_look_at(ParamSet params) {
        TRACE("Processing Look At");
        look_from = params.retrieve<Point3<real_type>>("look_from", Point3<real_type>{0, 0, 0});
        look_at = params.retrieve<Point3<real_type>>("look_at", Point3<real_type>{0, 0, 0});
        up = params.retrieve<Vector3<real_type>>("up", {0, 0, 0});
        DEBUG(FORMAT("Processed Look At: ", to_string()));
    }
}