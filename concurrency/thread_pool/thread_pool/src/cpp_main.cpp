#include <cpp_thread_pool.h>


std::string LogPrefix()
{
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()).count();
	time_t unix_tm = ms/1000;

	char buf[64] = {0};
	struct tm t;
	localtime_r(&unix_tm, &t);
	std::snprintf(buf, sizeof(buf), "[%04d-%02d-%02d %02d:%02d:%02d.%04d] [%u] ",
			t.tm_year + 1900,
			t.tm_mon + 1,
			t.tm_mday,
			t.tm_hour,
			t.tm_min,
			t.tm_sec,
			ms%1000,
			std::this_thread::get_id());

	return buf;
}

//互斥量，用来打日志，避免日志混乱
std::mutex g_log_mutex;


//测试对象回调
class Num
{
public:
    Num(uint32_t m):max_(m){};
    uint64_t summary() //模拟某种复杂计算
    {
        std::cout << LogPrefix() << "member function starting ~~~~~~~\n";
        uint64_t sum = 0;
        for(uint32_t i = 0; i < max_; i++)
        {
            sum += i;
        }
        std::cout << LogPrefix() << "sum 1 to " << max_ << " = " << sum << "\n";
        std::cout << LogPrefix() << "member function ending ~~~~~~~\n";
        return sum;
    }
private:
    const uint32_t max_;
};

//使用一般方式回调
void hello()
{
    std::cout << LogPrefix() << "normal function starting ~~~~~~~\n";
    std::cout << LogPrefix() << "Hello,World!😭😭😭😭 \n";
    std::cout << LogPrefix() << "norma; function ending ~~~~~~~\n";
}

//使用lambda方式回调
void lambda(int n)
{
    std::cout << LogPrefix() << "lambda function starting ~~~~~~~\n";
    int res = 0;
    for(int i = 1; i <= n; i ++ )
        res += i;
    std::cout << LogPrefix() << "Res of 1 from n = " << res << "\n";
    std::cout << LogPrefix() << "lambda function ending ~~~~~~~\n";
}

//延时任务
void delay(int id)
{
    std::cout << LogPrefix() << "delay function starting ~~~~~~~\n";
    std::cout << LogPrefix() << "I will sleep 1s, and my ID is " << id << "\n";
    sleep(1);
    std::cout << LogPrefix() << "I sleep down 😊\n";
    std::cout << LogPrefix() << "delay function ending ~~~~~~~\n";
}

int main()
{
    PRINT_BEGIN;
    srand(time(NULL));

    //生成一个线程池，包含4个线程
    auto pool = std::make_shared<ThreadPool>(4);

    //正常执行的任务
    pool->push_task(hello);
    //延时任务
    pool->push_delay_task(1000, [](){delay(0);});
    //lambda表达式
    pool->push_task([](){lambda(999);});

    // 执行成员函数的任务
    Num n(100);
    //future任务，等待返回值
    auto return_future = std::move(pool->push_future_task(std::mem_fn(&Num::summary), &n));
    //等待future的执行结果，注意如果函数有异常，会在get()函数中抛出，这是C++11的标准决定的
    std::cout << LogPrefix() << "Waiting for summary future task..." << std::endl;
    try
    {
        uint64_t sum_val = return_future.get();
        std::cout << LogPrefix() << LogPrefix() << "Summary result: "<<sum_val << std::endl;
    }
    catch(...)
    {
        std::cout << LogPrefix() << LogPrefix() << "catch exception in future::get()\n";   
    }

    sleep(3);
    //销毁线程池
    std::cout << LogPrefix() << "Destroying thread pool..."<<std::endl;
    //等待所有[定时任务以外]的线程完成
    pool->stop();

    PRINT_END;
    return 0;
}