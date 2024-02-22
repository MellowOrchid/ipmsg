#include "file.h"
#include "filelist.h"
#include "public.h"

filelist::filelist() {}
filelist::~filelist() {}

/**
 * 在收到的文件中检索目标文件
 * @param fname 要检索的文件名，包含扩展名
 * @return 一个 `rcvfile` 的对象，若没有匹配，则返回其空对象
 */
rcvfile filelist::find_file(string fname)
{
    rcvfile dfile;

    for (auto &&i : receive_file_list)
        if (i.name == fname)
        {
            dfile = i;
            break;
        }
    return dfile;
}
