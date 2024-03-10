#ifndef _PUBLIC_H_
#define _PUBLIC_H_

#include <list>
#include <string>
#include "user.h"
#include "userlist.h"
#include "file.h"
#include "encode_convert.h"
using std::list, std::string;

#define BUFF_SIZE 128
#define MSG_PORT 2425
#define BUFFER_SIZE 1024
#define OFFLINE 939

extern char myname[20];
extern char hname[20];
extern char ip[20], br_ip[20];
extern string lmsg;
extern int utf_convert;

extern int udp_sock;
extern int tcp_sock;
extern sockaddr_in udp_sock_addr;
extern sockaddr_in tcp_sock_addr;

extern list<user> ulist;
extern list<sendfile> send_file_list;
extern list<rcvfile> receive_file_list;

#endif
