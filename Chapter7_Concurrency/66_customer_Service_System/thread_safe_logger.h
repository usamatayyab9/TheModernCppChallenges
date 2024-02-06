#include <iostream>
#include <mutex>
#include <string_view>

using std::endl;
using std::lock_guard;
using std::mutex;
using std::ostream;
using std::string_view;

class Logger_mt
{
public:
    Logger_mt(ostream &output_stream_arg) : output_stream_{ output_stream_arg }
    {
    }
    void Log(string_view data)
    {
        auto guard = lock_guard{ mutex_ };
        output_stream_ << data << endl;
    }
private:
    Logger_mt() = delete;
    /* mutex is non-copyable and non-assignable so logger class should also
        be non-copyable and non-assignable
    */
    Logger_mt(const Logger_mt &) = delete; 
    mutex    mutex_;
    ostream& output_stream_;
};
