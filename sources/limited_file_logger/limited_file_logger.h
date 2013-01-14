#ifndef _SYSTEM_UTILITIES_COMMON_LIMITED_FILE_LOGGER_H_
#define _SYSTEM_UTILITIES_COMMON_LIMITED_FILE_LOGGER_H_

#include <fstream>

#include <boost/filesystem.hpp>

#include <file_logger.h>

namespace system_utilities
{
    namespace common
	{

		// default limited_file_logger template class - created to write log into files with auto limit
		// limit is unsigned integer value that should be > 0; this value shows limit in megabytes for log files
		// for example: log file name = "my.log", file_size_limit = 1, auto_delete = true
		// you will find logs: "my.log" - current log for working system and "my.log.old" for old log 
		// see logger module - to understand template parameters

		namespace details
		{
			template< bool turn_on = true, bool flush_stream = true, bool print_prefix = true >
			struct message_size_counter
			{
				static size_t message_size( const details::message_level::value value, const std::string& message );
			};
			
		}

		template< 
			bool turn_on = true, 
			bool flush_stream = true, 
			bool print_prefix = true, 
			const size_t file_size_limit = 1024ul,
			class logger_type = logger< turn_on, flush_stream, print_prefix > >
		class limited_file_logger : public file_logger< logger_type >
		{
			typedef file_logger< logger_type > inside_logger;
			std::string file_path_;
			size_t current_file_size_;
		public:
			explicit limited_file_logger( const std::string& file_path, std::ios::open_mode open_mode = std::ios::app )
				: inside_logger( file_path, open_mode )
				, file_path_( file_path )
			{
				fill_file_size_on_open_( file_path );
			}
			template< class P1 >
			explicit limited_file_logger( const std::string& file_path, P1& p1, std::ios::open_mode open_mode = std::ios::app )
				: inside_logger( file_path, p1, open_mode )
				, file_path_( file_path )
			{
				fill_file_size_on_open_( file_path );
			}
			template< class P1, class P2 >
			explicit limited_file_logger( const std::string& file_path, P1& p1, P2& p2, std::ios::open_mode open_mode = std::ios::app )
				: inside_logger( file_path, p1, p2, open_mode )
				, file_path_( file_path )
			{
				fill_file_size_on_open_( file_path );
			}
			~limited_file_logger()
			{
			}
		protected:
			void write( const details::message_level::value value, const std::string& message )
			{
				const size_t new_message_size = details::message_size_counter< turn_on, flush_stream, print_prefix >::message_size( value, message );

				const size_t file_size_limit_in_bytes = file_size_limit * 1024 * 1024;
				if ( current_file_size_ + new_message_size >= file_size_limit_in_bytes )
					rename_( file_path_ );

				current_file_size_ += new_message_size;
				inside_logger::write( value, message );
			}
		private:
			void fill_file_size_on_open_( const std::string& file_path )
			{
				current_file_size_ = boost::filesystem::file_size( file_path );
				const size_t file_size_limit_in_bytes = file_size_limit * 1024 * 1024;
				if ( current_file_size_ >= file_size_limit_in_bytes )
					rename_( file_path );
			}
			void rename_( const std::string& file_path )
			{
				close_stream();
				const std::string old_file_path = file_path + ".old";
				if ( boost::filesystem::exists( old_file_path ) )
					boost::filesystem::remove( old_file_path );
				boost::filesystem::rename( file_path, old_file_path );
				open_stream( file_path );
				current_file_size_ = 0;
			}
		};

	};
};

#endif // _SYSTEM_UTILITIES_COMMON_LIMITED_FILE_LOGGER_H_

