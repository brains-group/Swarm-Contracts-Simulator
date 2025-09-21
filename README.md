# Swarm-Contracts-Simulator

Simulator for Swarm Contracts research project

## How to build & run

Should be very simple. Run the follwoing in the top level directory:

```sh
cmake -S . -B build
cmake --build build
./build/bin/visualizer
```

Once the project grows, more information will be needed, but this is sufficient
for now.

Some notes:
- The required CMake version is just set to what I currently have, it can
  probably be lowered safely to whatever you have installed, I am not doing
  anything too complex. Change this by setting `cmake_minimum_required` in the
  top level CMakeLists.txt to the version you have, and see if it works.
