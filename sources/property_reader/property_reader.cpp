
#include "property_reader.h"

#include <fstream>
#include <string.h>
#include <stdexcept> 

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <boost/filesystem.hpp>

using namespace system_utilities::common;

property_reader::property_reader()
{
}

property_reader::property_reader( const std::string& file_name, const std::string& default_binary_path )
: default_binary_path_( default_binary_path )
{
	read_sub_property_file( file_name );
}
property_reader::property_reader( std::istream& stream, const std::string& default_binary_path )
: default_binary_path_( default_binary_path )
{
	parse_istream( stream );
}
void property_reader::parse_istream( std::istream& stream )
{
	size_t line_counter = 0;
	while (!stream.eof())
	{
		line_counter ++;
		std::string line;
		std::getline( stream, line );
		delete_comment( line );
		if (line.length() == 0 || line[0] == '\0')
			continue;
		if (! parse_line( line ) )
			throw std::logic_error( "configuration file have bad format on line: " + boost::lexical_cast< std::string >( line_counter ) );
	}
}
std::string& property_reader::delete_comment( std::string& str )
{
	const size_t index = str.find_first_of("#;");
	if ( index != str.npos )
		str.erase( index );
	trim( str );
	return str;
}
bool property_reader::parse_line( const std::string& line )
{
	if ( include_line( line ) )
		return true;
	if ( string_with_setting( line ) )
		return true;
	if ( additional_string_with_setting( line ) )
		return true;
	return false;
}
bool property_reader::include_line( const std::string& str )
{
	std::string file_name;
	if ( test_on_include( str, file_name ) )
		return read_sub_property_file( file_name );
	return false;
}
bool property_reader::test_on_include( const std::string& str, std::string& file_name )
{
	static const boost::regex include_pattern( "\\s*include\\s+(.+)\\s*" );
	boost::smatch matching;
	if ( boost::regex_match( str, matching, include_pattern ) )
	{
		file_name = matching[1];
		trim( file_name );
		return true;
	}
	return false;
}


bool property_reader::read_sub_property_file( const std::string& file_name )
{
	std::string file_path = file_name;
	if ( !boost::filesystem::exists( file_path ) )
	{
		file_path = default_binary_path_ + file_name;
		if ( !boost::filesystem::exists( file_path ) )
			throw std::invalid_argument( "file: " + file_name + " not found" );
	}
	std::ifstream input( file_path.c_str(), std::ios::in );
	if (!input.is_open())
		throw std::invalid_argument( "file: " + file_name + " could not be opened" );
	parse_istream( input );
	return true;
}
bool property_reader::string_with_setting( const std::string& str )
{
	static const boost::regex string_with_setting("\\s*([a-zA-Z0-9\\.\\_]+)\\s*\\=\\s*(.+)\\s*" );
	boost::smatch matching;

	if ( boost::regex_match( str, matching, string_with_setting ) )
	{
		std::string property_name = matching[1];
		trim( property_name );
		std::string value = matching[2];
		trim( value );
		if ( !property_name.empty() )
		{
			properties_[ property_name ] = value;
			return true;
		}
	}
	return false;
}
bool property_reader::additional_string_with_setting( const std::string& str )
{
	static const boost::regex string_with_setting("\\s*([a-zA-Z0-9\\.\\_]+)\\s*\\+\\=\\s*(.+)\\s*" );
	boost::smatch matching;

	if ( boost::regex_match( str, matching, string_with_setting ) )
	{
		std::string property_name = matching[1];
		trim( property_name );
		std::string value = matching[2];
		trim( value );
		if ( !property_name.empty() )
		{
			if ( !value.empty() )
				properties_[ property_name ] += " " + value;
			return true;
		}
	}
	return false;
}

std::string property_reader::trim( const std::string& str )
{
	std::string result;
	size_t f_s_index = str.find_first_not_of("\t \0");
	size_t l_s_index = str.find_last_not_of("\t \0");
	if (f_s_index != std::string::npos && l_s_index != std::string::npos)
		result = str.substr(f_s_index, l_s_index-f_s_index + 1);
	if (f_s_index == std::string::npos)
		result = "";
	return result;
}
std::string& property_reader::trim( std::string& str )
{
	size_t f_s_index = str.find_first_not_of("\t \0");
	size_t l_s_index = str.find_last_not_of("\t \0");
	if (f_s_index != std::string::npos && l_s_index != std::string::npos)
		str = str.substr(f_s_index, l_s_index-f_s_index + 1);
	if (f_s_index == std::string::npos)
		str = "";
	return str;
}

namespace 
{
	bool is_empty( const std::string& str )
	{
		return str.empty();
	}
}

property_reader::strings property_reader::split( const std::string& str, const std::string& delimeters, const bool trim_each )
{
	strings result;
	boost::algorithm::split( result, str, boost::algorithm::is_any_of( delimeters ));
	if ( trim_each )
		for (strings::iterator i = result.begin() ; i != result.end() ; ++i )
			trim( *i );
	result.erase( std::remove_if( result.begin(), result.end(), is_empty ), result.end() );
	return result;
}

size_t property_reader::size() const
{
	return properties_.size();
}

bool property_reader::get_value( const std::string& parameter_name, const bool& default_param_value ) const
{
    bool result = default_param_value;
    properties::const_iterator i = properties_.find( parameter_name );
    if ( i != properties_.end() )
    {
        if (i->second == "true" || i->second == "on" || i->second == "1" || i->second == "TRUE")
            return true;
        else
            return false;
    }
    return result;
}
std::string property_reader::get_value( const std::string& parameter_name, const char* const default_param_value ) const
{
    return get_value( parameter_name, std::string( default_param_value ) );
}
std::string property_reader::get_value( const std::string& parameter_name, const std::string& default_param_value ) const
{
    properties::const_iterator i = properties_.find( parameter_name );
    if ( i != properties_.end() )
        return i->second;
    return default_param_value;
}
property_reader::strings property_reader::get_values( const std::string& parameter_name, const std::string& delimeters ) const
{
    properties::const_iterator i = properties_.find( parameter_name );
    if ( i != properties_.end() )
        return split( i->second, delimeters, true );
    return strings();
}

bool property_reader::set_value( const std::string& parameter_name, const std::string& value )
{
    properties_[ parameter_name ] = value;
    return true;
}

bool property_reader::reset_value( const std::string& parameter_name, const std::string& value )
{
    properties_[ parameter_name ] = value;
    return true;
}


bool property_reader::delete_value( const std::string& parameter_name )
{
	properties_.erase( parameter_name );
	return true;
}

bool property_reader::rename_parameter( const std::string& old_parameter_name, const std::string& new_parameter_name )
{
	properties::iterator i = properties_.find( old_parameter_name );
	if ( i != properties_.end() )
	{	
		reset_value( new_parameter_name, i->second );
		properties_.erase( i );
		return true;
	}
	return false;
}

bool property_reader::check_value( const std::string& parameter_name ) const
{
	properties::const_iterator i = properties_.find( parameter_name );
	return (i != properties_.end());
}
