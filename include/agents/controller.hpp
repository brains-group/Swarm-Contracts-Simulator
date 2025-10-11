#pragma once

#include <cstdint>
#include <memory>

#include <common/logger.hpp>
#include <common/macros.hpp>

#define CONTROLLER_TYPES Client, Worker

#define CONTROLLER_CLASS(...)                                         \
    class Controller {                                                \
    public:                                                           \
        INTERFACE_CTOR_DTORS(Controller);                             \
        enum class Type : uint8_t { __VA_ARGS__ };                    \
                                                                      \
        static auto create(Type type) -> std::shared_ptr<Controller>; \
                                                                      \
        virtual auto run() -> void = 0;                               \
    };

namespace scs::agents {

CONTROLLER_CLASS(CONTROLLER_TYPES);

}    // namespace scs::agents

