#ifndef MY_HTEADER_GUARD

/*================ include ================*/
#include <thread> // C++ Concurrency Library for thread's management
#include <mutex> // std::mutex & std::mutex_guard
#include <condition_variable> // condition_variable & condition_variable_any
#include <future> // future & async
#include <shared_mutex> // std::shared_mutex

#include <iostream>

#include <string>
#include <cstring>
#include <algorithm>    // for_each
#include <ratio>    // class ratio

#include <memory> // unique_ptr
#include <exception>

#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <type_traits> // result_of
#include <numeric> // accumulate
#include <functional>   // mem_fn
#include <utility>
#include <chrono>   // std::chrono, time point, time duration
#include <exception>    // copy_exception renamed to make_exception_ptr
#include <iomanip>   // put_time

#include <cmath>

#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

#include <climits>
#include <ctime>
/*================ include end ================*/


/*================ define ================*/
#define PRINT_BEGIN std::cout << "|------ begin ------|" << std::endl;
#define PRINT_END   std::cout << "|------- end -------|" << std::endl;
/*================ define end ================*/

#define MY_HTEADER_GUARD
#endif 