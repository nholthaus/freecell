cmake_minimum_required(VERSION 3.21)

# Project name and version
project(freecell VERSION 1.0 LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required Qt modules
set(CMAKE_PREFIX_PATH $ENV{Qt6_DIR})
find_package(Qt6 REQUIRED COMPONENTS Widgets Core)
qt_standard_project_setup()

# Add the executable
add_executable(${PROJECT_NAME}
               main.cpp
               card.cpp
               deck.cpp
               board.cpp
               cardwidget.cpp
               cardproxy.cpp
               cardspot.cpp
               freecell.cpp
               abstractcardholder.cpp
               columnspot.cpp
               cardspotproxy.cpp
               acespot.cpp
               mainwindow.cpp
               boardscene.cpp
               )

# Add header files (optional, but helps IDEs like CLion)
target_sources(${PROJECT_NAME} PRIVATE
               card.h
               deck.h
               board.h
               cardwidget.h
               cardproxy.h
               cardspot.h
               freecell.h
               abstractcardholder.h
               columnspot.h
               cardspotproxy.h
               acespot.h
               mainwindow.h
               boardscene.h
               )

# Add Qt resource file
qt6_add_resources(RESOURCE_FILES resources/resources.qrc)
target_sources(${PROJECT_NAME} PRIVATE ${RESOURCE_FILES})

# Link necessary Qt libraries to the executable
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Core)

# Enable console output (optional)
if(WIN32)
	set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE OFF)
endif()

# Install the required DLLs to run
include(InstallRequiredSystemLibraries)
if (WIN32)
	include(windeployqt)
	windeployqt(${PROJECT_NAME})
	qt_generate_deploy_app_script(
			TARGET ${PROJECT_NAME}
			OUTPUT_SCRIPT deploy_script
			NO_UNSUPPORTED_PLATFORM_ERROR
			)
	install(SCRIPT ${deploy_script})
endif (WIN32)