find_package( Boost 1.54 ${SEARCH_PARAMETERS} COMPONENTS
	filesystem
	thread
	date_time
	system
	unit_test_framework
	chrono
)
compile_project( windows_service_tests_helper "*.cpp" "*.h" BINARY tests windows_service Boost )
register_test( windows_service_tests_helper 1.0 1.0 )
