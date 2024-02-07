#include "file.h"

sendfile::sendfile() {}
sendfile::~sendfile() {}

bool sendfile::operator==(const sendfile &other) const
{
    return other.pkgnum == pkgnum;
}

rcvfile::rcvfile() {}
rcvfile::~rcvfile() {}
