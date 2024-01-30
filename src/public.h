#ifndef _PUBLIC_H_
#define _PUBLIC_H_

#define BUFF_SIZE 128
#define MSG_PORT 2425
#define BUFFER_SIZE 1024

extern char myname[20];

extern int udp_sock;
extern int tcp_sock;
extern struct sockaddr_in udp_sock_addr;
extern struct sockaddr_in tcp_sock_addr;

#endif
