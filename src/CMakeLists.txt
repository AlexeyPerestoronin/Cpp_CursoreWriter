cmake_minimum_required(VERSION 3.1.0)

include_directories(./)

file(GLOB HPP_FILES "./*.hpp")
file(GLOB CPP_FILES "./*.cpp")

add_library(${LIB_NAME} STATIC ${CPP_FILES} ${HPP_FILES})
target_compile_features(${LIB_NAME} PRIVATE ${PROJECT_CPP_STANDART})
