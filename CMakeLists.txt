cmake_minimum_required(VERSION 3.16)
project(rpi_sbus)
add_subdirectory( Utilities)
add_library(sbuslib STATIC
        src/SBUS.cpp src/sbus_driver.c)
target_link_libraries( sbuslib utilities )
target_include_directories(sbuslib PUBLIC  include)

add_executable(blocking_receiver examples/blocking_receiver.cpp)
target_link_libraries(blocking_receiver PUBLIC sbuslib)
