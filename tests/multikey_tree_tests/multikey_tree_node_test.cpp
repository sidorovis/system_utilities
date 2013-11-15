#include "test_registrator.h"

#include <cstring>

#include <boost/cstdint.hpp>

#include <multikey_tree.h>
#include <time_tracker.h>


using namespace system_utilities::common;

namespace system_utilities
{
	namespace tests_
	{
		namespace common
		{
			namespace
			{
				template< class CumVolTy = size_t >
				class node_data
				{
				private:
					CumVolTy cumulative_volume_;
					mutable boost::mutex cumulative_volume_mutex_;

				public:
					explicit node_data< CumVolTy >():
					cumulative_volume_(0)
					{
					}

					explicit node_data< CumVolTy > (const node_data< CumVolTy >& copy_obj)
					{
						cumulative_volume_ = copy_obj.get_value();
					}

					~node_data(){};

					void add( CumVolTy add_volume )
					{
						boost::mutex::scoped_lock lock( cumulative_volume_mutex_ );
						cumulative_volume_ += add_volume;
					}

					CumVolTy get_value(void) const
					{
						boost::mutex::scoped_lock lock( cumulative_volume_mutex_ );
						return cumulative_volume_;
					}

					CumVolTy add_and_get_value(const CumVolTy add_volume)
					{
						boost::mutex::scoped_lock lock( cumulative_volume_mutex_ );
						cumulative_volume_ += add_volume;
						return cumulative_volume_;
					}
				};

				//
				typedef std::pair<std::string, size_t> test_signal_pair;
				typedef std::vector< test_signal_pair > test_vector_of_signals;
				typedef multikey_tree< node_data< size_t > > test_thread_root_def;
				typedef std::vector< test_vector_of_signals > signal_matrix;

				const std::string get_random_stock_name( const size_t max_name_len )
				{
					static size_t key_range_size = 'z' - '/' + 1;

					size_t len = rand() % max_name_len + 1;

					std::string res;
					res.resize(len);

					for (size_t i = 0; i < len; ++i)
						res[i] =(char) (rand() % key_range_size + '/');

					return res;
				}

				void test_init_data_matrix( signal_matrix& test_all_data_matrix, const size_t num_thread, const size_t num_signals, const size_t max_stock_name_len)
				{
					for (size_t i = 0; i < num_thread; ++i)
					{
						test_vector_of_signals temp;
						temp.reserve(num_signals + 1);
						for (size_t j = 0; j < num_signals; ++j)
							temp.push_back(test_signal_pair(get_random_stock_name(max_stock_name_len), rand() % 1000));
						test_all_data_matrix.push_back(temp);
					}
				}

				//
				void multikey_tree_node_editor_thread( test_thread_root_def* root, const signal_matrix* test_all_data_matrix, const size_t cur_index )
				{
					for (test_vector_of_signals::const_iterator it = (*test_all_data_matrix)[cur_index].begin(); it != (*test_all_data_matrix)[cur_index].end(); ++it)
					{
						(*root).get_for_edit((*it).first.begin(), (*it).first.end())->add((*it).second);
					}
				}

				void multikey_tree_node_smoke_test_helper( const size_t number_type_of_thread, const size_t number_type_of_signals_on_thread, const size_t max_stock_name_len )
				{
					time_tracker tt;
					test_thread_root_def test_thread_root;
					signal_matrix test_all_data_matrix;

					test_init_data_matrix(test_all_data_matrix, number_type_of_thread, number_type_of_signals_on_thread, max_stock_name_len);

					boost::thread_group test_main_thread_group; 
					{
						for (size_t i = 0; i < number_type_of_thread; ++i)
							test_main_thread_group.create_thread( boost::bind( multikey_tree_node_editor_thread, &test_thread_root, &test_all_data_matrix, i)); 

						test_main_thread_group.join_all();
					}

					{
						std::map<std::string, size_t> check_map;
						for(std::vector<test_vector_of_signals>::iterator it = test_all_data_matrix.begin(); it != test_all_data_matrix.end(); ++it)
						{
							for (test_vector_of_signals::iterator it2 = (*it).begin(); it2 != (*it).end(); ++it2)
								check_map[(*it2).first] += (*it2).second;
						}
						
						for(std::map<std::string, size_t>::iterator it = check_map.begin(); it != check_map.end(); ++it)
						{
							std::string temp_str = (*it).first;
							BOOST_CHECK_EQUAL((test_thread_root.get_for_read(temp_str.begin(), temp_str.end()))->get_value(), (*it).second);
						}
					}
				}
			}


			void multikey_tree_node_smoke_test()
			{
				multikey_tree_node_smoke_test_helper( 10, 200, 18 );
			}
			void multikey_tree_with_map_comparator_tests()
			{
				static const size_t test_size = 1000;
				{
					time_tracker tt;
					multikey_tree< size_t > mkt;
					for ( size_t i = 0 ; i < test_size ; ++i )
						BOOST_CHECK_NO_THROW( (*mkt.get_for_edit( get_random_stock_name( 4 ) )) = i );
					for ( size_t i = 0 ; i < test_size ; ++i )
						BOOST_CHECK_NO_THROW( (*mkt.get_for_edit( get_random_stock_name( 4 ) )) = i );
				}
				{
					time_tracker tt;
					std::map< std::string, size_t > mt;
					for ( size_t i = 0 ; i < test_size ; ++i )
						mt[ get_random_stock_name( 4 ) ] = i;
					for ( size_t i = 0 ; i < test_size ; ++i )
						mt[ get_random_stock_name( 4 ) ] = i;
				}
			}
			//
			void multikey_tree_node_smoke_performance_big_name_test()
			{
				multikey_tree_node_smoke_test_helper( 10, 20000, 18 );
			}
			void multikey_tree_node_smoke_performance_many_signals_test()
			{
				multikey_tree_node_smoke_test_helper( 10, 500000, 4 );
			}
			void multikey_tree_node_smoke_performance_many_threads_test()
			{
				multikey_tree_node_smoke_test_helper( 50, 100000, 3 );
			}
		}
	}
}
