#include "BaseSimulator.h"

#include <filesystem>

#include "Logger.h"
#include "TimeUtil.h"

namespace base {

Simulator::Simulator() {
  LOG(INFO) << "Beginning base initialization";

  std::filesystem::path data_dir("data");
  data_dir /= program_time(true);
  LOG(INFO) << "Opening " << data_dir << " to store run data";
  LOG(DEBUG) << "Opening " << data_dir / "info.log" << " and " << data_dir / "trace.log"
             << " for logging";
  Logger::addFile(data_dir / "info.log", INFO);
  Logger::addFile(data_dir / "trace.log", TRACE);

  LOG(INFO) << "Completed base initialization";
}

Simulator::~Simulator() = default;

void Simulator::run() {
  LOG(INFO) << "Beginning simulation run";
  while (!shouldTerminate()) {
    LOG(INFO) << "Simulating step " << m_step_num;
    LOG(DEBUG) << "Updating Contracts";
    for (auto& contract : m_contracts) {
      if (contract->shouldExecute_wrapper()) {
        LOG(INFO) << "Executing contract " << contract->ID;
        contract->execute_wrapper();
      } else {
        LOG(TRACE) << "Not executing contract " << contract->ID;
      }
    }
    LOG(DEBUG) << "Updating Agents";
    for (auto& agent : m_agents) {
      if (agent->active()) {
        LOG(TRACE) << "Simulating Agent " << agent->ID << " - Step " << m_step_num;
        agent->act();
      } else {
        LOG(TRACE) << "Agent " << agent->ID << " is inactive - Step " << m_step_num;
      }
    }
    LOG(TRACE) << "Updating Simulation";
    step();
    ++m_step_num;
  }
}

unsigned int Simulator::getStep() const { return m_step_num; }

}    // namespace base
