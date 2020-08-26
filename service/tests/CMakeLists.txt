#enable_testing()

find_package(GTest CONFIG REQUIRED)
add_executable(unit_tests main.cpp)
target_link_libraries(unit_tests PRIVATE GTest::gtest GTest::gmock)

#add_test(AllTestsInMain main)
