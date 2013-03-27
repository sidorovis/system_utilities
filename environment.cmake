# --------------------------------------------------------------------------------
# --------------------------------------------------------------------------------
# environment.cmake script, create for defining environment variables
# --------------------------------------------------------------------------------
# should we show debug information
# should we create configuration for tests
# --------------------------------------------------------------------------------

option( VERBOSE "should we say as much messages as possible" ON )
option( BUILD_TESTS "Should we build tests for modules" ON )
if (BUILD_TESTS AND VERBOSE)
	message(STATUS " -T: Test will be builded")
endif(BUILD_TESTS AND VERBOSE)

# --------------------------------------------------------------------------------
#  * build type: CMAKE_BUILD_TYPE (Debug, Release, ReleaseWithDebugInfo)
#   Release, Debug variable defining
# --------------------------------------------------------------------------------

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/_cmake_scripts")
include( utils )

test_variable_on_existance( SOLUTION_NAME )
test_variable_on_existance( CMAKE_BUILD_TYPE )
test_variable_on_equal_to_one_of_the_list( CMAKE_BUILD_TYPE Release Debug ReleaseWithDebugInfo )

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	option( Debug "user choose debug" ON )
	option( Release "user choose debug" OFF )
	if (VERBOSE)
		message(STATUS " -T: Debug configuration choosed")
	endif(VERBOSE)
else()
	option( Debug "user choose debug" OFF )
	option( Release "user choose debug" ON )
	if (VERBOSE)
		message(STATUS " -T: Release (" ${CMAKE_BUILD_TYPE} ") configuration choosed")
	endif(VERBOSE)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")

# --------------------------------------------------------------------------------
# should we run performance tests
# --------------------------------------------------------------------------------

if (Release)
	option( RUN_PERFORMANCE_TESTS "should we run performance tests for projects" ON )
	if (VERBOSE)
		message(STATUS " -T: Performance tests going to start")
	endif(VERBOSE)
else()
	option( RUN_PERFORMANCE_TESTS "should we run performance tests for projects" OFF )
	if (VERBOSE)
		message(STATUS " -T: Performance tests NOT going to start")
	endif(VERBOSE)
endif()

# --------------------------------------------------------------------------------
# * address model: CMAKE_ADDRESS_MODEL (x64, x32)
# --------------------------------------------------------------------------------

if(WIN32)
    if(CMAKE_CL_64)
        set(CMAKE_ADDRESS_MODEL 64)   
    else(CMAKE_CL_64)
        set(CMAKE_ADDRESS_MODEL 32)
    endif(CMAKE_CL_64)
elseif(UNIX)
    ADD_DEFINITIONS (-D_LINUX -pthread -D_XOPEN_SOURCE=600)

    if(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        set(CMAKE_ADDRESS_MODEL 64) 
    elseif(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "i686")
        set(CMAKE_ADDRESS_MODEL 32)
    elseif(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "i386")
        set(CMAKE_ADDRESS_MODEL 32)
    else()
        message( FATAL_ERROR "Unknown address model type" )
    endif(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86_64")
else()
    message( FATAL_ERROR "Unknown address model type" )
endif(WIN32)

if(${VERBOSE})
	message(STATUS " -T: Address model: ${CMAKE_ADDRESS_MODEL}")
endif(${VERBOSE})

# --------------------------------------------------------------------------------
# add compilation settings ( optimization level, enable_testing, warning disabling, output folder)
# --------------------------------------------------------------------------------

if (BUILD_TESTS)
	enable_testing()
endif (BUILD_TESTS)

if ( UNIX )
	if (Debug)
		add_definitions( " -O0 -g -Wall" )
	else()
		add_definitions( " -O3 -Wall -Werror" )
	endif()

	set(output_path ${PROJECT_BINARY_DIR}/bin_${CMAKE_ADDRESS_MODEL}/${CMAKE_BUILD_TYPE})
elseif( WIN32 )
	add_definitions( -D_CRT_SECURE_NO_WARNINGS )
	add_definitions( -D_WIN32_WINNT=0x0501 )
	SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHa") 
	set(output_path ${PROJECT_BINARY_DIR}/bin_${CMAKE_ADDRESS_MODEL}/)
endif( UNIX )

set( SOLUTION_BINARY_DIR ${PROJECT_BINARY_DIR} )
set( EXECUTABLE_OUTPUT_PATH ${output_path} )
set( LIBRARY_OUTPUT_PATH ${output_path} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${output_path} )
set( BINARIES_DIRECTORY ${PROJECT_BINARY_DIR}/bin_${CMAKE_ADDRESS_MODEL}/${CMAKE_BUILD_TYPE} )

# --------------------------------------------------------------------------------
# search for boost
# --------------------------------------------------------------------------------

set( Boost_USE_STATIC_LIBS ON )
set( Boost_USE_MULTITHREADED ON )
set( BOOST_LIBRARYDIR "$ENV{BOOST_ROOT}/stage_${CMAKE_ADDRESS_MODEL}/lib" )

add_definitions( -DBOOST_FILESYSTEM_VERSION=2 )
if (${VERBOSE})
	message( STATUS " -T: Using Boost filesystem version: 2" )
endif(${VERBOSE})

# --------------------------------------------------------------------------
# use_folders, saving configuration to the build
# --------------------------------------------------------------------------

if (${VERBOSE})
	set( SEARCH_PARAMETERS REQUIRED QUIET )
else()
	set( SEARCH_PARAMETERS REQUIRED QUIET )
endif()

# --------------------------------------------------------------------------
# use_folders, saving configuration to the build
# --------------------------------------------------------------------------

if ( ${VERBOSE} )
	message(STATUS "Solution name: ${SOLUTION_NAME}")
endif( ${VERBOSE} )

