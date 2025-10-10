#pragma once

#include <agents/agent.hpp>
#include <common/macros.hpp>
#include <data/part.hpp>

namespace scs::contracts {

class Contract {
public:
    explicit Contract(agents::Agent* client)
        : m_client(client) {}

    DELETE_COPY(Contract);
    DEFAULT_MOVE(Contract);
    DEFAULT_DTOR(Contract);

    [[nodiscard]] auto getClient() -> agents::Agent* { return m_client; }
    [[nodiscard]] auto getClient() const -> const agents::Agent* { return m_client; }

    [[nodiscard]] auto setWorker(agents::Agent* worker) -> bool {
        if (m_worker != nullptr) { return false; }
        m_worker = worker;
        return true;
    }
    [[nodiscard]] auto getWorker() -> agents::Agent* { return m_worker; }
    [[nodiscard]] auto getWorker() const -> const agents::Agent* { return m_worker; }

private:
    data::Part m_part;

    agents::Agent* m_client;
    agents::Agent* m_worker = nullptr;
};

}    // namespace scs::contracts
