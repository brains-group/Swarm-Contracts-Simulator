#include "../controllertypes.hpp"

namespace scs::agents {

auto WorkerController::run(SimInterface& /* sim */) -> void { LOG(FINE) << "Running a worker"; }

}    // namespace scs::agents
