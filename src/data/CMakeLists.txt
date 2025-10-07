add_library(scs_data
    point.cpp
)

if(BUILD_TESTING)
    add_subdirectory(unittest)
endif()
