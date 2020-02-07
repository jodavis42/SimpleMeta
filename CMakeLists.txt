################################################################################
# Copyright (c) 2020 Josh Davis
################################################################################

################################################################################
# Set required version of cmake and any version specific policies
################################################################################
cmake_minimum_required(VERSION 3.9.1 FATAL_ERROR)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CONFIGURATION_TYPES Debug Release RelWithDebInfo)
project(SimpleMeta CXX)

include(CMake/CommonCompileOptions.cmake)

#paths
set(SupportedConfigs $<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>)

add_subdirectory(SimpleMeta)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SimpleMeta)
