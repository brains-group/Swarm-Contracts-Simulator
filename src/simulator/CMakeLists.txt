add_library(scs_simulator
    simulator.cpp
)

if(BUILD_TESTING)
    add_subdirectory(unittest)
endif()
