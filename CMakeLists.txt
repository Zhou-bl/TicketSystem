cmake_minimum_required(VERSION 3.16)
project(NewTicketSystem)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/bpt)

add_executable(NewTicketSystem
        src/test/compare.cpp)
