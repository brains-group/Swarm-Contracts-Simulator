#include <memory>
#include <simulator/config.hpp>
#include <simulator/simulator.hpp>
#include <vector>

namespace {

class SimulatorImpl : public sim::Simulator {
public:
    DELETE_COPY_MOVE(SimulatorImpl);
    DEFAULT_DTOR(SimulatorImpl);

    explicit SimulatorImpl(std::shared_ptr<const sim::Config>&& config)
        : m_config(std::move(config)) {
        m_agents.emplace_back(100, 100);
        m_agents.emplace_back(200, 200);
        m_agents.emplace_back(300, 300);
    }

    auto runFrame() -> void override {}

    auto getConfig() -> const sim::Config& override { return *m_config; }
    auto getAgents() -> const std::vector<sim::Agent>& override { return m_agents; }

private:
    const std::shared_ptr<const sim::Config> m_config;
    std::vector<sim::Agent>                  m_agents;
};

}    // namespace

auto sim::Simulator::create(std::shared_ptr<const sim::Config> config)
    -> std::unique_ptr<sim::Simulator> {
    return std::make_unique<SimulatorImpl>(std::move(config));
};

