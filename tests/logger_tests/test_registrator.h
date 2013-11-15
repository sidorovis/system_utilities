#include <boost/test/test_tools.hpp>
#include <dynamic_link_helper.h>

namespace system_utilities
{
	namespace tests_
	{
		namespace common
		{
			void logger_constructor_tests();
			void logger_write_tests();
			void logger_flush_tests();
			void logger_note_tests();
			void logger_warn_tests();
			void logger_error_tests();
			void logger_debug_tests();
			void logger_fatal_tests();
			//
			void logger_write_performance_tests();
		}
	}
}
