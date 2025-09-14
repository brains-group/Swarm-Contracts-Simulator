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
        // TODO: Make the initial config come from the config class
        m_agents.emplace_back(Point(100, 100));
        m_agents.emplace_back(Point(200, 200));
        m_agents.emplace_back(Point(300, 300));

        m_target.start = Point(0, 0);
        m_target.end   = Point(50, 50);

        m_materials.emplace_back(Rect(Point(600, 600), Point(650, 650)), sim::Material(255, 0, 0));
    }

    auto runFrame() -> void override {
        for (sim::Agent& agent : m_agents) { agent.runFrame(); }
    }

    [[nodiscard]] auto getConfig() const -> const sim::Config& override { return *m_config; }
    [[nodiscard]] auto getAgents() const -> const std::vector<sim::Agent>& override {
        return m_agents;
    }
    [[nodiscard]] auto getTargetPosition() const -> Point override { return m_target.start; }
    [[nodiscard]] auto getMaterialStores() const
        -> const std::vector<sim::MaterialStore>& override {
        return m_materials;
    }

private:
    const std::shared_ptr<const sim::Config> m_config;
    std::vector<sim::Agent>                  m_agents;
    Rect                                     m_target;
    std::vector<sim::MaterialStore>          m_materials;
};

}    // namespace

auto sim::Simulator::create(std::shared_ptr<const sim::Config> config)
    -> std::unique_ptr<sim::Simulator> {
    return std::make_unique<SimulatorImpl>(std::move(config));
};

