add_library(scs_simulator
    simulator.cpp
)

target_link_libraries(scs_simulator
    scs_agents
    scs_data
)

if(BUILD_TESTING)
    add_subdirectory(unittest)
endif()
