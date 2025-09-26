#pragma once

#include <cstdint>

namespace scs::data {

struct ClientInfo {
    uint64_t id;
    enum Type : uint8_t { DEFAULT } type;
};

};    // namespace scs::data
