#ifndef _BC_H_
#define _BC_H_

#include <string>
#include <netinet/in.h>
using std::string;
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

#endif
