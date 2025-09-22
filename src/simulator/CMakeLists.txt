add_library(simulator
    simulator.cpp
)

target_include_directories(simulator PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)

target_link_libraries(simulator PRIVATE
    agents
)
