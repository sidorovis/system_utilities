cmake_minimum_required(VERSION 2.8)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set( CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE} CACHE STRING "Configurations" FORCE )
project( ${SOLUTION_NAME} )
include( environment.cmake required )


#if(WIN32)
#	modules( time_tracker ts_queue property_reader task_processor logger ts_logger queue_logger file_logger timer system_processor windows_service )
#elseif(UNIX)
#	modules( file_logger system_processor )
#endif(WIN32)

modules( time_tracker timer logger ts_logger ts_queue property_reader task_processor queue_logger file_logger system_processor )

