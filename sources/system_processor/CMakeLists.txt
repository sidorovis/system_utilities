find_package( Boost 1.54 ${SEARCH_PARAMETERS} COMPONENTS
	filesystem
	thread
	regex
	date_time
	system
	program_options
)

compile_project( ${module_name} "*.cpp" "*.h" STATIC libraries Boost task_processor file_logger logger queue_logger ts_queue property_reader time_tracker )

