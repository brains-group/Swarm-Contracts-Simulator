#pragma once

#include <data/point.hpp>

namespace scs::data {

struct Transform {
    Point loc;
    Angle rot;
    float size;
};

}    // namespace scs::data
