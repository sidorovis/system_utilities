#include "limited_file_logger.h"

namespace system_utilities
{
    namespace common
	{
		namespace details
		{
			template<>
			size_t message_size_counter< false >::message_size(const system_utilities::common::details::message_level::value value, const std::string &message)
			{
				return 0ul;
			}
            template<>
			size_t message_size_counter< false, false >::message_size( const details::message_level::value value, const std::string& message )
			{
				return 0ul;
			}
			template<>
			size_t message_size_counter< false, true, false >::message_size( const details::message_level::value value, const std::string& message )
			{
				return 0ul;
			}
			template<>
			size_t message_size_counter< false, false, false >::message_size( const details::message_level::value value, const std::string& message )
			{
				return 0ul;
			}
			template<>
			size_t message_size_counter< true, false, false >::message_size( const details::message_level::value value, const std::string& message )
			{
				return message.size() + 1;
			}
			template<>
			size_t message_size_counter< true, true, false >::message_size( const details::message_level::value value, const std::string& message )
			{
				return message.size() + 1;
			}
			template<>
			size_t message_size_counter< true, false, true >::message_size( const details::message_level::value value, const std::string& message )
			{
				using namespace boost::posix_time;
				static const size_t current_time_size = to_simple_string( microsec_clock::universal_time() ).size();
				return 1 + current_time_size + logger<>::message_levels[ value ].size() + message.size() + 2;
			}
			template<>
			size_t message_size_counter< true, true, true >::message_size( const details::message_level::value value, const std::string& message )
			{
				using namespace boost::posix_time;
				static const size_t current_time_size = to_simple_string( microsec_clock::universal_time() ).size();
				return 1 + current_time_size + logger<>::message_levels[ value ].size() + message.size() + 2;
			}
		}
	}
}
