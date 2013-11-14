
find_package( Boost 1.54 ${SEARCH_PARAMETERS} COMPONENTS
	filesystem
	thread
	regex
	date_time
	system
	unit_test_framework
	chrono
)
compile_project( ${tests_name} "*.cpp" "*.h" BINARY tests ${module_name} file_logger logger queue_logger task_processor ts_logger ts_queue time_tracker Boost )
register_test( ${tests_name} 1.0 1.5 )
