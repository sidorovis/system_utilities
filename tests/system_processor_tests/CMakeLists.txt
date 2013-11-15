
find_package( Boost 1.54 ${SEARCH_PARAMETERS} COMPONENTS
	filesystem
	thread
	regex
	date_time
	system
	program_options
	unit_test_framework
	chrono
)
compile_project( ${tests_name} "*.cpp" "*.h" BINARY tests ${module_name} boost_dynamic_test_helper task_processor file_logger logger queue_logger ts_queue property_reader time_tracker Boost )
register_test( ${tests_name} 1.0 1.0 )
