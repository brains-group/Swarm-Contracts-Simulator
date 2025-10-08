#pragma once

#include <data/angle.hpp>
#include <data/point.hpp>

namespace scs::data {

struct Transform {
    Transform(Point loc_, Angle rot_, float size_)
        : loc(loc_)
        , rot(rot_)
        , size(size_) {}
    // NOLINTBEGIN(*-non-private-member-variables-in-classes)
    Point loc;
    Angle rot;
    float size;
    // NOLINTEND(*-non-private-member-variables-in-classes)
};

}    // namespace scs::data
