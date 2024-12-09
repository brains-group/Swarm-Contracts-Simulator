#pragma once

#include <array>
#include <limits>
#include "Logger.h"
#include <ranges>

class Entity;

constexpr unsigned int DIM = 2;

struct Job {
  Job(unsigned int ID_, unsigned int step, unsigned int deadline,
      const std::array<unsigned int, DIM>& vec)
      : ID(ID_)
      , created_step(step)
      , deadline_step(deadline)
      , job_vector(vec) {
        LOG(INFO) << "Created job " << ID << " with deadline " << deadline << " and difficulty " << job_vector;
      }

  // Unique Identifier
  const unsigned int ID;

  // The step the job was created on
  const unsigned int created_step;

  // The step the job must be completed by
  const unsigned int deadline_step;

  // The vector representing the "difficulty" of the job
  const std::array<unsigned int, DIM> job_vector;

  // The step the job was claimed on
  unsigned int claimed_step = std::numeric_limits<unsigned int>::max();

  // The step the job was contracted on
  unsigned int contracted_step = std::numeric_limits<unsigned int>::max();

  // The step the job was started on
  unsigned int started_step = std::numeric_limits<unsigned int>::max();

  // The step the job was completed on
  unsigned int completed_step = std::numeric_limits<unsigned int>::max();

  static std::string csv_headers() {
    auto               vals = {"ID",         "created_step",    "deadline_step",
                               "job_vector", "claimed_step", "completed_step"};
    std::ostringstream ss;
    for (const auto& val : vals) { ss << val << ","; }
    return ss.str();
  }

  std::string dump_csv() const {
    auto               vals  = {ID, created_step, deadline_step};
    auto               vals2 = {claimed_step, completed_step};
    std::ostringstream ss;
    for (const auto& val : vals) { ss << val << ","; }
    for (const auto& val : job_vector) { ss << val << " "; }
    ss << ",";
    for (const auto& val : vals2) { ss << val << ","; }
    return ss.str();
  }

  // THE BELOW METHODS NEED TO BE IMPLEMENTED BY THE USER ------------------------------------------

  /**
   * Calculates the cost of a job (given an ability vector).
   *
   * @param ability The ability vector of an agent
   *
   * @return The cost of doing the job (for that agent)
   */
  unsigned int cost(const std::array<unsigned int, DIM>& ability) const;

  /**
   * Calculates how long it will take for an agent to do a job
   *
   * @param ability The ability vector of an agent
   *
   * @return The number of steps the job will occupy the agent for
   */
  unsigned int duration(const std::array<unsigned int, DIM>& ability) const;

  /**
   * Calculates the reward of a job (given an ability vector).
   *
   * @param ability The ability vector of an agent
   *
   * @return The cost of doing the job (for that agent)
   */
  unsigned int reward(const std::array<unsigned int, DIM>& ability) const;
};
