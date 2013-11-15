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
		class shared_name_storage;

		namespace details
		{
			class shared_name_storage_const_iterator
			{
				friend class shared_name_storage;

				typedef boost::shared_ptr< std::string > shared_string;
				typedef std::map< std::string, shared_string > strings_shared_map;
				mutable strings_shared_map::const_iterator i_;

				explicit shared_name_storage_const_iterator( strings_shared_map::const_iterator i );
			public:
				~shared_name_storage_const_iterator();
				void operator++() const;
				const shared_string& shared() const;
				const std::string& operator*() const;
				bool operator==( const shared_name_storage_const_iterator& other ) const;
				bool operator!=( const shared_name_storage_const_iterator& other ) const;
			};
		}
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
			typedef details::shared_name_storage_const_iterator const_iterator;
		public:
			explicit shared_name_storage();
			~shared_name_storage();
			bool add_name( const std::string& name );
			shared_name_storage& operator<<( const std::string& string );
			shared_string get_shared( const std::string& name ) const;
			bool check_shared( const shared_string& shared ) const;
			//
			void clear();
			bool del_name( const std::string& name );
			bool del_name( const shared_string& name );
			//
			const_iterator begin() const;
			const_iterator end() const;
		};
	}
}


#endif // _SYSTEM_UTILITIES_COMMON_SHARED_NAME_STORAGE_H_
