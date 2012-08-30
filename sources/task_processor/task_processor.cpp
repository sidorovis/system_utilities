#include "task_processor.h"

namespace task_processor_compilation_checker
{
    class task
    {
    public:
        void operator()()
        {
        }
    };

	void compilation_checker()
	{
		using namespace system_utilities::common;
		task_processor< task > ts( 4 );
	}
}