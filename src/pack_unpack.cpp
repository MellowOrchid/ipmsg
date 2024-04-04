#include <iostream>
#include <unistd.h>
#include <time.h>
#include "IPMSG.H"
#include "public.h"
#include "pack_unpack.h"

cmd::cmd() {}
cmd::~cmd() {}

/**
 * 将接收到的字符串分解为 `cmd` 的对象
 * @param object 目标 `cmd` 对象
 * @param buffer 接收到的字符串指针
 */
void cmd::transcode(cmd &object, char *buffer)
{
    char version[50];
    sscanf(buffer, "%[^:]:%[^:]:%[^:]:%[^:]:%d:%[^\003]", version,
           object.id, object.name, object.hostname, &object.cmdid, object.buf);
}

/**
 * 将要发送的信息编码为字符串
 * @param buffer 目标字符串指针
 * @param cmd IPMSG 协议中的指令代号
 * @param append 附加信息
 */
void cmd::coding(char *buffer, unsigned cmd, const char *append)
{
    time_t h;
    time(&h);
    sprintf(buffer, "1:%ld:%s:%s:%d:%s", h, myname, hname, cmd, append);
}
