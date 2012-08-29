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

		template< class inside_logger = logger< true, true, true > >
		class file_logger;

		namespace details
		{
			template< class inside_logger = logger< true, true, true > >
			class file_logger_pimpl : public inside_logger
			{
				friend class file_logger< inside_logger >;

				explicit file_logger_pimpl( std::ofstream& stream )
					: inside_logger( stream )
				{
				}
				template< class P1 >
				explicit file_logger_pimpl( std::ofstream& stream, P1& p1 )
					: inside_logger( stream, p1 )
				{
				}
				template< class P1, class P2 >
				explicit file_logger_pimpl( std::ofstream& stream, P1& p1, P2& p2 )
					: inside_logger( stream, p1, p2 )
				{
				}
			public:
				~file_logger_pimpl()
				{
				}
			};

		}

		template< class inside_logger >
		class file_logger : public boost::shared_ptr< details::file_logger_pimpl< inside_logger > >
		{
			typedef details::file_logger_pimpl< inside_logger > pimpl;
			std::ofstream file_stream_;

			void open_stream( const std::string& file_path )
			{
				using namespace boost::filesystem;
				path p( file_path );
				if (exists( p ))
					throw std::logic_error( "file: " + file_path + " exists, we can't use it for log file" );
				file_stream_.open( file_path.c_str(), std::ios::out );
				if (!file_stream_.is_open())
					throw std::logic_error( "file: " + file_path + " could not be opened." );
			}
		public:
			explicit file_logger( const std::string& file_path )
			{
				using namespace boost::filesystem;
				path p( file_path );
				/// TODO check on existance
				file_stream_.open( file_path.c_str(), std::ios::out );
				this->reset( new pimpl( file_stream_ ) );
			}
			template< class P1 >
			explicit file_logger( const std::string& file_path, P1& p1 )
			{
				open_stream( file_path );
				this->reset( new pimpl( file_stream_, p1 ) );
			}
			template< class P1, class P2 >
			explicit file_logger( const std::string& file_path, P1& p1, P2& p2 )
			{
				using namespace boost::filesystem;
				path p( file_path );
				/// TODO check on existance
				file_stream_.open( file_path.c_str(), std::ios::out );
				this->reset( new pimpl( file_stream_, p1, p2 ) );
			}
			~file_logger()
			{
				this->reset();
			}
			//

            inline void note( const std::string& message )
			{
                this->get()->note( message );
			}
			inline typename pimpl::streamer note()
			{
				return this->get()->note();
			}
            inline void warn( const std::string& message )
			{
				this->get()->warn( message );
			}
			inline typename pimpl::streamer warn()
			{
				return this->get()->warn();
			}
            inline void error( const std::string& message )
			{
			    this->get()->error( message );
			}
			inline typename pimpl::streamer error()
			{
				return this->get()->error();
			}
			inline void debug( const std::string& message )
			{
			    this->get()->debug( message );
			}
			inline typename pimpl::streamer debug()
			{
				return this->get()->debug();
			}
            inline void fatal( const std::string& message )
			{
			    this->get()->fatal( message );
			}
			inline typename pimpl::streamer fatal()
			{
				return this->get()->fatal();
			}
			inline void flush()
			{
			    this->get()->flush();
			}

		};

	};
};

#endif // _SYSTEM_UTILITIES_COMMON_FILE_LOGGER_H_

