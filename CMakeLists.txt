cmake_minimum_required(VERSION 3.16)
project(MyYolo VERSION 0.1 LANGUAGES CXX)

add_subdirectory(myYolo_qt)
add_subdirectory(Ttentacle)

add_subdirectory(vendor/spdlog-1.15.0)

add_library(httplib INTERFACE)

target_include_directories(httplib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/cpp-httplib-0.36.0
)
