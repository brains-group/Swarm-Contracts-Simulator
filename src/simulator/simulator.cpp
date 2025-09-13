#include <memory>
#include <simulator/config.hpp>
#include <simulator/simulator.hpp>

namespace {

class SimulatorImpl : public sim::Simulator {
public:
    DELETE_COPY_MOVE(SimulatorImpl);
    DEFAULT_DTOR(SimulatorImpl);

    explicit SimulatorImpl(std::shared_ptr<const sim::Config>&& config)
        : m_config(std::move(config)) {}

    auto run() -> Result<void> override { return {}; }

    auto getConfig() -> const sim::Config& override { return *m_config; }

private:
    const std::shared_ptr<const sim::Config> m_config;
};

}    // namespace

auto sim::Simulator::create(std::shared_ptr<const sim::Config> config)
    -> std::unique_ptr<sim::Simulator> {
    return std::make_unique<SimulatorImpl>(std::move(config));
};

