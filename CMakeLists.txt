cmake_minimum_required(VERSION 3.28)

<<<<<<< HEAD
project(unique-key VERSION 0.3 LANGUAGES CXX)
=======
project(unique-key VERSION 0.2 LANGUAGES CXX)
>>>>>>> 699a368ae4828e430bed1331a825479b04286b2e

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
