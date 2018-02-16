CMAKE_MINIMUM_REQUIRED( VERSION 3.2)
project(AppImageUpdater)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

include_directories(. AppImageUpdaterBridge)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# include subdirectories in the right order
add_subdirectory(AppImageUpdaterBridge)
add_subdirectory(src)

