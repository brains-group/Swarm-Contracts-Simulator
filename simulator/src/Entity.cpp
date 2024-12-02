#include "Entity.h"

#include "Logger.h"
#include "Simulator.h"

bool Entity::isActive() const {
  return m_sim->getStep() >= m_activation_step && m_sim->getStep() < m_deactivation_step;
}

void Entity::deactivate() { m_deactivation_step = m_sim->getStep(); }

Entity::Entity(unsigned int ID_, Simulator* sim)
    : ID(ID_)
    , m_sim(sim)
    , m_activation_step(sim->getStep()) {
  LOG(INFO) << "Entity " << ID << " created";
}
