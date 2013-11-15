add_subdirectory( windows_service_tests_helper )

find_package( Boost 1.54 ${SEARCH_PARAMETERS} COMPONENTS
	filesystem
	thread
	date_time
	system
	unit_test_framework
	chrono
)

compile_project( ${tests_name} "*.cpp" "*.h" BINARY tests ${module_name} boost_dynamic_test_helper Boost )
add_dependencies( windows_service_tests windows_service_tests_helper )
register_test( ${tests_name} 2.0 2.0 )
