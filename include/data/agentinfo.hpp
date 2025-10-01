#pragma once
#include <cstdint>

#include <data/point.hpp>

namespace scs::data {

struct AgentInfo {
    uint64_t id;
    Point    loc;
    float    size;
    bool     physical;

    enum Type : uint8_t { CLIENT, WORKER } type;
};

}    // namespace scs::data
