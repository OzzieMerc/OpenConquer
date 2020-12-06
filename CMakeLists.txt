####################################################################
# File: CMakeLists.txt
# Author: Ozzie Mercado
# Created: December 5, 2020
# Description: Generates the Visual Studio project on Windows.
####################################################################

cmake_minimum_required(VERSION 3.6)

if (WIN32)
	project(OpenConquer)

	# Recursively create a list of .h/.cpp files in the project folder.
	file(
		GLOB_RECURSE ProjectFiles
		./Project/*.h
		./Project/*.cpp
	)

	# Use "CMakePredefinedTargets" folder for ALL_BUILD and ZERO_CHECK.
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	# Set up the project on Windows.
    add_executable(OpenConquer ${ProjectFiles})

	# Set the startup project.
	set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT OpenConquer)

	# Preserve the folder structure.
	source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${ProjectFiles})
else()
    message("ERROR: This project supports Windows only.\n")
endif()
