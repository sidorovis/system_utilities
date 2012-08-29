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

			namespace message_level
			{
				enum value
				{
					note = 0,
					warn = 1,
					error = 2,
					debug = 3,
					fatal = 4
				};
			}
			template< bool turn_on = true, bool flush_stream = true, bool print_prefix = true >
			class logger_streamer
			{
				typedef system_utilities::common::logger< turn_on, flush_stream, print_prefix > defined_logger;
				friend class system_utilities::common::logger< turn_on, flush_stream, print_prefix >;
                
				boost::shared_ptr< std::stringstream > stream_;
				const message_level::value message_level_ ;
				defined_logger& defined_logger_;
				mutable bool destroy_;

				explicit logger_streamer( defined_logger& logger, const message_level::value message_level )
					: stream_( new std::stringstream )
					, message_level_( message_level )
					, defined_logger_( logger )
					, destroy_( true )
				{
				}

			public:
				logger_streamer( const logger_streamer& ls  )
					: stream_( ls.stream_ )
					, message_level_( ls.message_level_ )
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
						defined_logger_.write( message_level_, stream_->str() );
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
			friend class details::logger_streamer< turn_on, flush_stream, print_prefix >;

			static std::string message_levels[ 5 ];
			static void init_message_levels()
			{
				if ( message_levels[0] == "" )
				{
					message_levels[0] = ":NOTE   ]: ";
					message_levels[1] = ":WARNING]: ";
					message_levels[2] = ":ERROR  ]: ";
					message_levels[3] = ":DEBUG  ]: ";
					message_levels[4] = ":FATAL  ]: ";
				}
			}

			friend void tests_::common::logger_write_tests();

			explicit logger();
			void operator=( const logger< turn_on, flush_stream, print_prefix >& logger_copy );

			std::ostream& stream_;

		private:
			explicit logger( const logger< turn_on, flush_stream, print_prefix >& logger_copy )
				: stream_( logger_copy.stream_ )
			{
				init_message_levels();
			}

		public:
			explicit logger( std::ostream& stream )
				: stream_( stream )
			{
				init_message_levels();
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
			    write( details::message_level::note, message );
			}
			inline streamer note()
			{
				return streamer( *this, details::message_level::note );
			}
            inline void warn( const std::string& message )
			{
				write( details::message_level::warn, message );
			}
			inline streamer warn()
			{
				return streamer( *this, details::message_level::warn );
			}
            inline void error( const std::string& message )
			{
			    write( details::message_level::error, message );
			}
			inline streamer error()
			{
				return streamer( *this, details::message_level::error );
			}
			inline void debug( const std::string& message )
			{
			    write( details::message_level::debug, message );
			}
			inline streamer debug()
			{
				return streamer( *this, details::message_level::debug );
			}
            inline void fatal( const std::string& message )
			{
				write( details::message_level::fatal, message );
			}
			inline streamer fatal()
			{
				return streamer( *this, details::message_level::fatal );
			}
		protected:
			virtual void write( const details::message_level::value value , const std::string& message )
			{
				using namespace boost::posix_time;
				const std::string current_time = to_simple_string( microsec_clock::universal_time() );
				static const char open_quote = '[';
				stream_ << open_quote << current_time << message_levels[ value ] << message << "\n";
				flush();
			}
		};
		//
		template<>
		void logger< false >::write( const details::message_level::value value , const std::string& message )
		{
		}
		template<>
		void logger< false, false >::write( const details::message_level::value value , const std::string& message )
		{
		}
		template<>
		void logger< false, true, false >::write( const details::message_level::value value, const std::string& message )
		{
		}
		template<>
		void logger< false, false, false >::write( const details::message_level::value value, const std::string& message )
		{
		}

		template<>
		void logger< true, false, false >::write( const details::message_level::value value, const std::string& message )
		{
			stream_ << message << "\n";
		}

		template<>
		void logger< true, true, false >::write( const details::message_level::value value, const std::string& message )
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
		//
		template< bool turn_on, bool flush_stream, bool print_prefix >
		std::string logger< turn_on, flush_stream, print_prefix >::message_levels[ 5 ];
	};
};

#endif // _SYSTEM_UTILITIES_COMMON_LOGGER_H_

