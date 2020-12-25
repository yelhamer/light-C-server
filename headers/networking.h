#ifndef __networking1af8
#define __networking1af8

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


// Macros
#define MAX_CONNECTIONS (100)
#define BUFFERSIZE 	(BUFSIZ)
#define HOSTLEN		(256)
#define PORTLEN		(32)


// Typedefs
typedef struct __socket {
	int fd;
	struct addrinfo *addrinfo;
} socket_t;

typedef struct __connection {
	int fd;
	struct sockaddr_in addr;
	socklen_t addrlen;
} connection_t;


// Functions' Declarations
int init_server(socket_t *, char *, char *);
int start_server(socket_t *, int);
int close_connection(int);
int send_msg(int, char *, size_t);
int recv_msg(int, char *);
int action(int, char *, char *, size_t); 


#endif //__networking1af8
