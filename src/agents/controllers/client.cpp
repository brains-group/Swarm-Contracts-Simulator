#include "../controllertypes.hpp"

namespace scs::agents {

auto ClientController::run(SimInterface& /* sim */) -> void { LOG(FINE) << "Running a client"; }

}    // namespace scs::agents
