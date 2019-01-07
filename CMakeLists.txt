cmake_minimum_required(VERSION 3.1.3)

project(tpl)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Weffc++ -Wnull-dereference -Wold-style-cast -Wshadow -pedantic-errors")
endif()

if ( CMAKE_COMPILER_IS_GNUCC )
	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "6" OR CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "6")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnull-dereference -Wduplicated-cond")
	endif()
	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "7" OR CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "7")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wduplicated-branches" )
	endif()

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Weffc++ -Wlogical-op -Wold-style-cast -Wuseless-cast -Wshadow -pedantic-errors")
endif ( CMAKE_COMPILER_IS_GNUCC )
if ( MSVC )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif ( MSVC )

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_COLOR_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 14)

set(TEST_SOURCES
    tests/zipped_test.cpp
    tests/filtered_test.cpp
    tests/flattened_test.cpp
    tests/transformed_test.cpp
    tests/sorted_test.cpp
    tests/any_test.cpp
    tests/all_test.cpp
    tests/count_test.cpp
    tests/is_enumerable_test.cpp
    tests/is_factory_test.cpp
    tests/is_associative_test.cpp
    tests/enumerable_traits_test.cpp
    tests/keys_test.cpp
    tests/mapped_values_test.cpp
    tests/copy_to_test.cpp
    tests/infinite_test.cpp
    tests/generator_test.cpp
    tests/fold_left_test.cpp
    tests/cycle_test.cpp
    tests/take_test.cpp
    tests/drop_test.cpp
    tests/composite_factory_test.cpp
    tests/cache_test.cpp
    tests/no_operation_test.cpp
)

set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/External/Catch)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

include_directories(${PROJECT_NAME} INTERFACE "${CMAKE_SOURCE_DIR}/include")
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
