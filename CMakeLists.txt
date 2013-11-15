cmake_minimum_required(VERSION 2.8)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set( CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE} CACHE STRING "Configurations" FORCE )
project( ${SOLUTION_NAME} )
include( environment.cmake required )

modules( boost_dynamic_test_helper multikey_tree time_tracker ts_queue property_reader task_processor logger ts_logger queue_logger file_logger limited_file_logger timer system_processor shared_name_storage )

if(WIN32)
	modules( windows_service )
endif(WIN32)

compile()
