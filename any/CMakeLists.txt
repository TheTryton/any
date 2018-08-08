cmake_minimum_required(VERSION 3.8)
project(any)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(any_Sources
	Include/any.h
	Source/any.cpp

	Include/detail/any_helpers.h
	Include/detail/value_container.h
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${any_Sources}/Include)

add_library(any STATIC ${any_Sources})

target_include_directories(any PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(any PROPERTIES LINKER_LANGUAGE CXX)