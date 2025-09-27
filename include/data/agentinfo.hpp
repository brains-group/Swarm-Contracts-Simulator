#pragma once
#include <cstdint>

#include <data/point.hpp>

namespace scs::data {

struct AgentInfo {
    uint64_t id;
    Point    loc;
    float    size;

    enum Type : uint8_t { DEFAULT } type;
};

}    // namespace scs::data
