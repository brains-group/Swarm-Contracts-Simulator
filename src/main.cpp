#include <cstdlib>

#include "logger.hpp"
#include "simulator.hpp"

auto main() -> int {
    // uint64_t seed = std::random_device{}();
    uint64_t seed = 12345;

    LogManager::instance().addHandler(simpleConsoleLogger);

    Simulator sim(Config::instance(), seed);

    sim.run();

    return EXIT_SUCCESS;
}
