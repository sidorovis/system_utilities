add_subdirectory( ../windows_service_tests_helper )
find_package( Boost 1.41 ${SEARCH_PARAMETERS} COMPONENTS
	filesystem
	thread
	date_time
	system
	unit_test_framework
)
compile_project( ${tests_name} "*.cpp" "*.h" BINARY tests ${module_name} Boost )
register_test( ${tests_name} 1.0 1.0 )
