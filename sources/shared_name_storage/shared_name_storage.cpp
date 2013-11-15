
#include "shared_name_storage.h"

#include <stdexcept>

namespace system_utilities
{
	namespace common
	{
		namespace details
		{
			shared_name_storage_const_iterator::shared_name_storage_const_iterator( shared_set::const_iterator i )
				: i_( i )
			{
			}
			shared_name_storage_const_iterator::~shared_name_storage_const_iterator()
			{
			}
			void shared_name_storage_const_iterator::operator++(  ) const
			{
				++(i_);
			}
			const shared_name_storage_const_iterator::shared_string& shared_name_storage_const_iterator::shared() const
			{
				return *i_;
			}
			const std::string& shared_name_storage_const_iterator::operator*() const 
			{
				return *(*i_);
			}
			bool shared_name_storage_const_iterator::operator==( const shared_name_storage_const_iterator& other ) const
			{
				return i_ == other.i_;
			}
			bool shared_name_storage_const_iterator::operator!=( const shared_name_storage_const_iterator& other ) const
			{
				return i_ != other.i_;
			}
		}
		//
		shared_name_storage::shared_name_storage()
		{
		}
		shared_name_storage::~shared_name_storage()
		{
		}
		//
		bool shared_name_storage::add_name( const std::string& name )
		{
			if ( name.empty() )
				throw std::logic_error( "shared name storage can't store empty string" );
			strings_shared_map::const_iterator i = str2shared_.find( name );
			if ( i != str2shared_.end() )
				return false;
			shared_string new_shared_string( new std::string( name ) );
			str2shared_[ name ] = new_shared_string;
			shared_set_.insert( new_shared_string );
			return true;
		}
		shared_name_storage& shared_name_storage::operator<<( const std::string& string )
		{
			add_name( string );
			return *this;
		}
		shared_name_storage::shared_string shared_name_storage::get_shared( const std::string& name ) const
		{
			strings_shared_map::const_iterator i = str2shared_.find( name );
			if ( i == str2shared_.end() )
				return shared_string( new std::string( "" ) );
			return i->second;
		}
		bool shared_name_storage::check_shared( const shared_name_storage::shared_string& shared ) const
		{
			return shared_set_.find( shared ) != shared_set_.end();
		}
		//
		void shared_name_storage::clear()
		{
			shared_set_.clear();
			str2shared_.clear();
		}
		bool shared_name_storage::del_name( const std::string& name )
		{
			strings_shared_map::iterator i = str2shared_.find( name );
			if ( i == str2shared_.end() )
				throw std::invalid_argument( "no " + name + " shared string" );
			shared_set_.erase( i->second );
			str2shared_.erase( i );
			return true;
		}
		bool shared_name_storage::del_name( const shared_string& name )
		{
			shared_set::iterator i = shared_set_.find( name );
			if ( i == shared_set_.end() )
				throw std::invalid_argument( "no " + *name + " shared string" );
			str2shared_.erase( *name );
			shared_set_.erase( i );
			return true;
		}
		//
		shared_name_storage::const_iterator shared_name_storage::begin() const
		{
			return const_iterator( shared_set_.begin() );
		}
		shared_name_storage::const_iterator shared_name_storage::end() const
		{
			return const_iterator( shared_set_.end() );
		}
	}
}
