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

		template< bool turn_on = true, bool flush_stream = true, bool print_prefix = true >
		class logger;

		namespace details
		{
			template< bool turn_on = true, bool flush_stream = true, bool print_prefix = true >
			class logger_streamer
			{
				typedef logger< turn_on, flush_stream, print_prefix > defined_logger;
				friend class defined_logger;
				boost::shared_ptr< std::stringstream > stream_;
				const std::string& level_message_;
				defined_logger& defined_logger_;
				mutable bool destroy_;

				explicit logger_streamer( defined_logger& logger, const std::string& level_message )
					: stream_( new std::stringstream )
					, level_message_( level_message )
					, defined_logger_( logger )
					, destroy_( true )
				{
				}

			public:
				logger_streamer( const logger_streamer& ls  )
					: stream_( ls.stream_ )
					, level_message_( ls.level_message_ )
					, defined_logger_( ls.defined_logger_ )
				{
					ls.destroy_ = false;
					destroy_ = true;
				}

				template< class element >
				std::ostream& operator<<( const element& el )
				{
					std::stringstream& stream = (*stream_);
					stream << el;
					return stream;
				}
				~logger_streamer()
				{
					if ( destroy_ )
						defined_logger_.write( level_message_, stream_->str() );
				}
			};
		}
		template< bool turn_on, bool flush_stream, bool print_prefix >
		class logger
		{
		public:
			static const bool turn_on_value = turn_on;
			static const bool flush_stream_value = flush_stream;
			static const bool print_prefix_value = print_prefix;
		protected:
			typedef details::logger_streamer< turn_on, flush_stream, print_prefix > streamer;
			friend class streamer;

			static const std::string note_message_level;
			static const std::string warn_message_level;
			static const std::string error_message_level;
			static const std::string debug_message_level;
			static const std::string fatal_message_level;

			friend void tests_::common::logger_write_tests();

			explicit logger();
			void operator=( const logger< turn_on, flush_stream, print_prefix >& logger_copy );

			std::ostream& stream_;

		private:
			explicit logger( const logger< turn_on, flush_stream, print_prefix >& logger_copy )
				: stream_( logger_copy.stream_ )
			{
			}

		public:
			explicit logger( std::ostream& stream )
				: stream_( stream )
			{
			}
			virtual ~logger()
			{
			}
			inline void flush()
			{
				stream_.flush();
			}
            inline void note( const std::string& message )
			{
			    write( note_message_level, message );
			}
			inline streamer note()
			{
				return streamer( *this, note_message_level );
			}
            inline void warn( const std::string& message )
			{
				write( warn_message_level, message );
			}
			inline streamer warn()
			{
				return streamer( *this, warn_message_level );
			}
            inline void error( const std::string& message )
			{
			    write( error_message_level, message );
			}
			inline streamer error()
			{
				return streamer( *this, error_message_level );
			}
			inline void debug( const std::string& message )
			{
			    write( debug_message_level, message );
			}
			inline streamer debug()
			{
				return streamer( *this, debug_message_level );
			}
            inline void fatal( const std::string& message )
			{
			    write( fatal_message_level, message );
			}
			inline streamer fatal()
			{
				return streamer( *this, fatal_message_level );
			}
		protected:
			virtual void write( const std::string& message_level, const std::string& message )
			{
				using namespace boost::posix_time;
				const std::string current_time = to_simple_string( microsec_clock::universal_time() );
				static const char open_quote = '[';
				stream_ << open_quote << current_time << message_level << message << "\n";
				flush();
			}
		};
		//
		template<>
		void logger< false >::write( const std::string& message_level, const std::string& message )
		{
		}
		template<>
		void logger< false, false >::write( const std::string& message_level, const std::string& message )
		{
		}
		template<>
		void logger< false, true, false >::write( const std::string& message_level, const std::string& message )
		{
		}
		template<>
		void logger< false, false, false >::write( const std::string& message_level, const std::string& message )
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

		//
		template< bool turn_on, bool flush_stream, bool print_prefix >
		const std::string logger<turn_on, flush_stream, print_prefix >::note_message_level = ":NOTE   ]: ";

		template< bool turn_on, bool flush_stream, bool print_prefix >
		const std::string logger<turn_on, flush_stream, print_prefix >::warn_message_level = ":WARNING]: ";

		template< bool turn_on, bool flush_stream, bool print_prefix >
		const std::string logger<turn_on, flush_stream, print_prefix >::error_message_level = ":ERROR  ]: ";
    
		template< bool turn_on, bool flush_stream, bool print_prefix >
		const std::string logger<turn_on, flush_stream, print_prefix >::fatal_message_level = ":FATAL  ]: ";

		template< bool turn_on, bool flush_stream, bool print_prefix >
		const std::string logger<turn_on, flush_stream, print_prefix >::debug_message_level = ":DEBUG  ]: ";
	};
};

#endif // _SYSTEM_UTILITIES_COMMON_LOGGER_H_

