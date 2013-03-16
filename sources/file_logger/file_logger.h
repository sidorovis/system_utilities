#ifndef _SYSTEM_UTILITIES_COMMON_FILE_LOGGER_H_
#define _SYSTEM_UTILITIES_COMMON_FILE_LOGGER_H_

#include <fstream>

#include <boost/filesystem.hpp>

#include <logger.h>

namespace system_utilities
{
	namespace tests_
	{
		namespace common
		{
			void file_logger_constructor();
		};
	}
    namespace common
	{
		// default file_logger template class - created to write log into files
		// see logger module - to understand template parameters

		template< class inside_logger = logger< true, true, true > >
		class file_logger : public inside_logger
		{
			std::ofstream file_stream_;
		protected:
			void open_stream( const std::string& file_path, std::ios_base::openmode mode = std::ios_base::app )
			{
				file_stream_.open( file_path.c_str(), mode );
				if (!file_stream_.is_open())
					throw std::logic_error( "file: " + file_path + " could not be opened." );
			}
			void close_stream()
			{
				file_stream_.close();
			}
		public:
			explicit file_logger( const std::string& file_path, std::ios_base::openmode mode = std::ios_base::app )
				: inside_logger( file_stream_ )
			{
				open_stream( file_path, mode );
			}
			template< class P1 >
			explicit file_logger( const std::string& file_path, P1& p1, std::ios_base::openmode mode = std::ios_base::app )
				: inside_logger( file_stream_, p1 )
			{
				open_stream( file_path, mode );
			}
			template< class P1, class P2 >
			explicit file_logger( const std::string& file_path, P1& p1, P2& p2, std::ios_base::openmode mode = std::ios_base::app )
				: inside_logger( file_stream_, p1, p2 )
			{
				open_stream( file_path, mode );
			}
			~file_logger()
			{
				file_stream_.close();
			}
		};

	};
};

#endif // _SYSTEM_UTILITIES_COMMON_FILE_LOGGER_H_

