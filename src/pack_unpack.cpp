#include <iostream>
#include <unistd.h>
#include <time.h>
#include "IPMSG.H"
#include "public.h"
#include "pack_unpack.h"

cmd::cmd() {}
cmd::~cmd() {}

// 将接收到的字符串分解为 cmd 结构体
void cmd::transcode(cmd &object, char buffer[])
{
    char version[50];
    sscanf(buffer, "%[^:]:%[^:]:%[^:]:%[^:]:%d:%[^\003]",
           version, object.id, object.name, object.hostname, &object.cmdid, object.buf);
}

// 要发送的信息编码为字符串保存到buf中
int cmd::coding(char *buffer, unsigned int cmd, char *append)
{
    char hname[20];
    time_t h;
    time(&h);
    gethostname(hname, sizeof(hname));
    sprintf(buffer, "1:%ld:%s:%s:%d:%s", h, myname, hname, cmd, append);

    return 0;
}
