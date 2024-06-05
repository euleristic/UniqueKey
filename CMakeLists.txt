cmake_minimum_required(VERSION 3.28)

project(unique-key VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_library(unique-key INTERFACE unique-key/unique-key.hpp)

target_include_directories(unique-key INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)

target_sources(unique-key
	PUBLIC
		FILE_SET HEADERS
		FILES
			unique-key/unique-key.hpp
)

include(GNUInstallDirs)
install(TARGETS unique-key 
	FILE_SET HEADERS
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
