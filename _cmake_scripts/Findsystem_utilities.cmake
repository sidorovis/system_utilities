include( utils )

if (system_utilities_ROOT)
	set(system_utilities_root ${system_utilities_ROOT} )
elseif(NOT "$ENV{SYSTEM_UTILITIES_ROOT}" STREQUAL "")
	set(system_utilities_root $ENV{SYSTEM_UTILITIES_ROOT} )
else()
	message(FATAL_ERROR "[ERROR]: No SYSTEM_UTILITIES_ROOT environment variable found")
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
	set(CMAKE_BUILD_TYPE Debug)
	if (!system_utilities_FIND_QUIETLY)
		message(STATUS "CMAKE_BUILD_TYPE was not tuned, so we install there 'Debug'")
	endif()
else()
endif()

if (system_utilities_DEBUG)
	message(STATUS "We going to look into '${system_utilities_root}' (SYSTEM_UTILITIES_ROOT environment variable) folder.")
	create_string_from_list( component_list ${system_utilities_FIND_COMPONENTS} )
	message(STATUS "We are looking for: '" ${component_list} "' components.")
endif(system_utilities_DEBUG)

set(system_utilities_FOUND FALSE)
set(system_utilities_INCLUDE_DIRS "")
set(system_utilities_LIBRARIES "")

if (UNIX)
	set(binary_search_folder ${system_utilities_root}/_build_${CMAKE_BUILD_TYPE}_${CMAKE_ADDRESS_MODEL}/bin_${CMAKE_ADDRESS_MODEL}/${CMAKE_BUILD_TYPE})
else(UNIX)
	set(binary_search_folder ${system_utilities_root}/_build_${CMAKE_BUILD_TYPE}_${CMAKE_ADDRESS_MODEL}/bin_${CMAKE_ADDRESS_MODEL}/${CMAKE_BUILD_TYPE})
endif(UNIX)
if (NOT system_utilities_FIND_QUIETLY)
	message(STATUS "Looking for libraries at " ${binary_search_folder})
endif()

if (system_utilities_DEBUG)
	message(STATUS "Binary search folder: " ${binary_search_folder} )
endif(system_utilities_DEBUG)

set(components multikey_tree timer time_tracker ts_queue property_reader task_processor logger ts_logger queue_logger file_logger limited_file_logger system_processor windows_service shared_name_storage)

foreach(component ${system_utilities_FIND_COMPONENTS})

list_contains(we_should_find_${component} ${component} ${components})
if(we_should_find_${component})

	if (system_utilities_DEBUG)
		message(STATUS "Searching for ${component} library. ")
	endif()
	set(system_utilities_${component}_FOUND FALSE)
	set(system_utilities_${component}_INCLUDE_DIRS NOTFOUND)
	set(system_utilities_${component}_LIBRARIES NOTFOUND)

	find_path( system_utilities_${component}_INCLUDE_DIR
			NAMES "${component}.h"
			HINTS "${system_utilities_root}/sources/${component}/")
	set(system_utilities_${component}_INCLUDE_DIRS ${system_utilities_${component}_INCLUDE_DIR})

	find_library( system_utilities_${component}_LIBRARY
		NAMES "${component}" 
		HINTS ${binary_search_folder} )
	set(system_utilities_${component}_LIBRARIES ${system_utilities_${component}_LIBRARY})

	if(NOT system_utilities_FIND_QUIETLY)
		message(STATUS "system_utilities ${component} component: ")
		create_string_from_list(headers ${system_utilities_${component}_INCLUDE_DIRS})
		message(STATUS "   headers  :  " ${headers})
		create_string_from_list(libraries ${system_utilities_${component}_LIBRARIES})
		message(STATUS "   libraries:  " ${libraries})
	endif()	

	if(NOT system_utilities_${component}_INCLUDE_DIR OR NOT system_utilities_${component}_LIBRARY)
		message(FATAL_ERROR " We can't find system_utilities ${component} component")
	else()
		set(system_utilities_INCLUDE_DIRS ${system_utilities_INCLUDE_DIRS} ${system_utilities_${component}_INCLUDE_DIRS})
		set(system_utilities_LIBRARIES ${system_utilities_LIBRARIES} ${system_utilities_${component}_LIBRARIES})
	endif()
else()
	message(FATAL_ERROR "Unknown component: ${component}")
endif(we_should_find_${component})

endforeach( component )


if (system_utilities_DEBUG)
	create_string_from_list(headers ${system_utilities_INCLUDE_DIRS})
	message(STATUS "system_utilities_INCLUDE_DIRS: " ${headers})
	create_string_from_list(libraries ${system_utilities_LIBRARIES})
	message(STATUS "system_utilities_LIBRARIES: " ${libraries})
endif(system_utilities_DEBUG)

