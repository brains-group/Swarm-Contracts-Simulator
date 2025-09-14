#include <common/logger.hpp>
#include <memory>
#include <simulator/config.hpp>
#include <simulator/simulator.hpp>

#include "visualizer.hpp"
auto main() -> int {
    LogManager::instance().addHandler(simpleConsoleLogger);

    LOG(INFO) << "Running visualizer";

    std::unique_ptr<vis::Visualizer> vis = vis::Visualizer::create();

    Result<void> res = vis->run();
    if (!res) {
        LOG(ERROR) << "Visualizer failed with error: " << res.error();
        return res.error().value();
    }
    LOG(INFO) << "Visualizer finished, exiting";
}
