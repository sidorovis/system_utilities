#include "logger.h"


namespace system_utilities
{
    namespace common
    {
		template<>
		void logger< false >::write( const details::message_level::value , const std::string& )
		{
		}
		template<>
		void logger< false, false >::write( const details::message_level::value , const std::string& )
		{
		}
		template<>
		void logger< false, true, false >::write( const details::message_level::value, const std::string& )
		{
		}
		template<>
		void logger< false, false, false >::write( const details::message_level::value, const std::string& )
		{
		}

		template<>
		void logger< true, false, false >::write( const details::message_level::value, const std::string& message )
		{
			stream_ << message << "\n";
		}

		template<>
		void logger< true, true, false >::write( const details::message_level::value, const std::string& message )
		{
			stream_ << message << "\n";
			flush();
		}

		template<>
		void logger< true, false, true >::write( const details::message_level::value value, const std::string& message )
		{
			using namespace boost::posix_time;
			const std::string current_time = to_simple_string( microsec_clock::universal_time() );
			static const char open_quote = '[';
			stream_ << open_quote << current_time << message_levels[ value ] << message << "\n";
		}

		template<>
		void logger< true, true, true >::write( const details::message_level::value value, const std::string& message )
		{
			using namespace boost::posix_time;
			const std::string current_time = to_simple_string( microsec_clock::universal_time() );
			static const char open_quote = '[';
			stream_ << open_quote << current_time << message_levels[ value ] << message << "\n";
			flush();
		}

    }
}


