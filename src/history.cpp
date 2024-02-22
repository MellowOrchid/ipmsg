#include <iostream>
#include <fstream>
#include <filesystem>
#include "history.h"
#include "public.h"
#include "write_log.h"
using std::cout, std::cerr, std::list, std::string, std::ios, std::fstream;
namespace fs = std::filesystem;

const string all_h_file = "all.txt";
const string history_dir = "./history/";
string uname, his_message;
fstream all_his, one_his;
list<string> all_users;

history::history() {}

history::~history() {}

/**
 * 读取所有历史记录名单中的用户名
 * @return 以 `std::string` 为内容的 `std::list`
 */
list<string> history::get_all()
{
    ck_dir();
    all_users.clear();

    all_his.open(history_dir + all_h_file, ios::in);

    // 先尝试创建文件
    if (!all_his.is_open())
    {
        all_his.close();
        all_his.open(history_dir + all_h_file, ios::out);
        all_his.close();
        all_his.open(history_dir + all_h_file, ios::in);
    }
    // 如果仍无法打开则报错
    if (!all_his.is_open())
    {
        lmsg = "用户名单：无法打开文件";
        wlog::log(lmsg);
        cerr << lmsg << "\n\n";
        return all_users;
    }

    while (all_his >> uname)
        all_users.push_back(uname);

    all_his.close();
    return all_users;
}

/**
 * 追加用户到历史记录名单
 * @param name 要追加的用户名
 */
void history::app_user(string name)
{
    ck_dir();

    all_his.open(history_dir + all_h_file, ios::app);
    if (!all_his.is_open())
    {
        lmsg = "用户名单：无法写入文件";
        wlog::log(lmsg);
        cerr << lmsg << "\n\n";
        return;
    }

    all_his << name << '\n';
    all_his.close();
}

/**
 * 追加用户到历史记录名单
 * @param name 要追加的用户名
 */
void history::app_user(char *name)
{
    ck_dir();

    all_his.open(history_dir + all_h_file, ios::app);
    if (!all_his.is_open())
    {
        lmsg = "用户名单：无法写入文件";
        wlog::log(lmsg);
        cerr << lmsg << "\n\n";
        return;
    }

    all_his << name << '\n';
    all_his.close();
}

/**
 * 检查某用户是否在历史记录名单中
 * @param name 目标用户名
 * @return 若该用户存在，则返回真
 */
bool history::has_user(string name)
{
    ck_dir();

    get_all();
    for (auto &&i : all_users)
        if (i == name)
            return true;
    return false;
}

/**
 * 检查某用户是否在历史记录名单中
 * @param name 目标用户名
 * @return 若该用户存在，则返回真
 */
bool history::has_user(char *name)
{
    ck_dir();

    get_all();
    for (auto &&i : all_users)
        if (!strcmp(name, i.c_str()))
            return true;
    return false;
}

/**
 * @param op_side 与谁交流
 * @param who_said 谁说的话
 * @param message 说的什么
 */
void history::write_history(string op_side, char *who_said, char *message)
{
    // 获取当前系统时间点
    auto currentTime = std::chrono::system_clock::now();
    // 将时间点转换为 time_t（自 UTC 1970 年 1 月 1 日起的秒数）
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);

    string dest_file = history_dir + op_side + ".txt";
    one_his.open(dest_file, ios::app);
    if (!one_his.is_open())
    {
        lmsg = "写入历史记录失败";
        wlog::log(lmsg);
        cerr << lmsg << "\n\n";
        return;
    }

    one_his << std::put_time(std::localtime(&currentTime_t), "%Y-%m-%d %H:%M:%S") << '\t'
            << who_said << ": " << message << '\n';
    one_his.close();
}

/**
 * @param op_side 与谁交流
 * @param who_said 谁说的话
 * @param message 说的什么
 */
void history::write_history(char *op_side, char *who_said, char *message)
{
    // 获取当前系统时间点
    auto currentTime = std::chrono::system_clock::now();
    // 将时间点转换为 time_t（自 UTC 1970 年 1 月 1 日起的秒数）
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);

    string dest_file = history_dir + op_side + ".txt";
    one_his.open(dest_file, ios::app);
    if (!one_his.is_open())
    {
        lmsg = "写入历史记录失败";
        wlog::log(lmsg);
        cerr << lmsg << "\n\n";
        return;
    }

    one_his << std::put_time(std::localtime(&currentTime_t), "%Y-%m-%d %H:%M:%S") << '\t'
            << who_said << ": " << message << '\n';
    one_his.close();
}

/**
 * @param op_side 与谁交流
 * @param who_said 谁说的话
 * @param message 说的什么
 * @param app 附加信息
 */
void history::write_history(string op_side, char *who_said, string message, string app)
{
    // 获取当前系统时间点
    auto currentTime = std::chrono::system_clock::now();
    // 将时间点转换为 time_t（自 UTC 1970 年 1 月 1 日起的秒数）
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);

    string dest_file = history_dir + op_side + ".txt";
    one_his.open(dest_file, ios::app);
    if (!one_his.is_open())
    {
        lmsg = "写入历史记录失败";
        wlog::log(lmsg);
        cerr << lmsg << "\n\n";
        return;
    }

    one_his << std::put_time(std::localtime(&currentTime_t), "%Y-%m-%d %H:%M:%S") << '\t'
            << who_said << ": " << message << app << '\n';
    one_his.close();
}

/**
 * @param op_side 与谁交流
 * @param who_said 谁说的话
 * @param message 说的什么
 * @param app 附加信息
 */
void history::write_history(char *op_side, char *who_said, string message, string app)
{
    // 获取当前系统时间点
    auto currentTime = std::chrono::system_clock::now();
    // 将时间点转换为 time_t（自 UTC 1970 年 1 月 1 日起的秒数）
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);

    string dest_file = history_dir + op_side + ".txt";
    one_his.open(dest_file, ios::app);
    if (!one_his.is_open())
    {
        lmsg = "写入历史记录失败";
        wlog::log(lmsg);
        cerr << lmsg << "\n\n";
        return;
    }

    one_his << std::put_time(std::localtime(&currentTime_t), "%Y-%m-%d %H:%M:%S") << '\t'
            << who_said << ": " << message << app << '\n';
    one_his.close();
}

/**
 * 从文件中读取聊天记录
 * @param name 目标用户名
 */
void history::read_history(string name)
{
    string dest_file = history_dir + name + ".txt";
    one_his.open(dest_file, ios::in);

    if (!one_his.is_open())
    {
        lmsg = "聊天记录：没有记录或无法打开";
        wlog::log(lmsg);
        cerr << lmsg << "\n\n";
        return;
    }

    while (getline(one_his, his_message))
        cout << his_message << '\n';

    one_his.close();
}

/** 检查 `history` 目录是否存在*/
void history::ck_dir()
{
    // 检查目录是否存在
    if (fs::exists(history_dir))
        return;
    lmsg = "没有 `history` 这个目录";
    wlog::log(lmsg);

    // 目录不存在，创建目录
    try
    {
        fs::create_directory(history_dir);
        lmsg = "新建成功";
        wlog::log(lmsg);
    }
    catch (const fs::filesystem_error &e)
    {
        lmsg = "创建目录失败";
        wlog::log(lmsg);
        wlog::log(e.what());
        cerr << lmsg << "：" << e.what() << '\n';
    }
}
