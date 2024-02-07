#include <iostream>
#include <unistd.h>
#include <fstream>
#include "IPMSG.H"
#include "pack_unpack.h"
#include "userlist.h"
#include "filelist.h"
#include "public.h"
#include "tcp_send_file.h"
#include "write_log.h"
using std::cout, std::cerr, std::ifstream;

// 发送文件
void tcp_send_file()
{
    int cnct_sockt; // 连接套接字
    int recvbytes;
    socklen_t len;
    char rcv[1024];
    char recvbuf[BUFF_SIZE];
    char sendbuf[BUFFER_SIZE];
    ifstream ifs;
    sockaddr_in serv_addr, cli_addr;
    cmd cmd_obj;
    sendfile sdfile;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(MSG_PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(tcp_sock, (sockaddr *)&serv_addr, sizeof(sockaddr_in)) < 0)
    {
        lmsg = "未能绑定 TCP 套接字";
        wlog::log(lmsg);
        cerr << lmsg << '\n';
        exit(2);
    }

    if (listen(tcp_sock, 10))
    {
        lmsg = "TCP 监听错误";
        wlog::log(lmsg);
        cerr << lmsg << '\n';
        exit(2);
    }

    while (1)
    {
        cnct_sockt = accept(tcp_sock, (sockaddr *)&tcp_sock_addr, &len);
        lmsg = "TCP 连接成功";
        wlog::log(lmsg);
        cout << '\n' << lmsg << '\n';

        recvbytes = recv(cnct_sockt, recvbuf, sizeof(recvbuf), 0);
        recvbuf[recvbytes] = '\003';
        recvbuf[recvbytes + 1] = 0;
        lmsg = "收到 TCP 消息：";
        wlog::log(lmsg);
        wlog::log(recvbuf);

        char version[50];
        sscanf(recvbuf, "%[^:]:%[^:]:%[^:]:%[^:]:%d:%[^\003]", version, rcv, cmd_obj.name, cmd_obj.hostname, &cmd_obj.cmdid, cmd_obj.buf);

        if (GET_MODE(cmd_obj.cmdid) == IPMSG_GETFILEDATA)
        {
            sscanf(cmd_obj.buf, "%lx:%ld", &sdfile.pkgnum, &sdfile.num);
            for (auto &&i : send_file_list)
                if (i.pkgnum == sdfile.pkgnum)
                {
                    sdfile = i;
                    break;
                }

            ifs.open(sdfile.name);
            if (!ifs.is_open())
            {
                lmsg = "未找到文件：";
                wlog::log(lmsg);
                wlog::log(sdfile.name);
                cerr << lmsg << sdfile.name << "\n\n";
                continue;
            }
            bzero(sendbuf, BUFFER_SIZE); // 缓冲区清零

            // 从文件读取数据并发送到客户端
            while (!ifs.eof())
            {
                ifs.read(sendbuf, BUFFER_SIZE);
                send(cnct_sockt, sendbuf, ifs.gcount(), 0);
            }

            send_file_list.remove(sdfile);
            ifs.close();

            lmsg = "发送文件完成：";
            wlog::log(lmsg);
            wlog::log(sdfile.name);
            cout << lmsg << sdfile.name << "\n\n"
                 << "请继续写：" << std::flush;
            close(cnct_sockt);
        }
    }
}
