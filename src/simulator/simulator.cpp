#include <agents/agentmanager.hpp>
#include <agents/environmentinterface.hpp>
#include <common/logger.hpp>
#include <config/config.hpp>
#include <config/simulatorconfig.hpp>
#include <memory>
#include <simulator/agentsimulator.hpp>
#include <simulator/simulator.hpp>
#include <utility>
#include <vector>

namespace scs::sim {

namespace {

class SimulatorImpl : public sim::AgentSimulator {
public:
    DELETE_COPY_MOVE(SimulatorImpl);
    DEFAULT_DTOR(SimulatorImpl);

    explicit SimulatorImpl(const config::SimulatorConfig& config)
        : m_config(config)
        , m_agents(agents::AgentManager::create(*this, m_config.initialAgentInfos())) {}

    auto runFrame() -> void override {
        ++m_frameNumber;
        LOG(INFO) << "Running frame " << m_frameNumber;
        m_agents->runAgents();
    }

    [[nodiscard]] auto getFrameNumber() const -> unsigned int override { return m_frameNumber; }

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
        return std::as_const(*m_agents).getAgentInfo(agentID);
    }

    [[nodiscard]] auto moveAgent(uint64_t agentID, const data::Vector& vec) -> bool override {
        // NOTE: We trust the agent environment interface to do the necessary checks (this may be a
        // bad idea, decide later)
        return m_agents->moveAgent(agentID, vec);
    }

private:
    const config::SimulatorConfig& m_config;

    // NOTE: I need to make frame 0 just have starting config, frame 1 is where agents execute
    unsigned int                          m_frameNumber = 0;
    std::unique_ptr<agents::AgentManager> m_agents;
};

}    // namespace

auto Simulator::create() -> std::unique_ptr<Simulator> {
    return std::make_unique<SimulatorImpl>(config::Config::instance().getSimulatorConfig());
};

}    // namespace scs::sim
