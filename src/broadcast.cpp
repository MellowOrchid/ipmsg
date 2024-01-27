#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#define IPMSG_BR_ENTRY 0x00000001UL
using std::cin, std::cout, std::string, std::cerr;

class broadcast
{
private:
    int sockfd, port, optval;
    struct sockaddr_in servaddr;
    string ip;
    char name[10];
    char buffer[1024];

public:
    broadcast();
    ~broadcast();
    void send(const string &message);
    void coding(char *buffer, unsigned int cmd, char *append);
    void bc();
};

broadcast::broadcast()
{
    port = 2425;

    cout << "本机 IP：";
    cin >> ip;

    cout << "用户名：";
    cin >> name;
}

broadcast::~broadcast()
{
    close(sockfd);
}

/** 发送广播信息 */
void broadcast::send(const string &message)
{
    int result = sendto(sockfd, message.c_str(), message.size(), 0,
                        (struct sockaddr *)&servaddr, sizeof(servaddr));
    cout << "套接字发送结果：" << result << '\n';
}

/** 合并信息 */
void broadcast::coding(char *buffer, unsigned int cmd, char *append)
{
    char hname[20];
    time_t h;

    time(&h);
    gethostname(hname, sizeof(hname));
    if (append == NULL)
        *append = '0';

    sprintf(buffer, "1:%ld:%s:%s:%d:%s", h, append, hname, cmd, append);
    cout << "合并后信息：" << buffer << '\n';
}

/** 执行广播 */
void broadcast::bc()
{
    optval = 1;

    cout << "端口号为：" << port << '\n';

    // 创建套接字
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        cerr << "未能创建套接字\n";
        exit(1);
    }

    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval));

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(ip.c_str());

    coding(buffer, IPMSG_BR_ENTRY, name);
    send(buffer);
}
