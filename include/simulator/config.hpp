#pragma once

#include <common/macros.hpp>
#include <memory>

namespace sim {

class Config {
public:
    INTERFACE_CTOR_DTORS(Config);

    static auto create() -> std::shared_ptr<Config>;
};

}    // namespace sim
