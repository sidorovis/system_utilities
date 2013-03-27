#ifndef _SYSTEM_UTILITIES_COMMON_MULTIKEY_TREE_H_
#define _SYSTEM_UTILITIES_COMMON_MULTIKEY_TREE_H_

#include <string>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

namespace system_utilities
{
	namespace common
	{
		namespace details
		{
			class multikey_tree_hasher : public std::unary_function< const std::string::const_iterator& , const char >
			{
			public:
				result_type operator()( argument_type arg )
				{
					if (*arg < '/' || *arg > 'z')
					{
						throw std::invalid_argument( std::string("invalid symbol in stock name : ") + (*arg) );
					}
					else
					{
						return *arg - '/';
					}
				}
			};
		}

		template< class value_type, const size_t key_range_size = 'z' - '/' + 1 >
		class multikey_tree
		{
			static details::multikey_tree_hasher hasher_;
			value_type node_data_;

			typedef std::string key_type;
			typedef key_type::const_iterator key_const_iterator;
			typedef std::vector< multikey_tree< value_type, key_range_size >* > children_nodes;

			children_nodes children_nodes_;
			mutable boost::mutex children_nodes_mutex_;

		public:
			explicit multikey_tree< value_type, key_range_size >()
				: node_data_()
				, children_nodes_( key_range_size, NULL )
			{
			}

			~multikey_tree()
			{
				for ( typename children_nodes::iterator i = children_nodes_.begin() ; i != children_nodes_.end(); ++i )
					delete *i;
			};


			value_type* get_for_edit( const std::string& key )
			{
				return get_for_edit( key.begin(), key.end() );
			}
			value_type* get_for_edit( key_const_iterator stock_name_hash_suffix_iterator_begin, const key_const_iterator stock_name_hash_suffix_iterator_end )
			{
				if ( stock_name_hash_suffix_iterator_begin == stock_name_hash_suffix_iterator_end )
				{
					return &node_data_;
				}
				else
				{
					const size_t index =  hasher_(stock_name_hash_suffix_iterator_begin);
					{
						boost::mutex::scoped_lock lock(children_nodes_mutex_);
						if (children_nodes_[index] == NULL)
							children_nodes_[index] = new multikey_tree();
					}
					return children_nodes_[index]->get_for_edit(stock_name_hash_suffix_iterator_begin + 1, stock_name_hash_suffix_iterator_end);
				}
			}


			const value_type* get_for_read( key_const_iterator stock_name_hash_suffix_iterator_begin, key_const_iterator stock_name_hash_suffix_iterator_end ) const
			{
				if ( stock_name_hash_suffix_iterator_begin == stock_name_hash_suffix_iterator_end )
				{
					return &node_data_;
				}
				else
				{
					const size_t index = hasher_(stock_name_hash_suffix_iterator_begin);
					{
						boost::mutex::scoped_lock lock(children_nodes_mutex_);
						if ( children_nodes_[index] == NULL )
							throw std::invalid_argument(" Incorrect request parameter ");
					}
					return children_nodes_[index]->get_for_read(stock_name_hash_suffix_iterator_begin + 1, stock_name_hash_suffix_iterator_end);
				}
			}

			void clear()
			{
				for( size_t i = 0; i < key_range_size; ++i )
					if ( children_nodes_[i] != NULL )
					{
						delete children_nodes_[i];
						children_nodes_[i] = NULL;
					}
			}

		};

		template< class value_type, const size_t key_range_size >
		details::multikey_tree_hasher multikey_tree< value_type, key_range_size >::hasher_;

	};
};


#endif // _SYSTEM_UTILITIES_COMMON_MULTIKEY_TREE_H_
