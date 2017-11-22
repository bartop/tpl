cmake_minimum_required(VERSION 2.8.10)

project(tpl)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_COLOR_MAKEFILE ON)

set(TEST_SOURCES
    tests/filtered_test.cpp
    tests/transformed_test.cpp
    tests/sorted_test.cpp
    tests/any_test.cpp
    tests/all_test.cpp
)

set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/External/Catch)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

include_directories(${PROJECT_NAME} INTERFACE "${CMAKE_SOURCE_DIR}/tpl")
include_directories(${CATCH_INCLUDE_DIR})

add_library(${PROJECT_NAME} INTERFACE)

enable_testing()
foreach( testsourcefile ${TEST_SOURCES} )
    string( REPLACE "tests/" "" testname ${testsourcefile} )
    string( REPLACE ".cpp" "" testname ${testname} )
    add_executable( ${testname} ${testsourcefile} )
    target_link_libraries(${testname} tpl Catch)
	add_test( ${testname} ${testname})
endforeach( testsourcefile ${TEST_SOURCES} )
