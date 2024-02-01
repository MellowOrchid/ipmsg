#include <iostream>
#include <unistd.h>
#include <time.h>
#include "IPMSG.H"
#include "public.h"
#include "pack_unpack.h"

char myname[20];

// 将接收到的字符串分解为 cmd 结构体
void transcode(struct cmd *object, char *buffer, int len)
{
    sscanf(buffer, "1:%[^:]:%[^:]:%[^:]:%d:%[^\n]", object->id, object->name, object->hostname, &object->cmdid, object->buf);
}

// 要发送的信息编码为字符串保存到buf中
int coding(char *buffer, unsigned int cmd, char *append)
{
    char hname[20];
    time_t h;
    time(&h);
    gethostname(hname, sizeof(hname));
    if (append == NULL)
        *append = '0';
    sprintf(buffer, "1:%ld:%s:%s:%d:%s", h, myname, hname, cmd, append);

    return 0;
}
