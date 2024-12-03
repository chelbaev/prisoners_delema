cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(bin_array_project VERSION 0.2.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_subdirectory(lib)

target_include_directories(binArray PUBLIC .) 

add_subdirectory(tests)

add_executable(BA_main main.cpp) 

target_link_libraries(BA_main binArray)
