cmake_minimum_required(VERSION 3.16)
project(MyYolo VERSION 0.1 LANGUAGES CXX)

add_subdirectory(myYolo_qt)
add_subdirectory(Ttentacle)

add_subdirectory(vendor/spdlog-1.15.0)
