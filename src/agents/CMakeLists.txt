add_library(scs_agents
    controller.cpp
)

target_link_libraries(scs_agents scs_controllers)

add_subdirectory(controllers)
