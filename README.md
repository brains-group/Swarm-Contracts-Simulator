# Swarm-Contracts-Simulator

A **C++23 project** built with **CMake**, demonstrating a **visual simulation of Swarm Contracts** — a blockchain-inspired protocol for decentralized task distribution among autonomous agents.

Located at: https://github.com/brains-group/Swarm-Contracts-Simulator 

---

## Overview

The **Swarm Contracts** protocol enables **trustless cooperation** among multiple independent agents.  
It defines three core participant types:

| Role | Description |
|------|--------------|
| **Client** | Places orders for work to be completed |
| **Worker** | Accepts and fulfills client orders |
| **Adjudicator** | Judges whether work is completed successfully *(not yet implemented)* |

This simulator provides a **visual demonstration** of this concept in the context of **custom part manufacturing** — a setting where small-batch, high-variation production makes decentralized coordination especially useful.

---

## Motivation: Custom Manufacturing

Conventional manufacturing methods struggle with:
- Small batch sizes  
- High customization requirements  
- Inflexible centralized production systems  

**Swarm Contracts** offer an alternative:  
> Independent robotic agents (“workers”) can bid on, collaborate over, and complete manufacturing tasks posted by clients, without centralized control or trust assumptions.

This simulation models:
- **Clients** placing *Custom Part* orders  
- **Workers** assembling parts from *Materials*  
- **Materials** visualized as colored squares that can be either:
  - **Tiled** (additively combined)
  - **Mixed** (e.g., `Red + Green → Yellow`, using RGB as the primary colors)

## Building & Running

### Requirements
- C++23
- CMake >= 3.20
- A C++ compiler (GCC, Clang, and possibly MSVC)
- SFML (if used for visualization)

Tested on linux, but CMake should be cross platform

### Build Instructions

```bash
# Configure and build
cmake -B build
cmake --build build

# Run the simulator
./build/bin/scs_visualizer
```

You can adjust build options or add extra visualization flags in the CMake configuration.

---

## Visualization

The simulator displays:
- **Clients** issuing part requests
- **Workers** generating and combining materials
- **Resulting parts** forming through mixing or tiling operations

Future updates will introduce:
- Adjudicator nodes
- Blockchain transaction visualization
- Multi-agent coordination strategies
- Configurable simulation scenarios

---

## License

This project is released under the [MIT License](LICENSE).

