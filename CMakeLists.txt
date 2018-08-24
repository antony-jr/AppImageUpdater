CMAKE_MINIMUM_REQUIRED( VERSION 3.2)
project(AppImageUpdater)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Find the QtCore library
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)
include_directories(${CMAKE_CURRENT_BINARY_DIR}) # just in case!
# Add include directory
include_directories(../AppImageUpdaterBridge)

add_executable(
	AppImageUpdater
	main.cpp
	AppImageUpdater.cpp
	AppImageUpdater.hpp
	../AppImageUpdaterResources.qrc
)

target_link_libraries(AppImageUpdater AIUpdaterBridge Qt5::Widgets Qt5::Network Qt5::Core)
