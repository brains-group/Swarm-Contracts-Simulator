#include <config/config.hpp>
#include <config/simulatorconfig.hpp>
#include <memory>
#include <simulator/simulator.hpp>
#include <vector>

namespace scs::sim {

namespace {

class SimulatorImpl : public sim::Simulator {
public:
    DELETE_COPY_MOVE(SimulatorImpl);
    DEFAULT_DTOR(SimulatorImpl);

    explicit SimulatorImpl(const config::SimulatorConfig& config)
        : m_config(config)
        , m_agents(m_config.initialAgentInfos()) {}

    auto load() -> Result<void> override { return {}; }
    auto runFrame() -> void override {}

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
        return m_agents;
    }

private:
    const config::SimulatorConfig& m_config;

    std::vector<data::AgentInfo> m_agents;
};

}    // namespace

auto Simulator::create() -> std::unique_ptr<Simulator> {
    return std::make_unique<SimulatorImpl>(config::Config::instance().getSimulatorConfig());
};

}    // namespace scs::sim
