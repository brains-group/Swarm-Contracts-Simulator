#include "Simulator.h"

#include "Entity.h"
#include "Agent.h"
#include "Logger.h"
#include "TimeUtil.h"
#include "Contract.h"

Simulator::Simulator() {
  LOG(INFO) << "Beginning Simulator Constructor";

  std::filesystem::path data_dir("data");
  data_dir /= program_time(true);
  LOG(INFO) << "Opening " << data_dir / "info.log" << " and " << data_dir / "trace.log"
            << " for logging";
  Logger::addFile(data_dir / "info.log", INFO);
  Logger::addFile(data_dir / "trace.log", TRACE);

  m_frameMetrics.emplace_back();

  LOG(INFO) << "Completed Simulator Constructor";
}

Simulator::~Simulator() {
  LOG(INFO) << "Beginning Simulator Destructor";

  std::filesystem::path data_dir("data");
  data_dir /= program_time(true);
  {
    std::filesystem::path metrics_path = data_dir / "frame_metrics.csv";
    LOG(INFO) << "Opening " << metrics_path << " to store frame metrics";
    std::ofstream metrics_csv(metrics_path);
    if (!metrics_csv.good()) {
      LOG(ERROR) << "Unable to open " << metrics_path;
    } else {
      metrics_csv << FrameMetrics::csv_headers() << "\n";
      for (const FrameMetrics& frame : m_frameMetrics) { metrics_csv << frame << "\n"; }
    }
  }

  {
    std::filesystem::path agents_path = data_dir / "general_agent_metrics.csv";
    LOG(INFO) << "Opening " << agents_path << " to store frame metrics";
    std::ofstream agents_csv(agents_path);
    if (!agents_csv.good()) {
      LOG(ERROR) << "Unable to open " << agents_path;
    } else {
      agents_csv << Agent::csv_headers() << "\n";
      for (const Agent* agent : m_agents) { agents_csv << *agent << "\n"; }
    }
  }

  {
    std::filesystem::path contracts_path = data_dir / "general_contract_metrics.csv";
    LOG(INFO) << "Opening " << contracts_path << " to store frame metrics";
    std::ofstream contracts_csv(contracts_path);
    if (!contracts_csv.good()) {
      LOG(ERROR) << "Unable to open " << contracts_path;
    } else {
      contracts_csv << Agent::csv_headers() << "\n";
      for (const Contract* contract : m_contracts) { contracts_csv << *contract << "\n"; }
    }
  }
}

void Simulator::run() {
  LOG(INFO) << "Initializing simualtor";
  init();
  LOG(INFO) << "Beginning simulation run";
  while (!shouldTerminate()) {
    m_frameMetrics.emplace_back();
    LOG(DEBUG) << "Step " << getStep() << " - Beginning Step";
    for (auto& entity : m_entities) {
      if (entity->isActive()) {
        LOG(TRACE) << "Step " << getStep() << " - Entity " << entity->ID << " acting...";
        entity->act();
      }
    }
    LOG(TRACE) << "Step " << getStep() << " - Running Simulator Step";
    step();
  }
}
