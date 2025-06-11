cmake_minimum_required(VERSION 3.9)
project(rov-sensor-broadcaster)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

add_library(rov_broadcaster_sensor
    selk_rc_msgs/build/cpp/rov_broadcaster_sensor.pb.cc
    selk_rc_msgs/build/cpp/rov_broadcaster_sensor.pb.h
)

target_link_libraries(rov_broadcaster_sensor PUBLIC ${Protobuf_LIBRARIES})
target_include_directories(rov_broadcaster_sensor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(UDP_SOCKET
    UDP_SOCKET.cpp
    UDP_SOCKET.h
)

add_executable(main main.cpp)
target_link_libraries(main PUBLIC UDP_SOCKET)