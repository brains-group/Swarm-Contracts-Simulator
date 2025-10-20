#pragma once

#include <cstdint>
#include <vector>

#include <common/macros.hpp>

namespace scs {

namespace agents {
class Agent;
}

namespace sim {

template <typename T, typename Container = std::vector<T>> class Handle {
public:
    Handle(Container* container, uint64_t index)
        : m_container(container)
        , m_index(index) {}

    DEFAULT_COPY_MOVE(Handle);
    DEFAULT_DTOR(Handle);

    auto get() const -> const T& { return (*m_container)[m_index]; }
    auto get() -> T& { return (*m_container)[m_index]; }

private:
    Container* m_container;
    uint64_t   m_index;
};

class Contract;

using ContractHandle = Handle<Contract>;
using AgentHandle    = Handle<agents::Agent>;

}    // namespace sim

}    // namespace scs
