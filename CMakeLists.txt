CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
project(AppImageUpdater)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(QAPPLICATION_CLASS QApplication CACHE STRING "Inheritance class for SingleApplication")

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(LibtorrentRasterbar)
find_package(QAppImageUpdate)
find_package(libappimage)
find_package(Qt5Gui)
find_package(Qt5Concurrent)
find_package(Qt5Qml)
find_package(Qt5QuickControls2)

add_subdirectory(third-party/SingleApplication)

# Include Directories.
include_directories(${CMAKE_BINARY_DIR})
include_directories(third-party/SingleApplication)

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

add_executable(AppImageUpdater main.cc
			     global.hpp
			     Version.hpp
			     Updater_p.hpp
			     Updater_p.cc
			     Updater.hpp
			     Updater.cc
			     Seeder_p.hpp
			     Seeder_p.cc
			     Seeder.hpp
			     Seeder.cc
			     AppImageImageProvider.hpp
			     AppImageImageProvider.cc
			     DropItemParser.hpp
			     DropItemParser.cc
	                     BuildConstants.hpp
			     BuildConstants.cc
			     SettingsManager.hpp
			     SettingsManager.cc
			     SystemTray.hpp
			     SystemTray.cc
			     Executer_p.hpp
			     Executer_p.cc
			     Executer.hpp
			     Executer.cc
			     Helpers.hpp
			     Helpers.cc
			     app_resources/resources.qrc
			     qml/qml.qrc)
target_link_libraries(AppImageUpdater PUBLIC 
				      Qt5::Concurrent 
				      Qt5::Gui 
				      Qt5::Qml 
				      Qt5::QuickControls2
				      util)
target_link_libraries(AppImageUpdater PRIVATE QAppImageUpdate)
target_link_libraries(AppImageUpdater PRIVATE libappimage)
target_link_libraries(AppImageUpdater PRIVATE LibtorrentRasterbar::torrent-rasterbar)
target_link_libraries(AppImageUpdater PRIVATE SingleApplication::SingleApplication)
