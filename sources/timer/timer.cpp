#include "timer.h"

namespace system_utilities
{
	namespace common
	{
		namespace testing_
		{
			void testing_method(){}

			class testing_class
			{
			public:
				explicit testing_class()
				{
					timer t = create_timer( 600, *this, &testing_class::class_method );
				}
				void class_method()
				{
				}
			};

			void class_instantiation_check()
			{
				timer t = create_timer( 600, &testing_method );
				testing_class tc;
			}
		}
	}
}

