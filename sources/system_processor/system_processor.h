#ifndef _SYSTEM_UTILITIES_COMMON_SYSTEM_PROCESSOR_H_
#define _SYSTEM_UTILITIES_COMMON_SYSTEM_PROCESSOR_H_

#include <property_reader.h>
#include <file_logger.h>

#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

namespace system_utilities
{
	namespace tests_
	{
		namespace common
		{
			void system_processor_constructor_tests();
		};
	}
    namespace common
    {
		namespace system_processor
		{

			// system processor is a namespace with a bunch of methods that helps to create multi-thread back-end servers
			// usage example:
			// creating system_processor singleton
			//	system_processor::sp sp = system_processor::init( argc, argv, "configuration.ini" );
			//  run_calculating_threads_or_system();
			//  wait until some thread will call stop() method
			//	sp->wait_for_stop();
			// You can use into your configuration file next strings:
			// * System.log.path = logs (will save all logs to 'logs' folder )
			// * System.log.name = engine.log - will create engine log file, with settings (system log file)
			// * System.stop_by_ctrl_c = true - this settings says - that ctrl+c - should call stop() method and stop application
			// * include new_file.ini - will include new_file.ini as a part of config
		
			namespace details
			{
				class sp_impl;
			}

			//
			typedef boost::shared_ptr< details::sp_impl > sp;
			//
			sp init( const int argc, char * const argv[], const std::string& default_config_file );
			//
			void stop( const std::string& reason );
			void wait_for_stop();
			void reset();
			//
			const std::string binary_path();
			const std::string logs_path();
			//
			const std::string config( const std::string& name, const std::string& default_value = "" );

			template< class result_type >
			result_type config( const std::string& name, const result_type& default_value = result_type() );

			template< class value_type >
			void set_config( const std::string& name, const value_type& value );
			//
			void config_reset_value( const std::string& name, const std::string& default_value = "" );

			template< class value_type >
			void config_reset_value( const std::string& name, const value_type& default_value = value_type() );
			//
			void config_delete_value( const std::string& name );

			const bool config_rename_parameter( const std::string& old_name, const std::string& new_name );
			//
			const bool config_check_value( const std::string& name );

			namespace details
			{
				void exit_handler(int);

				sp init_without_default_config_file( const std::string& binary_path, const std::string& config_file_path );
				sp init_with_default_config_file( const std::string& binary_path, const std::string& config_file_path, const std::string& default_config_file );

				template< class result_type >
				result_type config_impl( const std::string& name, const result_type& default_value = result_type() );

				template< class value_type >
				void set_config_impl( const std::string& name, const value_type& value );

				property_reader::strings config_values_impl( const std::string& name, const std::string& delimeters = "," );

				class sp_impl : protected virtual boost::noncopyable
				{
					friend void tests_::common::system_processor_constructor_tests();

					//
					friend sp system_processor::init( const int argc, char* const argv[], const std::string& default_config_file );
					friend sp init_without_default_config_file( const std::string& binary_path, const std::string& config_file_path );
					friend sp init_with_default_config_file( const std::string& binary_path, const std::string& config_file_path, const std::string& default_config_file );

					template< typename result_type >
					friend result_type config_impl( const std::string& name, const result_type& default_value );

					template< class value_type >
					friend void set_config_impl( const std::string& name, const value_type& value );

					friend property_reader::strings config_values_impl( const std::string& name, const std::string& delimeters );

					template< class value_type >
					friend void config_reset_value( const std::string& name, const value_type& default_value );
					
					friend void config_reset_value( const std::string& name, const std::string& delimeters );
					friend void config_delete_value( const std::string& name );
					friend const bool config_rename_parameter( const std::string& old_name, const std::string& new_name );
					friend const bool config_check_value( const std::string& name );


					friend void system_processor::stop( const std::string& );
					friend void system_processor::wait_for_stop();
					friend void system_processor::reset();
					friend void details::exit_handler(int);
					//
					friend const std::string system_processor::binary_path();
					friend const std::string system_processor::logs_path();

					//
					static boost::mutex instance_protector_;
					static sp_impl* instance_;
				public:
					boost::shared_ptr< property_reader > properties_;
					boost::shared_ptr< file_logger<> > engine_logger_;
				private:
					mutable boost::mutex stop_protector_;
					mutable boost::condition stop_waiter_;
					bool stopping_;

					std::string binary_path_;
					std::string logs_path_;

					static std::string parse_config_file_name(const int argc, char* const argv[]);

					explicit sp_impl( const std::string& binary_path );
					explicit sp_impl( const std::string& binary_path, const std::string& config_path );

					void create_logs_directory();
					void read_logs_path();
					void create_timestamp_folder();
					void create_system_logger();
					void load_predefined_logs_settings();
					void add_exit_handlers();
					//
				public:
					~sp_impl();
					void stop( const std::string& reason );
					void wait_for_stop();
					void reset();
				private:
					static inline void check_that_instance_not_exists()
					{
						if (sp_impl::instance_)
							throw std::logic_error( "init method could not be runned, system created" );
					}
					static inline void check_argc_parameter( const int argc )
					{
						if (argc == 0)
							throw std::logic_error( "please give argc, argv parameters from main() function " );
					}
					inline void check_binary_path( const std::string& binary_path )
					{
						if ( binary_path.empty() || !(boost::filesystem::exists( binary_path )) )
							throw std::logic_error( "wrong argc, argv parameters, please send here real parameters" );
						binary_path_ = binary_path;
					}
				};

				template< class result_type >
				result_type config_impl( const std::string& name, const result_type& default_value )
				{
					boost::mutex::scoped_lock lock( details::sp_impl::instance_protector_ );
					if (!details::sp_impl::instance_)
						throw std::logic_error( "system processor should exist (init method)." );
					if (!details::sp_impl::instance_->properties_.get())
						throw std::logic_error( "config file should be added by parameters." );
					return details::sp_impl::instance_->properties_->get_value( name, default_value );
				}
				template< class value_type >
				void set_config_impl( const std::string& name, const value_type& value )
				{
					boost::mutex::scoped_lock lock( details::sp_impl::instance_protector_ );
					if (!details::sp_impl::instance_)
						throw std::logic_error( "system processor should exist (init method)." );
					if (!details::sp_impl::instance_->properties_.get())
						throw std::logic_error( "config file should be added by parameters." );
					details::sp_impl::instance_->properties_->set_value( name, value );
				}

				template< class value_type >
				void config_reset_value( const std::string& name, const value_type& default_value )
				{
					boost::mutex::scoped_lock lock( details::sp_impl::instance_protector_ );
					if (!details::sp_impl::instance_)
						throw std::logic_error( "system processor should exist (init method)." );
					if (!details::sp_impl::instance_->properties_.get())
						throw std::logic_error( "config file should be added by parameters." );
					details::sp_impl::instance_->properties_->reset_value( name, default_value );
				}
				property_reader::strings config_values_impl( const std::string& name, const std::string& delimeters );
			}
			//
			template< class result_type >
			result_type config( const std::string& name, const result_type& default_value )
			{
				return details::config_impl< result_type >( name, default_value );
			}
			template< class value_type >
			void set_config( const std::string& name, const value_type& value )
			{
				return details::set_config_impl( name, value );
			}
			property_reader::strings config_values( const std::string& name, const std::string& delimeters = "," );
			//

			template< class value_type >
			void config_reset_value( const std::string& name, const value_type& default_value )
			{
				details::config_reset_value< value_type >( name, default_value );
			}
			
			//
			template< class T >
			boost::shared_ptr< file_logger< T > > create_log( const std::string& file_name )
			{
				typedef boost::shared_ptr< file_logger< T > > result_type;
				result_type result( new file_logger< T >( logs_path() + file_name ) );
				return result;
			}
			template< class T, class P1 >
			boost::shared_ptr< file_logger< T > > create_log( const std::string& file_name, const P1& p1 )
			{
				typedef boost::shared_ptr< file_logger< T > > result_type;
				result_type result( new file_logger< T >( logs_path() + file_name, p1 ) );
				return result;
			}
			template< class T, class P1, class P2 >
			boost::shared_ptr< file_logger< T > > create_log( const std::string& file_name, const P1& p1, const P2& p2 )
			{
				typedef boost::shared_ptr< file_logger< T > > result_type;
				result_type result( new file_logger< T >( logs_path() + file_name, p1, p2 ) );
				return result;
			}

		};
	};
};

#endif // _SYSTEM_UTILITIES_COMMON_SYSTEM_PROCESSOR_H_

