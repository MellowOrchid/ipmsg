#include <iostream>
#include <cstring>
#include "keyboard.cpp"
#include "broadcast.cpp"
using std::cin, std::cout, std::string;

int main()
{
    keyboard kb;
    broadcast broadcast;

    broadcast.bc();
    kb.help_cmd();
    kb.kb_scan();
    // system("pause");
    return 0;
}
