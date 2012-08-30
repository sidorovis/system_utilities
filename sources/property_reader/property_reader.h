#ifndef _SYSTEM_UTILITIES_COMMON_PROPERTY_READER_H_
#define _SYSTEM_UTILITIES_COMMON_PROPERTY_READER_H_

#include <istream>
#include <map>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>

namespace system_utilities
{
	namespace tests_
	{
		namespace common
		{
			class property_reader_test_helper;
		}
	}
    namespace common
    {
        class property_reader
        {
			friend class system_utilities::tests_::common::property_reader_test_helper;
		public:
			typedef std::map< std::string, std::string > properties;
		private:
			std::string default_binary_path_;
			properties properties_;
            property_reader( const property_reader& );
			property_reader& operator=( const property_reader& );
		public:
			explicit property_reader();
			explicit property_reader( const std::string& file_name, const std::string& default_binary_path = "" );
			explicit property_reader( std::istream& stream, const std::string& default_binary_path = "" );
			~property_reader(){}
		public:
			void parse_istream( std::istream& stream );
		private:
			static std::string& delete_comment( std::string& str );
			bool parse_line( const std::string& str );
			bool include_line( const std::string& str );
			static bool test_on_include( const std::string& std, std::string& file_name );
			bool read_sub_property_file( const std::string& file_name );
			bool string_with_setting( const std::string& str );
			bool additional_string_with_setting( const std::string& str );
		public:
			static std::string& trim( std::string& str );
			static const std::string trim(const std::string& str);
			typedef std::vector< std::string > strings;
			static const strings split( const std::string& str, const std::string& delimeters = ",", const bool trim_each = true );
		public:
			const size_t size() const;
            
			const bool get_value( const std::string& parameter_name, const bool& default_param_value ) const;
			const std::string get_value( const std::string& parameter_name, const std::string& default_param_value ) const;
			const std::string get_value( const std::string& parameter_name, const char* const default_param_value = "" ) const;
			template< class result_type >
			const result_type get_value( const std::string& parameter_name, const result_type& default_param_value ) const
			{
				result_type result = default_param_value;
				properties::const_iterator i = properties_.find( parameter_name );
				if ( i != properties_.end() )
					return boost::lexical_cast< result_type >( i->second );
				return result;
			}

			const strings get_values( const std::string& parameter_name, const std::string& delimeters = "," ) const;
			//
			const bool set_value( const std::string& parameter_name, const std::string& value );
			template< class value_type >
			const bool set_value( const std::string& parameter_name, const value_type& value )
			{
				properties_[ parameter_name ] = boost::lexical_cast< std::string >( value );
				return true;
			}
			//
			const bool reset_value( const std::string& parameter_name, const std::string& value );
			template< class value_type >
			const bool reset_value( const std::string& parameter_name, const value_type& value )
			{
				properties_[ parameter_name ] = boost::lexical_cast< std::string >( value );
				return true;
			}
			//
			const bool delete_value( const std::string& parameter_name );
			const bool rename_parameter( const std::string& old_parameter_name, const std::string& new_parameter_name );
			const bool check_value( const std::string& parameter_name ) const;
        };
    };
};

#endif // _SYSTEM_UTILITIES_COMMON_PROPERTY_READER_H_

