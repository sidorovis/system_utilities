find_package( Boost 1.41 ${SEARCH_PARAMETERS} COMPONENTS
	filesystem
	thread
	regex
	date_time
	system
	chrono
)

compile_project( ${module_name} "*.cpp" "*.h" STATIC libraries ts_queue Boost )

