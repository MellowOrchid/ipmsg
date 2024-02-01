#ifndef _KB_H_
#define _KB_H_

#include <list>
#include "user.h"
using std::list;

class keyboard
{
private:

public:
    keyboard();
    ~keyboard();
    void *kb_scan();
    void help_cmd();
    void users_cmd();
    void exit_cmd();
};

#endif
