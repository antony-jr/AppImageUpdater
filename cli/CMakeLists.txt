CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
project(appimageupdater)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(LibtorrentRasterbar)
find_package(QAppImageUpdate)

# Include Directories.
include_directories(${CMAKE_BINARY_DIR})

if(APPIMAGE_UPDATER_VERSION)
	add_definitions(-DAPPIMAGE_UPDATER_VERSION="${APPIMAGE_UPDATER_VERSION}")
endif()
if(APPIMAGE_UPDATER_COMMIT)
	add_definitions(-DAPPIMAGE_UPDATER_COMMIT="${APPIMAGE_UPDATER_COMMIT}")
endif()
if(APPIMAGE_UPDATER_BUILD_NO)
	add_definitions(-DAPPIMAGE_UPDATER_BUILD_NO="${APPIMAGE_UPDATER_BUILD_NO}")
endif()
if(APPIMAGE_UPDATER_BUILD_TIME)
	add_definitions(-DAPPIMAGE_UPDATER_BUILD_TIME="${APPIMAGE_UPDATER_BUILD_TIME}")
endif()

add_executable(appimageupdater main.cc cutelog.c cutelog.h)
target_link_libraries(appimageupdater PUBLIC Qt5::Core)
target_link_libraries(appimageupdater PRIVATE QAppImageUpdate)
target_link_libraries(appimageupdater PRIVATE LibtorrentRasterbar::torrent-rasterbar)
