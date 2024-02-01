#include <list>
#include "user.h"
#include "userlist.h"
using std::list;

#ifndef _PUBLIC_H_
#define _PUBLIC_H_

#define BUFF_SIZE 128
#define MSG_PORT 2425
#define BUFFER_SIZE 1024

extern char myname[20];

extern int udp_sock;
extern int tcp_sock;
extern sockaddr_in udp_sock_addr;
extern sockaddr_in tcp_sock_addr;

extern list<user> ulist;
// extern list<user>::iterator uiter;

#endif
