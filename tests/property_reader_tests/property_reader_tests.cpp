#include "test_registrator.h"

#include <property_reader.h>

using namespace system_utilities::common;

namespace system_utilities
{
	namespace tests_
	{
		namespace common
		{
			class property_reader_test_helper : public property_reader
			{
			public:
				void parse_istream_test_helper()
				{
					std::stringstream stream;
					BOOST_CHECK_NO_THROW( parse_istream( stream ) );
					stream << "hello = world # simple test";
					BOOST_CHECK_NO_THROW( parse_istream( stream ) );
				}
				static void delete_comment_test_helper()
				{
					std::string a_line( "hello = world # comment" );
					property_reader::delete_comment( a_line );
					BOOST_CHECK_EQUAL( a_line, "hello = world" );

					a_line = "dklje ejig; e;ergg werg; # egerg";
					property_reader::delete_comment( a_line );
					BOOST_CHECK_EQUAL( a_line, "dklje ejig" );

					a_line = "dklje #ejig; e;ergg werg; # egerg";
					property_reader::delete_comment( a_line );
					BOOST_CHECK_EQUAL( a_line, "dklje" );
				}
				void parse_line_test_helper()
				{
					BOOST_CHECK_EQUAL( parse_line( "" ), false );
					BOOST_CHECK_EQUAL( parse_line( "    " ), false );
					BOOST_CHECK_EQUAL( parse_line( " hello world " ), false );
					BOOST_CHECK_EQUAL( parse_line( " = uehfuehf aloha " ), false );
					BOOST_CHECK_EQUAL( parse_line( " = uehfuehf " ), false );
					BOOST_CHECK_EQUAL( parse_line( " uehfuehf wonderfull = " ), false );
					BOOST_CHECK_EQUAL( parse_line( " uehfuehf.wonderfull += " ), true );
					BOOST_CHECK_THROW( parse_line( " include tree " ), std::invalid_argument );
					BOOST_CHECK_EQUAL( parse_line( " additinal.settings = nosettings " ), true );
				}
				void include_line_test_helper()
				{
					bool result;
					BOOST_CHECK_NO_THROW( result = include_line( " no include " ) );
					BOOST_CHECK_EQUAL( result, false );
					BOOST_CHECK_NO_THROW( result = include_line( " included filename " ) );
					BOOST_CHECK_EQUAL( result, false );
					BOOST_CHECK_THROW( result = include_line( " include filename " ), std::invalid_argument );
				}
				static void test_on_include_test_helper()
				{
					std::string file_name;
					BOOST_CHECK_EQUAL( test_on_include( "hello_world", file_name ), false );
					BOOST_CHECK_EQUAL( test_on_include( "include", file_name ), false );
					BOOST_CHECK_EQUAL( test_on_include( "include asd", file_name ), true );
					BOOST_CHECK_EQUAL( file_name, "asd" );
					BOOST_CHECK_EQUAL( test_on_include( " include asd ", file_name ), true );
					BOOST_CHECK_EQUAL( file_name, "asd" );
					BOOST_CHECK_EQUAL( test_on_include( " include d://hello//world//file ", file_name ), true );
					BOOST_CHECK_EQUAL( file_name, "d://hello//world//file" );
					BOOST_CHECK_EQUAL( test_on_include( " include   d://hello//world//file ", file_name ), true );
					BOOST_CHECK_EQUAL( file_name, "d://hello//world//file" );
					BOOST_CHECK_EQUAL( test_on_include( " include   d:\\my\\file\\..\\file ", file_name ), true );
					BOOST_CHECK_EQUAL( file_name, "d:\\my\\file\\..\\file" );
					BOOST_CHECK_EQUAL( test_on_include( " include   ./file/name ", file_name ), true );
					BOOST_CHECK_EQUAL( file_name, "./file/name" );
					BOOST_CHECK_EQUAL( test_on_include( " include   ../../file/name ", file_name ), true );
					BOOST_CHECK_EQUAL( file_name, "../../file/name" );
					BOOST_CHECK_EQUAL( test_on_include( " include   d://hello// world//file ", file_name ), true );
					BOOST_CHECK_EQUAL( file_name, "d://hello// world//file" );
					BOOST_CHECK_EQUAL( test_on_include( " include   d:/h ello// world//file ", file_name ), true );
					BOOST_CHECK_EQUAL( file_name, "d:/h ello// world//file" );
				}
				void string_with_setting_test_helper()
				{
					BOOST_CHECK_EQUAL( string_with_setting( "" ), false );
					BOOST_CHECK_EQUAL( string_with_setting( "   " ), false );
					BOOST_CHECK_EQUAL( string_with_setting( " = " ), false );
					BOOST_CHECK_EQUAL( string_with_setting( " = hello" ), false );
					BOOST_CHECK_EQUAL( string_with_setting( " world = " ), true );
					BOOST_CHECK_EQUAL( string_with_setting( " My.Parameter.001_help = " ), true );
					BOOST_CHECK_EQUAL( string_with_setting( " My.Parameter.001_help2 = Any text%^&*# representation, r.rg, rtht" ), true );
					BOOST_CHECK_EQUAL( properties_[ "My.Parameter.001_help2" ], "Any text%^&*# representation, r.rg, rtht" );
					BOOST_CHECK_EQUAL( string_with_setting( " My.Parameter.,001_help2 = Any text%^&*# representation, r.rg, rtht" ), false );
					BOOST_CHECK_EQUAL( string_with_setting( " My.Parameter.-001_help2 = Any text%^&*# representation, r.rg, rtht" ), false );
					BOOST_CHECK_EQUAL( string_with_setting( " My.Parameter.|001_help2 = Any text%^&*# representation, r.rg, rtht" ), false );
					BOOST_CHECK_EQUAL( string_with_setting( " My.Parameter.+001_help2 = Any text%^&*# representation, r.rg, rtht" ), false );
					BOOST_CHECK_EQUAL( string_with_setting( " My.Parameter.$001_help2 = Any text%^&*# representation, r.rg, rtht" ), false );
				}
				void additional_string_with_setting_test_helper()
				{
					BOOST_CHECK_EQUAL( string_with_setting( " My.Parameter.001_help = " ), true );
					BOOST_CHECK_EQUAL( properties_[ "My.Parameter.001_help" ], "" );
					BOOST_CHECK_EQUAL( additional_string_with_setting( " My.Parameter.001_help += " ), true );
					BOOST_CHECK_EQUAL( properties_[ "My.Parameter.001_help" ], "" );
					BOOST_CHECK_EQUAL( additional_string_with_setting( " My.Parameter.001_help += hello" ), true );
					BOOST_CHECK_EQUAL( properties_[ "My.Parameter.001_help" ], " hello" );
					BOOST_CHECK_EQUAL( additional_string_with_setting( " My.Parameter.001_help += , world" ), true );
					BOOST_CHECK_EQUAL( properties_[ "My.Parameter.001_help" ], " hello , world" );
					BOOST_CHECK_EQUAL( string_with_setting( " My.Parameter.001_help = erase" ), true );
					BOOST_CHECK_EQUAL( properties_[ "My.Parameter.001_help" ], "erase" );
					BOOST_CHECK_EQUAL( additional_string_with_setting( " My.Parameter.001_help = , world" ), false );
					BOOST_CHECK_EQUAL( additional_string_with_setting( " My.Parameter.|001_help += , world" ), false );
					BOOST_CHECK_EQUAL( additional_string_with_setting( " My.Parameter.+001_help += , world" ), false );
					BOOST_CHECK_EQUAL( additional_string_with_setting( " My.Parameter.+001_help = , world" ), false );
					BOOST_CHECK_EQUAL( additional_string_with_setting( " My.Parameter.-001_help += , world" ), false );
					BOOST_CHECK_EQUAL( additional_string_with_setting( " My.Parameter.,001_help += , world" ), false );
					BOOST_CHECK_EQUAL( additional_string_with_setting( " My.Parameter. 001_help += , world" ), false );
				}
				void read_sub_property_file_test_helper()
				{
					BOOST_CHECK_THROW( read_sub_property_file("filename"), std::invalid_argument );
				}
			};
			///
			void property_reader_constructor_tests()
			{
				BOOST_CHECK_NO_THROW( property_reader pr );
				std::stringstream str;
				BOOST_CHECK_NO_THROW( property_reader( str ) );
				BOOST_CHECK_NO_THROW( property_reader( SOURCE_DIR "/tests/data/property_reader/good_001.ini" ) );
				BOOST_CHECK_THROW( property_reader( SOURCE_DIR "/tests/data/property_reader/bad_001.ini" ), std::logic_error );
			}
			void property_reader_parse_istream_tests()
			{
				property_reader_test_helper prth;
				prth.parse_istream_test_helper();
			}
			void property_reader_delete_comment_tests()
			{
				property_reader_test_helper::delete_comment_test_helper();
			}
			void property_reader_parse_line_tests()
			{
				property_reader_test_helper prth;
				prth.parse_line_test_helper();
			}
			void property_reader_include_line_tests()
			{
				property_reader_test_helper prth;
				prth.include_line_test_helper();
			}
			void property_reader_test_on_include_tests()
			{
				property_reader_test_helper prth;
				prth.test_on_include_test_helper();
			}
			void property_reader_string_with_setting_tests()
			{
				property_reader_test_helper prth;
				prth.string_with_setting_test_helper();
			}
			void property_reader_additional_string_with_setting_tests()
			{
				property_reader_test_helper prth;
				prth.additional_string_with_setting_test_helper();
			}
			void property_reader_read_sub_property_file_tests()
			{
				property_reader_test_helper prth;
				prth.read_sub_property_file_test_helper();
			}
			void property_reader_trim_tests()
			{
				std::string str1 = "  erger eerg \t ";
				BOOST_CHECK_EQUAL( property_reader::trim( str1 ), "erger eerg" );
				BOOST_CHECK_EQUAL( str1, "erger eerg" );
				str1 = "  hello ";
				BOOST_CHECK_EQUAL( property_reader::trim( str1 ), "hello" );
				BOOST_CHECK_EQUAL( str1, "hello" );
				str1 = " world  ";
				BOOST_CHECK_EQUAL( property_reader::trim( str1 ), "world" );
				BOOST_CHECK_EQUAL( str1, "world" );
			}
			void property_reader_const_trim_tests()
			{
				{
					const std::string str1 = "  erger eerg \t ";
					BOOST_CHECK_EQUAL( property_reader::trim( str1 ), "erger eerg" );
					BOOST_CHECK_EQUAL( str1, "  erger eerg \t ");
				}
				{
					const std::string str1 = "  hello ";
					BOOST_CHECK_EQUAL( property_reader::trim( str1 ), "hello" );
					BOOST_CHECK_EQUAL( str1, "  hello ");
				}
				{
					const std::string str1 = " world  ";
					BOOST_CHECK_EQUAL( property_reader::trim( str1 ), "world" );
					BOOST_CHECK_EQUAL( str1, " world  ");
				}
			}
			void property_reader_split_tests()
			{
				const std::string settings = " as, we. fe, feef, weffw, sdfsf, ";
				{
					const property_reader::strings s = property_reader::split( settings );
					BOOST_CHECK_EQUAL( s.size(), (size_t)5 );
					BOOST_CHECK_EQUAL( s[0], "as" );
					BOOST_CHECK_EQUAL( s[1], "we. fe" );
					BOOST_CHECK_EQUAL( s[2], "feef" );
					BOOST_CHECK_EQUAL( s[3], "weffw" );
					BOOST_CHECK_EQUAL( s[4], "sdfsf" );
				}
				{
					const property_reader::strings s = property_reader::split( settings, " " );
					BOOST_CHECK_EQUAL( s.size(), (size_t)6 );
					BOOST_CHECK_EQUAL( s[0], "as," );
					BOOST_CHECK_EQUAL( s[1], "we." );
					BOOST_CHECK_EQUAL( s[2], "fe," );
					BOOST_CHECK_EQUAL( s[3], "feef," );
					BOOST_CHECK_EQUAL( s[4], "weffw," );
					BOOST_CHECK_EQUAL( s[5], "sdfsf," );
				}
				{
					const std::string settings = " as, we. fe,\t feef, weffw, sdfsf, ";
					const property_reader::strings s = property_reader::split( settings, " ", false );
					BOOST_CHECK_EQUAL( s.size(), (size_t)6 );
					BOOST_CHECK_EQUAL( s[0], "as," );
					BOOST_CHECK_EQUAL( s[1], "we." );
					BOOST_CHECK_EQUAL( s[2], "fe,\t" );
					BOOST_CHECK_EQUAL( s[3], "feef," );
					BOOST_CHECK_EQUAL( s[4], "weffw," );
					BOOST_CHECK_EQUAL( s[5], "sdfsf," );
				}
				{
					const std::string settings = " as, we. fe,\t feef, weffw, sdfsf, ";
					const property_reader::strings s = property_reader::split( settings, ", ", false );
					BOOST_CHECK_EQUAL( s.size(), (size_t)7 );
					BOOST_CHECK_EQUAL( s[0], "as" );
					BOOST_CHECK_EQUAL( s[1], "we." );
					BOOST_CHECK_EQUAL( s[2], "fe" );
					BOOST_CHECK_EQUAL( s[3], "\t" );
					BOOST_CHECK_EQUAL( s[4], "feef" );
					BOOST_CHECK_EQUAL( s[5], "weffw" );
					BOOST_CHECK_EQUAL( s[6], "sdfsf" );
				}
			}
			void property_reader_comment_tests()
			{
				std::stringstream str;
				str << "; hello = world" << std::endl;
				str << std::endl;
				str << "# comment" << std::endl;
				property_reader reader( str );
				BOOST_CHECK_EQUAL( reader.size(), (size_t)0 );
			}
			void property_reader_small_amount_of_parameters_tests()
			{
				std::stringstream str;
				str << "; hello = world" << std::endl;
				str << std::endl;
				str << "# comment" << std::endl;
				str << " hello = world " << std::endl;
				str << " hello = world2 " << std::endl;
				str << " hell = add # for you " << std::endl;
				str << " hella = add4; for you " << std::endl;
				str << " hello = world3 " << std::endl;
				str << " hello = world3" << std::endl;
				str << "pirac=fint" << std::endl;
				property_reader reader( str );
				BOOST_CHECK_EQUAL( reader.size(), (size_t)4 );
				BOOST_CHECK_EQUAL( reader.get_value("hello"), "world3" );
				BOOST_CHECK_EQUAL( reader.check_value("hello"), true );
				BOOST_CHECK_EQUAL( reader.get_value("hello3"), "" );
				BOOST_CHECK_EQUAL( reader.check_value("hello3"), false );
				BOOST_CHECK_EQUAL( reader.get_value("hello3", "def_val"), "def_val" );
				BOOST_CHECK_EQUAL( reader.get_value("hell"), "add" );
				BOOST_CHECK_EQUAL( reader.get_value("pirac"), "fint" );
				BOOST_CHECK_EQUAL( reader.get_value("hella"), "add4" );

				std::stringstream throwsed_str;
				throwsed_str << " comunisti!!! ubivat' moy server!!! ";
				BOOST_CHECK_THROW( property_reader reader( throwsed_str ), std::exception );
			}

		}
	}
}
