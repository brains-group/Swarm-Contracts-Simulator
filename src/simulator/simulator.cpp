#include <memory>
#include <vector>

#include <config/config.hpp>
#include <simulator/simulator.hpp>

#include "config/simulatorconfig.hpp"

namespace {

class SimulatorImpl : public scs::sim::Simulator {
public:
    DELETE_COPY_MOVE(SimulatorImpl);
    DEFAULT_DTOR(SimulatorImpl);

    explicit SimulatorImpl(const scs::config::SimulatorConfig& config)
        : m_config(config) {
        // TODO: Make the initial config come from the config class
        m_agents.emplace_back(Point(100, 100));
        m_agents.emplace_back(Point(200, 200));
        m_agents.emplace_back(Point(300, 300));

        m_target.start = Point(0, 0);
        m_target.end   = Point(50, 50);

        m_materials.emplace_back(Rect(Point(600, 600), Point(650, 650)),
                                 scs::sim::Material(255, 0, 0));
    }

    auto runFrame() -> void override {
        for (scs::sim::Agent& agent : m_agents) { agent.runFrame(); }
    }

    [[nodiscard]] auto getAgents() const -> const std::vector<scs::sim::Agent>& override {
        return m_agents;
    }
    [[nodiscard]] auto getTargetPosition() const -> Point override { return m_target.start; }
    [[nodiscard]] auto getMaterialStores() const
        -> const std::vector<scs::sim::MaterialStore>& override {
        return m_materials;
    }

private:
    const scs::config::SimulatorConfig&  m_config;
    std::vector<scs::sim::Agent>         m_agents;
    Rect                                 m_target;
    std::vector<scs::sim::MaterialStore> m_materials;
};

}    // namespace

auto scs::sim::Simulator::create() -> std::unique_ptr<sim::Simulator> {
    return std::make_unique<SimulatorImpl>(scs::config::Config::instance().getSimulatorConfig());
};

