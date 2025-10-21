#pragma once

#include <agents/agent.hpp>
#include <common/macros.hpp>
#include <data/part.hpp>

namespace scs::contracts {

class Contract {
public:
    explicit Contract(uint64_t id, std::shared_ptr<agents::Agent> client, data::Part part)
        : m_id(id)
        , m_part(std::move(part))
        , m_client(std::move(client)) {}

    DELETE_COPY(Contract);
    DEFAULT_MOVE(Contract);
    DEFAULT_DTOR(Contract);

    [[nodiscard]] auto getID() const -> uint64_t { return m_id; }

    [[nodiscard]] auto getPart() const -> const data::Part& { return m_part; }

    [[nodiscard]] auto getClient() -> std::shared_ptr<agents::Agent> { return m_client; }
    [[nodiscard]] auto getClient() const -> const std::shared_ptr<agents::Agent>& {
        return m_client;
    }

    [[nodiscard]] auto setWorker(std::shared_ptr<agents::Agent> worker) -> bool {
        if (m_worker != nullptr) { return false; }
        m_worker = std::move(worker);
        return true;
    }
    [[nodiscard]] auto getWorker() -> std::shared_ptr<agents::Agent> { return m_worker; }
    [[nodiscard]] auto getWorker() const -> const std::shared_ptr<agents::Agent>& {
        return m_worker;
    }

    // TODO: Don't make this public to user code (Expand ContractHandle?)
    [[nodiscard]] auto isComplete() const -> bool { return m_complete; }
    auto               markComplete() -> void { m_complete = true; }

private:
    uint64_t   m_id;
    data::Part m_part;

    std::shared_ptr<agents::Agent> m_client;
    std::shared_ptr<agents::Agent> m_worker = nullptr;

    bool m_complete = false;
};

}    // namespace scs::contracts
