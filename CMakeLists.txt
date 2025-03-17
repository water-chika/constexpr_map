cmake_minimum_required(VERSION 3.20)

project(cpp_project)

add_executable(test
    main.cpp)

target_compile_features(test PUBLIC cxx_std_20)