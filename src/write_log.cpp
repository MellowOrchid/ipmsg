#include <iostream>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <chrono>
#include "write_log.h"
#include <mutex>
using std::cerr, std::string, std::fstream, std::ios, std::mutex;

const string log_file = "log.log";
fstream fst;
mutex mtx; // 全局互斥锁

wlog::wlog() {}

wlog::~wlog() {}

/**
 * 记录日志
 * @param message 要记录的内容
 */
void wlog::log(string message)
{
    std::lock_guard<std::mutex> lock(mtx); // 获取锁

    // 获取当前系统时间点
    auto currentTime = std::chrono::system_clock::now();
    // 将时间点转换为 time_t（自 UTC 1970 年 1 月 1 日起的秒数）
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);

    fst.open(log_file, ios::app);
    if (!fst.is_open())
    {
        cerr << "日志文件错误\n";
        return;
    }
    fst << std::put_time(std::localtime(&currentTime_t), "%Y-%m-%d %H:%M:%S") << '\t'
        << message << '\n';
    fst.close();
}

/**
 * 记录日志
 * @param message 要记录的内容
 */
void wlog::log(const char *message)
{
    std::lock_guard<std::mutex> lock(mtx); // 获取锁

    // 获取当前系统时间点
    auto currentTime = std::chrono::system_clock::now();
    // 将时间点转换为 time_t（自 UTC 1970 年 1 月 1 日起的秒数）
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);

    fst.open(log_file, ios::app);
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

/**
 * 记录日志
 * @param message 要记录的内容
 * @deprecated 为使数据在同一行上，改用 log(std::string, int);
 */
void wlog::log(int message)
{
    std::lock_guard<std::mutex> lock(mtx); // 获取锁

    // 获取当前系统时间点
    auto currentTime = std::chrono::system_clock::now();
    // 将时间点转换为 time_t（自 UTC 1970 年 1 月 1 日起的秒数）
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);

    fst.open(log_file, ios::app);
    if (!fst.is_open())
    {
        cerr << "日志文件错误\n";
        return;
    }
    fst << std::put_time(std::localtime(&currentTime_t), "%Y-%m-%d %H:%M:%S") << '\t'
        << message << '\n';
    fst.close();
}

/**
 * 记录日志
 * @param message 要记录的内容
 * @param append 追加记录的内容
 */
void wlog::log(string message, int append)
{
    std::lock_guard<std::mutex> lock(mtx); // 获取锁

    // 获取当前系统时间点
    auto currentTime = std::chrono::system_clock::now();
    // 将时间点转换为 time_t（自 UTC 1970 年 1 月 1 日起的秒数）
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);

    fst.open(log_file, ios::app);
    if (!fst.is_open())
    {
        cerr << "日志文件错误\n";
        return;
    }
    fst << std::put_time(std::localtime(&currentTime_t), "%Y-%m-%d %H:%M:%S") << '\t'
        << message << append << '\n';
    fst.close();
}
