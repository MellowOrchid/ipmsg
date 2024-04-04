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
#include "history.h"

using std::cout, std::cerr, std::ifstream;

tcp_send_file::tcp_send_file() {}

tcp_send_file::~tcp_send_file() {}

/** 发送文件 */
void tcp_send_file::send_f()
{
    int cnct_sockt; // 连接套接字
    int recvbytes;
    char rcv[1024];
    char recvbuf[BUFF_SIZE];
    char sendbuf[BUFFER_SIZE];
    socklen_t len;
    ifstream ifs;
    cmd cmd_obj;
    sendfile sdfile;

    while (true)
    {
        cnct_sockt = accept(tcp_sock, (sockaddr *)&tcp_sock_addr, &len);
        lmsg = "TCP 连接成功";
        wlog::log(lmsg);
        cout << '\n'
             << lmsg << '\n';

        recvbytes = recv(cnct_sockt, recvbuf, sizeof(recvbuf), 0);
        recvbuf[recvbytes] = '\003';
        recvbuf[recvbytes + 1] = 0;
        lmsg = "收到 TCP 消息：";
        wlog::log(lmsg);
        wlog::log(recvbuf);

        char version[50];
        sscanf(recvbuf, "%[^:]:%[^:]:%[^:]:%[^:]:%d:%[^\003]", version, rcv,
               cmd_obj.name, cmd_obj.hostname, &cmd_obj.cmdid, cmd_obj.buf);

        // 发文件
        if (GET_MODE(cmd_obj.cmdid) == IPMSG_GETFILEDATA)
        {
            sscanf(cmd_obj.buf, "%lx:%ld", &sdfile.pkgnum, &sdfile.num);
            for (auto &&i : send_file_list)
                if (i.pkgnum == sdfile.pkgnum)
                {
                    sdfile = i;
                    break;
                }

            ifs.open(sdfile.path);
            if (!ifs.is_open())
            {
                lmsg = "未找到文件：";
                wlog::log(lmsg);
                wlog::log(sdfile.name);
                cerr << lmsg << sdfile.name << "\n\n";
                continue;
            }
            bzero(sendbuf, BUFFER_SIZE); // 缓冲区清零

            lmsg = "正在发送文件";
            wlog::log(lmsg);
            cout << lmsg << '\n';
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

            history::write_history(cmd_obj.name, myname, lmsg, sdfile.name);
        }

        if (cmd_obj.cmdid == OFFLINE)
            break;
    }
    lmsg = "TCP 监听结束";
    wlog::log(lmsg);
    cout << lmsg << '\n';
}
