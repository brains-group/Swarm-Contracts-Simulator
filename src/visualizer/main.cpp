#include <memory>

#include <common/logger.hpp>

#include "visualizer.hpp"

auto main() -> int {
    LogManager::instance().addHandler(simpleConsoleLogger);

    LOG(INFO) << "Running visualizer";

    std::unique_ptr<scs::vis::Visualizer> vis = scs::vis::Visualizer::create();

    Result<void> res = vis->run();
    if (!res) {
        LOG(ERROR) << "Visualizer failed with error: " << res.error();
        return res.error().value();
    }

    LOG(INFO) << "Visualizer finished, exiting";
}
