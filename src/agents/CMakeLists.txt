add_library(agents
    agentmanager.cpp
    agentsiminterface.cpp
)

target_include_directories(agents PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)

