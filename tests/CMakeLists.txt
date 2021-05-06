cmake_minimum_required(VERSION 3.1.0)

set(TESTS_EXE_NAME "tests")

# GoogleTest: START: https://github.com/google/googletest/tree/master/googletest
# Add googletest directly to our build. This defines the gtest and gtest_main targets.
add_subdirectory(./googletest)

# The gtest/gtest_main targets carry header search path dependencies automatically when using CMake 2.8.11 or later. Otherwise we have to add them here ourselves.
include_directories("${gtest_SOURCE_DIR}/include")
# GoogleTest: END.

include_directories(../src)

file(GLOB TESTS_FILES_CPP "./test-unit-*.cpp")

add_executable(${TESTS_EXE_NAME} ${TESTS_FILES_CPP})
target_compile_features(${TESTS_EXE_NAME} PRIVATE ${PROJECT_CPP_STANDART})
target_link_libraries(${TESTS_EXE_NAME} ${LIB_NAME})
target_link_libraries(${TESTS_EXE_NAME} gtest_main)
