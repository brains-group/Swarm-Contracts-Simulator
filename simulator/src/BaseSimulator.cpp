#include "BaseSimulator.h"

#include <filesystem>

#include "Logger.h"
#include "TimeUtil.h"

namespace base {

Simulator::Simulator() {
  LOG(INFO) << "Beginning base initialization";

  std::filesystem::path data_dir("data");
  data_dir /= program_time(true);
  LOG(INFO) << "Storing run data in " << data_dir;
  LOG(DEBUG) << "Opening " << data_dir / "info.log" << " and " << data_dir / "trace.log"
             << " for logging";
  Logger::addFile(data_dir / "info.log", INFO);
  Logger::addFile(data_dir / "trace.log", TRACE);

  LOG(INFO) << "Completed base initialization";
}

Simulator::~Simulator() = default;

void Simulator::run() {
  LOG(INFO) << "Beginning run";
  while (!shouldTerminate()) {
    ++m_step_num;
    LOG(DEBUG) << "Simulating step " << m_step_num;
    for (Agent* agent : getAgents()) {
      LOG(TRACE) << "Simulating Agent " << agent->ID << " Step " << m_step_num;
      agent->act(this);
    }
    LOG(TRACE) << "Agents simulated, evolving state (" << m_step_num << " -> " << m_step_num + 1
               << ")";
    step(m_step_num);
  }
}

unsigned int Simulator::getStep() const { return m_step_num; }

}    // namespace base
