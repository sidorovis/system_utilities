
find_package( Boost 1.41 ${SEARCH_PARAMETERS} COMPONENTS
	filesystem
	thread
	regex
	date_time
	system
	unit_test_framework
)
compile_project( ${tests_name} "*.cpp" "*.h" BINARY tests ${module_name} time_tracker Boost )
register_test( ${tests_name} 14.3 27.3 )
