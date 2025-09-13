#include "visualizer.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <simulator/config.hpp>

namespace {

class VisualizerImpl : public vis::Visualizer {
public:
    DELETE_COPY_MOVE(VisualizerImpl);
    DEFAULT_DTOR(VisualizerImpl);
    explicit VisualizerImpl(const sim::Simulator& simulator)
        : m_window(sf::VideoMode({1920U, 1080U}), "CMake SMFL Project")
        , m_simulator(simulator) {
        m_window.setFramerateLimit(144);
    }

    auto run() -> Result<void> override {
        UNUSED(m_simulator);
        while (m_window.isOpen()) {
            while (const std::optional<sf::Event> event = m_window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) { m_window.close(); }
            }

            m_window.clear();
            m_window.display();
        }
        return {};
    }

private:
    sf::RenderWindow      m_window;
    const sim::Simulator& m_simulator;
};

}    // namespace

auto vis::Visualizer::create(const sim::Simulator& simulator) -> std::unique_ptr<Visualizer> {
    return std::make_unique<VisualizerImpl>(simulator);
}
