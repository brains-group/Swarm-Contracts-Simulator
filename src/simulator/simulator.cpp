#include <memory>
#include <vector>

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
        : m_config(config) {}

    auto load() -> Result<void> override { return {}; }
    auto runFrame() -> void override {}

    [[nodiscard]] auto getRoomCorners() const -> const std::vector<data::Point>& override {
        return m_config.roomCorners();
    }

    [[nodiscard]] auto getTargetCorners() const -> const std::vector<data::Point>& override {
        return m_config.targetCorners();
    }

private:
    const config::SimulatorConfig& m_config;
};

}    // namespace

auto Simulator::create() -> std::unique_ptr<Simulator> {
    return std::make_unique<SimulatorImpl>(config::Config::instance().getSimulatorConfig());
};

}    // namespace scs::sim
