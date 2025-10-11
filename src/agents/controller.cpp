#include <agents/controller.hpp>

#include "controllertypes.hpp"    // IWYU pragma: keep (used for macro CONTROLLER_TYPES below)

#define CONTROLLER_CASE(TYPE) \
    case Type::TYPE: return std::make_shared<TYPE##Controller>();

namespace scs::agents {

auto Controller::create(Type type) -> std::shared_ptr<Controller> {
    switch (type) { FOR_EACH(CONTROLLER_CASE, CONTROLLER_TYPES) }
}

}    // namespace scs::agents
