cmake_minimum_required(VERSION 3.20)

project(cpp_project)

add_library(constexpr_map
    constexpr_map.hpp
    constexpr_map.cpp)

target_include_directories(constexpr_map PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(constexpr_map PUBLIC cxx_std_20)

add_executable(constexpr_map_test
    main.cpp)

target_compile_features(constexpr_map_test PUBLIC cxx_std_20)
target_link_libraries(constexpr_map_test PUBLIC constexpr_map)

