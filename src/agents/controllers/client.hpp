#pragma once

#include <random>

#include <agents/controller.hpp>
#include <agents/siminterface.hpp>
#include <data/material.hpp>

namespace scs::agents {

class ClientController : public Controller {
public:
    auto run(SimInterface& sim) -> void override {
        int num = std::ranges::count_if(
            sim.getContracts(), [](const auto& contract) { return !contract->isComplete(); });

        while (num++ < 5) {
            auto contractPtr = sim.createContract(nextOrder());
            LOG(INFO) << "Created contract " << contractPtr->getID();
        }
    }

    auto nextOrder() -> data::Part {
        data::Part part = {};
        // Randomly choose order size
        std::uniform_int_distribution<unsigned int> sizeDist(1, 6);
        unsigned int                                size = sizeDist(m_randomGen);

        // Randomly choose a color for each index
        std::bernoulli_distribution colorDist(.333);
        while (part.size() < size) {
            data::Material mat(colorDist(m_randomGen) ? 255 : 0, colorDist(m_randomGen) ? 255 : 0,
                               colorDist(m_randomGen) ? 255 : 0);
            // Black is an invalid color
            if (mat != data::Material::Black && mat != data::Material::White) {
                part.emplace_back(mat);
            }
        }
        return part;
    }

private:
    std::random_device m_rd;
    std::mt19937       m_randomGen;
};

}    // namespace scs::agents
