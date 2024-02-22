#include "file.h"

sendfile::sendfile() {}
sendfile::~sendfile() {}

/** 根据文件的包编号来判断两个文件是否相同 */
bool sendfile::operator==(const sendfile &other) const
{
    return other.pkgnum == pkgnum;
}

rcvfile::rcvfile() {}
rcvfile::~rcvfile() {}
