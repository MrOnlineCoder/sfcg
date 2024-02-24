cmake_minimum_required(VERSION 3.16)
project(sfcg_example LANGUAGES CXX)

add_executable(sfcg_example main.cpp)

target_link_libraries(sfcg_example PRIVATE sfcg sfml-window sfml-system sfml-graphics)

target_compile_features(sfcg_example PRIVATE cxx_std_17)

target_include_directories(sfcg_example PRIVATE ${CMAKE_SOURCE_DIR}/include)