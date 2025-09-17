#include <chrono>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <functional>

struct Timer {

    std::thread* Task = nullptr;
    void SetTimeOut(int t) {
        TimeOut = t;
    }
    void Start() {
        Increment = [this]() {
            //
            while (1) {
                std::this_thread::sleep_for(std::chrono::microseconds(1));
                this->Counter++;
                if (Counter > TimeOut) {
                    Counter = 0;
                    if (this->CallBack) {
                        this->CallBack();
                    }
                   // std::cout << "hello!\n";
                }
            }
        };
        if (!Task) {
            Task = new std::thread(Increment);
            Task->join();
        }
    }

    std::function<void(void)> CallBack;

protected:
    uint32_t TimeOut = 0.f;
    uint32_t Counter = 1000;

    std::function<void(void)> Increment;
};

struct Check {
    Timer t;
    void init() {
        t.CallBack = [this]() {
            std::cout << "This is U " << U << " of Check!!";
            };

        t.SetTimeOut(1000);
        t.Start();
    }
    double U = 1000.0;
};

std::map<std::string, std::string> g_pages;
std::mutex g_pages_mutex;

void save_page()
{
    // simulate a long page fetch
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::string result = "fake content";
   // while(1)
    //std::cout << result << "\n";
 //   std::lock_guard<std::mutex> guard(g_pages_mutex);
 //   g_pages[url] = result;
}

int main()
{
   // std::thread th(save_page);
  //  th.join();
   /* Timer t;
    t.CallBack = []() {
        std::cout << "This is callback function!!";
        };
    t.SetTimeOut(1000);
    t.Start();
    */
    Check ck;
    ck.init();
    while (1);
}