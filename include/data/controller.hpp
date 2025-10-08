#pragma once

#include <cstdint>
#include <memory>

#include <common/logger.hpp>
#include <common/macros.hpp>

#define CONTROLLER_CASE(TYPE) \
    case Type::TYPE: return std::make_shared<TYPE##Controller>();

#define CONTROLLER_SUBCLASS(TYPE)                \
    class TYPE##Controller : public Controller { \
        auto run() -> void override;             \
    };

#define CONTROLLER_TYPES(...)                                                  \
    class Controller {                                                         \
    public:                                                                    \
        INTERFACE_CTOR_DTORS(Controller);                                      \
        enum class Type : uint8_t { __VA_ARGS__ };                             \
                                                                               \
        static auto create(Type type) -> std::shared_ptr<Controller>;          \
                                                                               \
        virtual auto run() -> void = 0;                                        \
    };                                                                         \
    FOR_EACH(CONTROLLER_SUBCLASS, __VA_ARGS__)                                 \
    inline auto Controller::create(Type type) -> std::shared_ptr<Controller> { \
        switch (type) { FOR_EACH(CONTROLLER_CASE, __VA_ARGS__) }               \
    }

namespace scs::data {

CONTROLLER_TYPES(Client, Worker);

inline auto ClientController::run() -> void { LOG(INFO) << "Running a client"; }
inline auto WorkerController::run() -> void { LOG(INFO) << "Running a worker"; }

}    // namespace scs::data

#undef CONTROLLER_CASE
#undef CONTROLLER_SUBCLASS
#undef CONTROLLER_TYPES
