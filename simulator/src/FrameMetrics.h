#pragma once

#include <iostream>
#include <sstream>
#include <string>

/**
 * Stores the data associated with each frame. All values are "deltas", meaning they do not
 * accumulate between frames. Aggregates can be found by post-processing of produced data
 */
struct FrameMetrics {
  FrameMetrics(unsigned int frame_number_)
      : frame_number(frame_number_) {}

  const unsigned int frame_number;

  // ENTITY METRICS
  unsigned int created_value   = 0;    // Value added by creating Entities with balances
  unsigned int destroyed_value = 0;    // Value lost by deactivating Entities with balances

  // CONTRACT METRICS
  unsigned int contracts_created     = 0;    // Number of contracts created
  unsigned int contracts_deactivated = 0;    // Number of contracts deactivated
  unsigned int contracts_expired     = 0;    // Number of contracts expired
  unsigned int contract_value        = 0;    // Total value held by contracts

  // AGENT METRICS
  unsigned int agents_created     = 0;    // Number of agents created
  unsigned int agents_deactivated = 0;    // Number of agents deactivated
  unsigned int agent_value        = 0;    // Total value held by agents

  // TRANSACTION METRICS
  unsigned int transactions      = 0;    // Number of transactions
  unsigned int transaction_value = 0;    // Total value of transactions

  // JOB METRICS
  unsigned int jobs_created    = 0;    // Number of jobs created
  unsigned int jobs_claimed    = 0;    // Number of jobs claimed
  unsigned int jobs_contracted = 0;    // Number of jobs assigned to contracts
  unsigned int jobs_completed  = 0;    // Number of completed jobs
  unsigned int job_rewards     = 0;    // Reward issued by completing jobs
  unsigned int job_costs       = 0;    // Cost paid to complete jobs

  static std::string csv_headers() {
    auto               vals = {"frame_number",
                               "created_value",
                               "destroyed_value",
                               "contracts_created",
                               "contracts_expired",
                               "agents_created",
                               "agents_deactivated",
                               "transactions",
                               "transaction_value",
                               "jobs_created",
                               "jobs_contracted",
                               "uncontracted_jobs_expired",
                               "contracted_jobs_completed",
                               "contracted_jobs_expired"};
    std::ostringstream ss;
    for (const auto& val : vals) { ss << val << ","; }
    return ss.str();
  }

  std::string dump_csv() const {
    std::ostringstream ss;
    auto               vals = {frame_number,
                               created_value,
                               destroyed_value,
                               contracts_created,
                               contracts_expired,
                               agents_created,
                               agents_deactivated,
                               transactions,
                               transaction_value,
                               jobs_created,
                               jobs_contracted};
    for (const auto& val : vals) { ss << val << ","; }
    return ss.str();
  }
};
