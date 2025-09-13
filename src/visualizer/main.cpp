#include <common/logger.hpp>
#include <memory>
#include <simulator/config.hpp>
#include <simulator/simulator.hpp>

#include "visualizer.hpp"
auto main() -> int {
    std::unique_ptr<sim::Simulator> sim = sim::Simulator::create(sim::Config::create());

    std::unique_ptr<vis::Visualizer> vis = vis::Visualizer::create(*sim);

    Result<void> res = vis->run();
    if (!res) { LOG(ERROR) << "Visualizer failed with error: " << res.error(); }
}
