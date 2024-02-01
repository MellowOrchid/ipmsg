#ifndef _CMD_H_
#define _CMD_H_

struct cmd
{
    char id[20];        // 包编号
    char name[20];      // 发送者姓名
    char hostname[20];  // 主机名
    unsigned int cmdid; // 命令字
    char buf[1024];     // 附加信息
};

// 译码，将接收到的字符串分解为 cmd 结构体
void transcode(cmd &obj, char buf[], int len);
// 编码，将要发送的信息编码为字符串保存到buf中
int coding(char *buf, unsigned int cmd, char *append);

#endif
