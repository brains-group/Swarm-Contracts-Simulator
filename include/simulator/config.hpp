#pragma once

#include <common/macros.hpp>
#include <memory>

namespace sim {

class Config {
public:
    INTERFACE_CTOR_DTORS(Config);

    static auto create() -> std::unique_ptr<Config>;
};

}    // namespace sim
