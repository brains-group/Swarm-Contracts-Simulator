#pragma once

#include <agents/agent.hpp>
#include <common/macros.hpp>
#include <data/part.hpp>

namespace scs::contracts {

class Contract {
public:
    explicit Contract(agents::Agent* client, data::Part part)
        : m_part(std::move(part))
        , m_client(client) {}

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

    // TODO: Don't make this public to user code (Expand ContractHandle?)
    [[nodiscard]] auto isComplete() const -> bool { return m_complete; }
    auto               markComplete() -> void { m_complete = true; }

private:
    data::Part m_part;

    agents::Agent* m_client;
    agents::Agent* m_worker = nullptr;

    bool m_complete = false;
};

}    // namespace scs::contracts
