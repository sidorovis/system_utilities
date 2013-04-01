#ifndef _SYSTEM_UTILITIES_COMMON_SHARED_NAME_STORAGE_H_
#define _SYSTEM_UTILITIES_COMMON_SHARED_NAME_STORAGE_H_

#include <string>
#include <vector>
#include <map>
#include <set>

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace system_utilities
{
	namespace common
	{
		class shared_name_storage : virtual protected boost::noncopyable
		{
		public:
			typedef boost::shared_ptr< std::string > shared_string;
		private:

			typedef std::map< std::string, shared_string > strings_shared_map;
			strings_shared_map str2shared_;
			typedef std::set< shared_string > shared_set;
			shared_set shared_set_;
		public:
			explicit shared_name_storage();
			~shared_name_storage();
			const bool add_name( const std::string& name );
			shared_name_storage& operator<<( const std::string& string );
			shared_string get_shared( const std::string& name ) const;
			const bool check_shared( const shared_name_storage::shared_string& shared ) const;
		};
	}
}


#endif // _SYSTEM_UTILITIES_COMMON_SHARED_NAME_STORAGE_H_
