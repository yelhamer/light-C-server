#include "../headers/networking.h"


// init_server: creates socket (endpoint) for connection to <hostname> and <port>
// PARAMS:
// 	    port:	string of port
// 	  socket: 	ptr to where to save the socket structure
// 	hostname:	string of hostname (domaine name, ip address)
// RETURN:
// 	 0: success
// 	-1: error
//
int init_server(socket_t *s, char *hostname, char *port) {
	int rc;
	struct addrinfo hint = { 0, AF_INET, SOCK_STREAM, 0, 0, NULL, NULL, NULL };


	// allocate and init the server socket's addrinfo struct
	if((s->addrinfo = calloc(1, sizeof (struct addrinfo))) == NULL) {
		perror("malloc() failed");
		exit(1);
	}

	// fill-out the addrinfo struct with the provided address
	rc = getaddrinfo(hostname, port, &hint, &s->addrinfo);
	if(rc == -1) {
		printf("%d\nerrno:%d\n", -1, errno);
		return -1;
	}

	// create a ipv4 tcp socket
	rc = s->fd = socket(AF_INET, SOCK_STREAM, 0);
	if(rc == -1) {
		printf("%d\nerrno:%d\n", -1, errno);
		return -1;
	}

	// bind socket to the provided address
	rc = bind(s->fd, s->addrinfo->ai_addr, sizeof(struct sockaddr));
	if(rc == -1) {
		return -1;
	}
	
	return 0;
}

// start_server: starts listening
// PARAMS:
// 	      s: ptr to the server's socket structure
// 	backlog: length of the pending connections' queue (maximum number of concurent connections)
// RETURN:
// 	 0: success
// 	-1: error
//
int start_server(socket_t *s, int backlog) {
	int rc;


	// start listening on the provided socket
	rc = listen(s->fd, backlog);
	if(rc == -1) {
		return -1;
	}

	return 0;
}

// close_connection: closes connection
// PARAMS:
// 	fd: file descriptor of the connected socket
// RETURN:
// 	 0: success
// 	-1: error
//
int close_connection(int fd) {
	int rc;


	// terminate the socket
	rc = close(fd);
	if(rc == -1) {
		return -1;
	}

	return 0;
}

// send_msg: sends message to connected socket
// PARAMS:
// 	   fd:  file descriptor of the connected socket
// 	  buf: message to send
// 	count: maximum message size
// RETURN:
// 	 0: success
// 	-1: error
//
int send_msg(int fd, char *buf, size_t count) {
	int rc;

	
	// write buffer's content into socket
	rc = write(fd, buf, count);
	if(rc == -1) {
		return -1;
	}

	return 0;
}

// recv_msg: receives message from connected socket
// PARAMS:
// 	 fd:  file descriptor of the connected socket
// 	buf: message to send
// RETURN:
// 	number of characters received
//
int recv_msg(int fd, char *buf) {
	// read message
	return read(fd, buf, BUFFERSIZE);
}
