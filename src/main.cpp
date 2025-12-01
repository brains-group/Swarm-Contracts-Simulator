#include <cstdlib>

#include "logger.hpp"
#include "simulator.hpp"

auto main() -> int {
    LogManager::instance().addHandler(simpleConsoleLogger);

    Simulator sim(Config::instance(), std::random_device{}());

    sim.run();

    return EXIT_SUCCESS;
}
