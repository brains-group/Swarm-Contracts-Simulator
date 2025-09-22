#include <memory>
#include <vector>

#include <agents/agentmanager.hpp>
#include <agents/environmentinterface.hpp>
#include <config/config.hpp>
#include <config/simulatorconfig.hpp>
#include <simulator/simulator.hpp>

namespace scs::sim {

namespace {

class SimulatorImpl : public sim::Simulator {
public:
    DELETE_COPY_MOVE(SimulatorImpl);
    DEFAULT_DTOR(SimulatorImpl);

    explicit SimulatorImpl(const config::SimulatorConfig& config)
        : m_config(config)
        , m_agents(agents::AgentManager::create(*this, m_config.initialAgentInfos())) {}

    auto runFrame() -> void override { m_agents->runAgents(); }

    [[nodiscard]] auto getRoomCorners() const -> const std::vector<data::Point>& override {
        return m_config.roomCorners();
    }

    [[nodiscard]] auto getTargetCorners() const -> const std::vector<data::Point>& override {
        return m_config.targetCorners();
    }

    [[nodiscard]] auto getMaterialInfos() const -> const std::vector<data::MaterialInfo>& override {
        return m_config.materialInfos();
    }

    [[nodiscard]] auto getAgentInfos() const -> const std::vector<data::AgentInfo>& override {
        return m_agents->getAgentInfos();
    }

    [[nodiscard]] auto getAgentInfo(uint64_t agentID) const -> const data::AgentInfo& override {
        return m_agents->getAgentInfo(agentID);
    }

private:
    const config::SimulatorConfig& m_config;

    std::unique_ptr<agents::AgentManager> m_agents;

    friend class agents::EnvironmentInterface;
};

}    // namespace

auto Simulator::create() -> std::unique_ptr<Simulator> {
    return std::make_unique<SimulatorImpl>(config::Config::instance().getSimulatorConfig());
};

}    // namespace scs::sim
