#ifndef MYLOG_H
#define MYLOG_H

#include <QString>

#include "spdlog/spdlog.h"
#include <memory>

class MyLog
{
public:
    static MyLog& getInstance();
    ~MyLog();
    void init(const std::string& file_name="./log/myYolo.log", size_t max_size=10 * 1024 * 1024, size_t max_files=10);
    static std::shared_ptr<spdlog::logger> getLogger();
private:
    MyLog();
    static std::shared_ptr<spdlog::logger> s_logger;
};

#define MY_LOG_TRACE(...)    MyLog::getInstance().getLogger()->trace(__VA_ARGS__)
#define MY_LOG_INFO(...)     MyLog::getInstance().getLogger()->info(__VA_ARGS__)
#define MY_LOG_WARN(...)     MyLog::getInstance().getLogger()->warn(__VA_ARGS__)
#define MY_LOG_ERROR(...)    MyLog::getInstance().getLogger()->error(__VA_ARGS__)
#define MY_LOG_CRITICAL(...) MyLog::getInstance().getLogger()->critical(__VA_ARGS__)

#endif // MYLOG_H
