cmake_minimum_required(VERSION 3.16)
project(opra_backup_receiver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(opra_backup_receiver
    src/main.cpp
    src/receiver.cpp
)

target_include_directories(opra_backup_receiver PRIVATE include)

if (WIN32)
    target_link_libraries(opra_backup_receiver PRIVATE ws2_32)
endif()

