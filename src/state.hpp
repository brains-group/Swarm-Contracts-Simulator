#pragma once

#include <memory>
#include <vector>

#include "agent.hpp"
#include "contract.hpp"

struct State {
    std::size_t                            frame = 0;
    std::vector<std::unique_ptr<Agent>>    agents;
    std::vector<std::unique_ptr<Contract>> contracts;
};
