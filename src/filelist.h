#ifndef _FILELIST_H_
#define _FILELIST_H_

#include "file.h"

class filelist
{
private:
public:
    filelist();
    ~filelist();
    rcvfile find_file(string fname);
};

#endif
