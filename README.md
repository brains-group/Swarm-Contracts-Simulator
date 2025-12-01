# Swarm-Contracts-Simulator

A **C++23 project** built with **CMake**, demonstrating a **visual simulation of Swarm Contracts**:
a blockchain-inspired protocol for decentralized task distribution among autonomous agents.

Located at: https://github.com/brains-group/Swarm-Contracts-Simulator 

---

## Swacm Contracts

### Overview

The **Swarm Contracts** protocol enables **trustless cooperation** among multiple independent agents.
It is optimal for scenarios when:

1. Independent agents identify jobs that have a cost to complete and give a reward when completed
2. Agents have varying costs & rewards associated with each job
3. It is difficult or infeasible to automatically verify whether a job was completed

It defines three core participant types:

| Role | Description |
|------|--------------|
| **Client** | Places orders for work to be completed |
| **Worker** | Accepts and fulfills client orders |
| **Adjudicator** | Judges whether work is completed successfully |

### Simulation

Jobs are modeled by N-dimensional "Difficulty Vectors". Each dimension of the vector represents a
dimension of difficulty of the abstract job. For example in the case of moving packages, this could map to
the weight of the package, the distance it must be moved, and how fragile it is.

Clients are external to the simulator, the focus is on the workers. So, contracts are created by the simulation
to be claimed by workers.

Every agent in the simulation has an "Ability Vector". Agents work to complete tasks by subtracting
their ability vector from the difficulty vector of a job. The cost for a worker to work on a task is a baseline of
1 for each item of the vector it is "working on".

---

## Building & Running

### Requirements
- CMake >= 3.20
- A C++ compiler (GCC, Clang, and possibly MSVC)

Tested on linux, but CMake should be cross platform

### Build Instructions

```bash
# Configure and build
cmake -S . -B build
cmake --build build

# Run the simulator
./build/bin/simulator
```

---

## License

This project is released under the [MIT License](LICENSE).

