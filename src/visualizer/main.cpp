#include <common/logger.hpp>

#include "visualizer.hpp"

auto main() -> int {
    LogManager::instance().addHandler(simpleConsoleLogger);

    scs::vis::Visualizer vis;
    vis.run();
}
