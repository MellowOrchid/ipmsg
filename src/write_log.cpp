#include <iostream>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <chrono>
#include "write_log.h"
using std::cerr, std::fstream, std::ios;

wlog::wlog() {}

wlog::~wlog() {}

void wlog::log(string message)
{
    // 获取当前系统时间点
    auto currentTime = std::chrono::system_clock::now();
    // 将时间点转换为 time_t（自 UTC 1970 年 1 月 1 日起的秒数）
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
    fstream fst;

    fst.open("log.log", ios::app);
    if (!fst.is_open())
    {
        cerr << "日志文件错误\n";
        return;
    }
    fst << std::put_time(std::localtime(&currentTime_t), "%Y-%m-%d %H:%M:%S") << '\t'
        << message << '\n';
    fst.close();
}

void wlog::log(const char *message)
{
    // 获取当前系统时间点
    auto currentTime = std::chrono::system_clock::now();
    // 将时间点转换为 time_t（自 UTC 1970 年 1 月 1 日起的秒数）
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
    fstream fst;

    fst.open("log.log", ios::app);
    if (!fst.is_open())
    {
        cerr << "日志文件错误\n";
        return;
    }
    fst << std::put_time(std::localtime(&currentTime_t), "%Y-%m-%d %H:%M:%S") << '\t';
    fst.write(message, strlen(message));
    fst << '\n';
    fst.close();
}

void wlog::log(int message)
{
    // 获取当前系统时间点
    auto currentTime = std::chrono::system_clock::now();
    // 将时间点转换为 time_t（自 UTC 1970 年 1 月 1 日起的秒数）
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
    fstream fst;

    fst.open("log.log", ios::app);
    if (!fst.is_open())
    {
        cerr << "日志文件错误\n";
        return;
    }
    fst << std::put_time(std::localtime(&currentTime_t), "%Y-%m-%d %H:%M:%S") << '\t'
        << message << '\n';
    fst.close();
}
