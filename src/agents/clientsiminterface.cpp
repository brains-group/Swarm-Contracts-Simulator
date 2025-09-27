#include <memory>

#include <agents/clientsiminterface.hpp>
#include <simulator/agentsimulator.hpp>
#include <simulator/clientsimulator.hpp>

namespace scs::agents {

namespace {

class ClientSimInterfaceImpl : public ClientSimInterface {
public:
    ClientSimInterfaceImpl(sim::ClientSimulator& sim, uint64_t clientID)
        : m_sim(sim)
        , m_clientID(clientID) {}

    auto placeOrder(const data::Color& order) const -> void override {
        m_sim.placeOrder(m_clientID, order);
    }

private:
    sim::ClientSimulator& m_sim;
    uint64_t              m_clientID;
};

}    // namespace

auto ClientSimInterface::create(sim::ClientSimulator& sim, uint64_t clientID)
    -> std::unique_ptr<ClientSimInterface> {
    return std::make_unique<ClientSimInterfaceImpl>(sim, clientID);
}

}    // namespace scs::agents
