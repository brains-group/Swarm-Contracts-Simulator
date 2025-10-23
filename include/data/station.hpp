#pragma once

#include <cstdint>

#include <data/rect.hpp>

namespace scs::data {

struct Station {
    enum Type : uint8_t { ADD, MIX };

    Point loc;
    float size;
    Type  type;
};

}    // namespace scs::data
