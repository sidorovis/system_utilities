#include "task_processor.h"

namespace
{
	void compilation_checker()
	{
		class task
		{
		public:
			void operator()()
			{
			}
		};
		using namespace system_utilities::common;
		task_processor< task > ts( 4 );
	}
}