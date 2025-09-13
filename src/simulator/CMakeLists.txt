add_library(simulator
    config.cpp
    simulator.cpp
)

target_include_directories(simulator PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
