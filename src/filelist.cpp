#include "file.h"
#include "filelist.h"
#include "public.h"

filelist::filelist() {}
filelist::~filelist() {}

rcvfile filelist::find_file(string fname)
{
    rcvfile dfile;

    for (auto &&i : receive_file_list)
        if (i.name == fname)
            dfile = i;

    return dfile;
}
