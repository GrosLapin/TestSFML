# Copyright © 2016 Lénaïc Bagnères, hnc@singularity.fr

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


cmake_minimum_required(VERSION 2.6)


# General C++ flags
	
	# General
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion -Wsign-conversion -Wlogical-op -Wdouble-promotion -std=c++14 -pedantic")
	
	# Release
	if (CMAKE_BUILD_TYPE STREQUAL "")
		set(CMAKE_BUILD_TYPE "Release")
	endif()
	
	# Release flags
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -march=native -ffast-math")
	
	# Debug flags
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3")


# SFML 2
	
	message(STATUS "---")
	
	find_file(SFML_CMAKE_DIR "cmake/Modules/FindSFML.cmake")
	if (NOT SFML_CMAKE_DIR)
		find_file(SFML_CMAKE_DIR "SFML/cmake/Modules/FindSFML.cmake")
	endif()
	if (NOT SFML_CMAKE_DIR)
		find_file(SFML_CMAKE_DIR "share/SFML/cmake/Modules/FindSFML.cmake")
	endif()
	
	if (SFML_CMAKE_DIR)
		string(REPLACE "FindSFML.cmake" "" SFML_CMAKE_DIR ${SFML_CMAKE_DIR})
		set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${SFML_CMAKE_DIR}")
	endif()
	
	find_package(SFML COMPONENTS system window graphics audio network)
	
	if (SFML_FOUND)
		
		include_directories(${SFML_INCLUDE_DIR})
		link_libraries(${SFML_LIBRARIES})
		
		message(STATUS "Library SFML found =) ${SFML_INCLUDE_DIR} | ${SFML_LIBRARIES}")
		
	else()
		
		message(STATUS "Library SFML not found :(")
		
	endif()


# Project
	message(STATUS "---")
	set(project_INCLUDE "./include")
	include_directories("${project_INCLUDE}")
	message(STATUS "Include project = ${project_INCLUDE}")


# Compiler log
	message(STATUS "---")
	message(STATUS "C++ compiler = ${CMAKE_CXX_COMPILER}")
	message(STATUS "C++ flags    = ${CMAKE_CXX_FLAGS}")
	message(STATUS "Build type   = ${CMAKE_BUILD_TYPE}")

# Sources
	
	file(GLOB_RECURSE project_sources src/*.cpp)

# Executables
	
	message(STATUS "---")
	add_executable(exe "${project_sources}")



# Little help
	message(STATUS "---")
	message(STATUS "You can execute:")
	message(STATUS "    make         # To compile executables and tests")
