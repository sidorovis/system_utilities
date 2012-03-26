#ifndef _SYSTEM_UTILITIES_COMMON_LOGGER_H_
#define _SYSTEM_UTILITIES_COMMON_LOGGER_H_

#include <ostream>
#include <string>

#include <boost/noncopyable.hpp>
#include <boost/date_time/local_time/local_time.hpp>

namespace system_utilities
{
	namespace tests_
	{
		namespace common
		{
			void logger_write_tests();
		};
	}
    namespace common
    {
		template< bool turn_on = true, bool flush = true, bool print_prefix = true, typename stream = std::ostream >
		class logger
		{
			friend void tests_::common::logger_write_tests();
		protected:
			stream& stream_;

		public:
			explicit logger( stream& stream )
				: stream_( stream )
			{
			}
			inline void flush()
			{
				stream_.flush();
			}
            inline void note( const std::string& message )
			{
				static const std::string message_level = ":NOTE   ]: ";
			    write( message_level, message );
			}
            inline void warn( const std::string& message )
			{
				static const std::string message_level = ":WARNING]: ";
				write( message_level, message );
			}
            inline void error( const std::string& message )
			{
				static const std::string message_level = ":ERROR  ]: ";
			    write( message_level, message );
			}
            inline void debug( const std::string& message )
			{
				static const std::string message_level = ":DEBUG  ]: ";
			    write( message_level, message );
			}
            inline void fatal( const std::string& message )
			{
				static const std::string message_level = ":FATAL  ]: ";
			    write( message_level, message );
			}
		private:
			void write( const std::string& message_level, const std::string& message );
		};

		template<>
		void logger< false >::write( const std::string& message_level, const std::string& message )
		{
		}

		template<>
		void logger< true, false, false >::write( const std::string& message_level, const std::string& message )
		{
			stream_ << message << "\n";
		}

		template<>
		void logger< true, true, false >::write( const std::string& message_level, const std::string& message )
		{
			stream_ << message << "\n";
			flush();
		}

		template<>
		void logger< true, false, true >::write( const std::string& message_level, const std::string& message )
		{
			using namespace boost::posix_time;
			const std::string current_time = to_simple_string( microsec_clock::universal_time() );
			static const char open_quote = '[';
			stream_ << open_quote << current_time << message_level << message << "\n";
		}

		template<>
		void logger< true, true, true >::write( const std::string& message_level, const std::string& message )
		{
			using namespace boost::posix_time;
			const std::string current_time = to_simple_string( microsec_clock::universal_time() );
			static const char open_quote = '[';
			stream_ << open_quote << current_time << message_level << message << "\n";
			flush();
		}
    };
};

#endif // _SYSTEM_UTILITIES_COMMON_LOGGER_H_

