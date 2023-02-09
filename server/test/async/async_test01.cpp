#include <iostream>   
#include <functional>     // std::ref
#include <thread>         // std::thread
#include <future>         // std::promise, std::future

namespace ASYNC_TEST01
{



std::promise<int> prom;
void print_int()
{
    std::future<int> fut = prom.get_future();
    int v = fut.get();
    std::cout << "value: " << v << std::endl;
    
}

void get_int(std::promise<int>& prom) {
    int x;
    std::cout << "Please, enter an integer value: ";
    std::cin.exceptions (std::ios::failbit);   // throw on failbit
    try {
        std::cin >> x;                         // sets failbit if input is not int
        prom.set_value(x);
    } catch (std::exception&) {
        prom.set_exception(std::current_exception());
    }
}

void print_int1(std::future<int>& fut) {
    try {
        int x = fut.get();
        std::cout << "value: " << x << '\n';
    } catch (std::exception& e) {
        std::cout << "[exception caught: " << e.what() << "]\n";
    }
}

}

using namespace ASYNC_TEST01;
int async_test01()
{
    std::thread t(print_int);
    prom.set_value(10);

    t.join();

    prom = std::promise<int>();
    std::thread t1(print_int);
    prom.set_value(20);

    std::promise<int> prom1;
    std::future<int> fut = prom1.get_future();

    std::thread th1(get_int, std::ref(prom1));
    std::thread th2(print_int1, std::ref(fut));

    th1.join();
    th2.join();

    t1.join();


    return 0;
}