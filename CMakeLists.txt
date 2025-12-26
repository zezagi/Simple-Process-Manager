cmake_minimum_required(VERSION 4.0)
project(ekwiwalent_cheatEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(ekwiwalent_cheatEngine main.cpp
        processExplorer_helpers.h
        ProcessExplorer/ProcessExplorer_helpers.cpp
        ProcessExplorer/console_helpers.cpp
        console_helpers.h)
