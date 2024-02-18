#include <iostream>
#include <fstream>
#include <filesystem>
#include "history.h"
#include "public.h"
#include "write_log.h"
using std::cerr, std::list, std::string, std::ios, std::fstream;
namespace fs = std::filesystem;

const string all_h_file = "all.txt";
const string history_dir = "./history/";
string uname;
fstream all_his, one_his;
list<string> all_users;

history::history() {}

history::~history() {}

list<string> history::get_all()
{
    ck_dir();

    all_his.open(history_dir + all_h_file, ios::in);
    if (!all_his.is_open())
    {
        lmsg = "用户名单：无法打开文件";
        wlog::log(lmsg);
        cerr << lmsg << "\n\n";
        // return all_users;
        all_his.close();
        all_his.open(history_dir + all_h_file, ios::out);
        all_his.close();
        all_his.open(history_dir + all_h_file, ios::in);
    }

    all_users.clear();
    while (all_his >> uname)
        all_users.push_back(uname);

    all_his.close();
    return all_users;
}

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

bool history::has_user(string name)
{
    ck_dir();

    get_all();
    for (auto &&i : all_users)
        if (i == name)
            return true;
    return false;
}

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
