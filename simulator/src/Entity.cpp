#include "Entity.h"

#include "Logger.h"
#include "Simulator.h"

Entity::Entity(unsigned int ID_, Simulator* sim, int balance)
    : ID(ID_)
    , m_sim(sim)
    , m_balance(balance) 
    , m_activation_step(sim->getStep())
    , m_starting_balance(balance) {}

bool Entity::isActive() const {
  return m_sim->getStep() >= m_activation_step && m_sim->getStep() < m_deactivation_step;
}

void Entity::deactivate() {
  LOG(INFO) << "Deactivating Entity " << ID;
  if (!m_sim) { LOG(CRITICAL) << "NULL POINTER SIMULATOR"; }
  m_sim->notifyDeactivated(ID);
  m_deactivation_step = m_sim->getStep();
  m_balance           = 0;
}

bool Entity::pay(unsigned int recipient_ID, unsigned int amount) { return pay(m_sim->getEntity(recipient_ID), amount); }

bool Entity::pay(Entity* recipient, unsigned int amount) {
  if (m_balance < amount) { return false; }
  m_sim->notifyPayment(ID, recipient->ID, amount);
  m_balance -= amount;
  recipient->m_balance += amount;
  LOG(INFO) << "Entity " << ID << " Paying Entity " << recipient->ID << " Amount: " << amount;
  return true;
}

std::string Entity::csv_headers() {
  auto vals = {"ID", "activation_step", "deactivation_step", "starting_balance", "ending_balance"};
  std::ostringstream ss;
  for (const auto& val : vals) { ss << val << ","; }
  return ss.str();
}

std::string Entity::dump_csv() const {
  std::ostringstream ss;
  for (const auto& val :
       {ID, m_activation_step, m_deactivation_step, m_starting_balance, m_balance}) {
    ss << val << ",";
  }
  return ss.str();
}
