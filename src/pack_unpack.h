#ifndef _CMD_H_
#define _CMD_H_

class cmd
{
private:
public:
    unsigned int cmdid; // 命令字
    char id[20];        // 包编号
    char name[50];      // 发送者姓名
    char hostname[20];  // 主机名
    char buf[1024];     // 附加信息
    cmd();
    ~cmd();
    static void transcode(cmd &obj, char *buf);
    static void coding(char *buf, unsigned cmd, char *append);
};

#endif
