add_library(scs_data
    angle.cpp
    material.cpp
    point.cpp
)

if(BUILD_TESTING)
    add_subdirectory(unittest)
endif()
