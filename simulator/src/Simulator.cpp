#include "Simulator.h"

#include <utility>

#include "Agent.h"
#include "Contract.h"
#include "Entity.h"
#include "Job.h"
#include "Logger.h"
#include "TimeUtil.h"

Simulator::Simulator() {
  LOG(INFO) << "Beginning Simulator Constructor";

  std::filesystem::path data_dir("data");
  data_dir /= program_time(true);
  LOG(INFO) << "Opening " << data_dir / "info.log" << " and " << data_dir / "trace.log"
            << " for logging";
  Logger::addFile(data_dir / "info.log", INFO);
  Logger::addFile(data_dir / "trace.log", TRACE);

  m_frameMetrics.emplace_back(0);

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
      for (const FrameMetrics& frame : m_frameMetrics) { metrics_csv << frame.dump_csv() << "\n"; }
    }
  }

  {
    std::filesystem::path agents_path = data_dir / "agent_metrics.csv";
    LOG(INFO) << "Opening " << agents_path << " to store agent metrics";
    std::ofstream agents_csv(agents_path);
    if (!agents_csv.good()) {
      LOG(ERROR) << "Unable to open " << agents_path;
    } else {
      agents_csv << Agent::csv_headers() << "\n";
      for (const Agent* agent : m_agents) { agents_csv << agent->dump_csv() << "\n"; }
    }
  }

  {
    std::filesystem::path contracts_path = data_dir / "contract_metrics.csv";
    LOG(INFO) << "Opening " << contracts_path << " to store contract metrics";
    std::ofstream contracts_csv(contracts_path);
    if (!contracts_csv.good()) {
      LOG(ERROR) << "Unable to open " << contracts_path;
    } else {
      contracts_csv << Contract::csv_headers() << "\n";
      for (const Contract* contract : m_contracts) {
        contracts_csv << contract->dump_csv() << "\n";
      }
    }
  }

  {
    std::filesystem::path job_path = data_dir / "job_metrics.csv";
    LOG(INFO) << "Opening " << job_path << " to store job metrics";
    std::ofstream job_csv(job_path);
    if (!job_csv.good()) {
      LOG(ERROR) << "Unable to open " << job_path;
    } else {
      job_csv << Job::csv_headers() << "\n";
      for (const auto& job : m_jobs) { job_csv << job->dump_csv() << "\n"; }
    }
  }
}

void Simulator::run() {
  LOG(INFO) << "Initializing simualtor";
  for (auto& entity : m_entities) {
    Agent* a = dynamic_cast<Agent*>(entity.get());
    if (!a) { return; }
    for (unsigned int i : a->m_ability) {
      LOG(ERROR) << "BEFORE INIT >>>>> Agent " << a->ID << " ability " << i;
    }
  }
  init();
  LOG(INFO) << "Beginning simulation run";
  while (!shouldTerminate()) {
    m_frameMetrics.emplace_back(m_frameMetrics.size());
    LOG(DEBUG) << "Step " << getStep() << " - Beginning Step";
    LOG(TRACE) << "Step " << getStep() << " - Updating Entities";
    std::ranges::move(m_newEntities, std::back_inserter(m_entities));
    m_newEntities.clear();
    // Allowing entities to act
    for (auto& entity : m_entities) {
      if (entity->isActive()) {
        LOG(TRACE) << "Step " << getStep() << " - Entity " << entity->ID << " acting...";
        entity->act();
        Agent* agent = nullptr;
        if ((agent = dynamic_cast<Agent*>(entity.get())) != nullptr) {
          if (getStep() == agent->m_cooldown_step) {
            LOG(INFO) << "Agent " << agent->ID << " cooldown expired, paying out "
                      << agent->m_reward_after_cooldown;
            m_frameMetrics.back().job_rewards += agent->m_reward_after_cooldown;
            ++m_frameMetrics.back().jobs_completed;
            agent->m_balance += agent->m_reward_after_cooldown;
            agent->m_reward_after_cooldown = 0;
          }
        }
      }
    }
    LOG(TRACE) << "Step " << getStep() << " - Running Simulator Step";
    step();
  }
}

void Simulator::notifyDeactivated(unsigned int ID) {
  m_frameMetrics.back().destroyed_value += getEntity(ID)->balance();
  if constexpr (std::is_base_of_v<Agent, decltype(*getEntity(ID))>) {
    ++m_frameMetrics.back().agents_deactivated;
  } else if constexpr (std::is_base_of_v<Contract, decltype(*getEntity(ID))>) {
    ++m_frameMetrics.back().contracts_deactivated;
  } else {
    // LOG(CRITICAL) << "HIT UNREACHABLE THIS IS BAD";
    // std::unreachable();
  }
}

void Simulator::notifyPayment(unsigned int /* source_ID */, unsigned int /* dest_ID */,
                              unsigned int amount) {
  ++m_frameMetrics.back().transactions;
  m_frameMetrics.back().transaction_value += amount;
}

void Simulator::notifyClaimedJob(unsigned int /* job_ID */) {
  ++m_frameMetrics.back().jobs_claimed;
}
