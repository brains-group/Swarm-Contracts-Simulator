#pragma once

#include <agents/controller.hpp>

#define CONTROLLER_SUBCLASS(TYPE)                     \
    class TYPE##Controller : public Controller {      \
        auto run(SimInterface& sim) -> void override; \
    };

namespace scs::agents {

FOR_EACH(CONTROLLER_SUBCLASS, CONTROLLER_TYPES)

}
