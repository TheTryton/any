cmake_minimum_required(VERSION 3.8)
project(any_performance_test)

find_package( Qt5 COMPONENTS Widgets Gui Core Charts REQUIRED )

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(any_performance_test_Sources
	Source/main.cpp

    Source/PerformanceTester/PerformanceTester.h
    Source/PerformanceTester/PerformanceTester.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${any_performance_test_Sources})

add_executable(any_performance_test ${any_performance_test_Sources})

target_link_libraries(any_performance_test Qt5::Widgets Qt5::Gui Qt5::Core Qt5::Charts any)