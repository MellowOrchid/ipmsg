#ifndef _HIS_H_
#define _HIS_H_

#include <list>
using std::list, std::string;

class history
{
private:
    static void ck_dir();

public:
    history();
    ~history();
    static list<string> get_all();
    static void app_user(string name);
    static void app_user(char *name);
    static bool has_user(string name);
    static bool has_user(char *name);
};

#endif
