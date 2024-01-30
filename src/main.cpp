#include <iostream>
#include <cstring>
#include <thread>
#include "keyboard.cpp"
#include "broadcast.cpp"
using std::cin, std::cout, std::string, std::thread;

int main()
{
    keyboard kb;
    broadcast broadcast;

    broadcast.bc();
    kb.help_cmd();
    thread kb_thrd(&keyboard::kb_scan, &kb);

    kb_thrd.join();
    // system("pause");
    return 0;
}
