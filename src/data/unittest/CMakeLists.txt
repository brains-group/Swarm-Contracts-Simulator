add_executable(test_data
    test_angle.cpp
    test_point.cpp
)

target_link_libraries(test_data
    scs_data
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_data)
