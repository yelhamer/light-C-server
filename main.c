#include "headers/networking.h"

// Main
int main(int argc, char **argv) {
	int		i = 0, rc = 0, max_connections = 5;
	char		hostname[HOSTLEN]; // 256
	char		port[PORTLEN]; //32
	char		buf[BUFFERSIZE];
	char		*remote_prompt = "file: ";
	socket_t 	server;
	connection_t	open_connections[MAX_CONNECTIONS]; // 100
	struct addrinfo addrinfo;


	// nullify buffers and structs
	memset(buf, 0, sizeof(buf));
	memset(hostname, 0, sizeof(hostname));
	memset(port, 0, sizeof(port));
	memset(&server, 0, sizeof(server));
	memset(&addrinfo, 0, sizeof(addrinfo));
	memset(&open_connections, -1, sizeof(open_connections));


	// parse cli-args
	if(argc != 3) {
		perror("Incorrect Usage");
		printf("Usage: %s <localaddress> <port>\n", argv[0]);
		exit(1);
	}

	strncpy(hostname, argv[1], HOSTLEN);
	strncpy(port, argv[2], PORTLEN);


	// initialize server socket
	rc = init_server(&server, hostname, port);
	if(rc == -1){
		perror("[-] init_server failed");
		exit(1);
	}

	
	// start listening for connections
	rc = start_server(&server, max_connections);
	if(rc == -1){
		perror("[-] start_server() failed");
		exit(1);
	}


	// listening loop
	while(1) {
		int maxfds;	// biggest file descriptor (for select)
		fd_set readfds; // the fds set to watch (for select)


		// Zero-out the read file-descriptor's set (readfds)
		FD_ZERO(&readfds);

		// Add the server-socket's fie descriptor to the readfds set
		FD_SET(server.fd, &readfds);
		maxfds = server.fd;

		// Add file descriptors of connected sockets to readfds set
		for(int j = 0; j < max_connections; j++) {
			if(open_connections[j].fd > 0) {
				FD_SET(open_connections[j].fd, &readfds);
				if(maxfds < open_connections[j].fd) {
					maxfds = open_connections[j].fd;
				}
			}
		}

		// select() monitors the file descriptors' set
		printf("\nListening on port %s...\n", port);
		rc = select(maxfds+1, &readfds, NULL, NULL, NULL);
		if(rc == -1) {
			perror("[-] select() error");
			continue;
		}

		// check if server's socket is ready for read
		rc = FD_ISSET(server.fd, &readfds);
		if(rc == -1) {
			perror("[-] FD_ISSET error");
			continue;
		}

		// if server's socket is ready for read a new connection has arrived in queue
		if(rc) {
			// accept connection
			rc = open_connections[i].fd = accept(server.fd, NULL, NULL);
			if(rc == -1) {
				perror("[-] accept() failed");
				exit(1);
			}

			// extract address of incoming connection from the connection's socket
			open_connections[i].addrlen = sizeof(struct sockaddr_in);
			rc = getsockname(open_connections[i].fd, (struct sockaddr *) &open_connections[i].addr, &open_connections[i].addrlen);
			if(rc == -1) {
				perror("[-] accept() failed");
				exit(1);
			}

			// send remote prompt
			rc = send_msg(open_connections[i].fd, remote_prompt, strlen(remote_prompt));
			if(rc == -1) {
				perror("[-] write() failed");
			}

			// print notification message
			printf("\n[!] INCOMMING CONNECTION SOCKET %d ADDRESS %s.\n", open_connections[i].fd, inet_ntoa(open_connections[i].addr.sin_addr));
			i++;
		}

		// check all connections for read status (check if they're in the fds set returned by select())
		for(int j = 0; j < i; j++) {
			if(FD_ISSET(open_connections[j].fd, &readfds)) {
				// receive message from connection socket
				rc = recv_msg(open_connections[j].fd, buf);
				if(rc == -1) { // error
					perror("[-] recv_msg() failed");
					exit(1);

				} else if(rc == 0) { // EOF received
					rc = close(open_connections[j].fd);
					if(rc == -1) {
						perror("[-] Couldn't terminate connection, close() failed");
						exit(1);
					}
					printf("\n[!] SOCKET %d TERMINATED.\n", open_connections[j].fd);
					open_connections[j--] = open_connections[i--];

				} else if(rc > 0) { // valid file request
					// parse request
					buf[strlen(buf)-1] = 0; // delete appended newline character
					switch(action(open_connections[j].fd, inet_ntoa(open_connections[open_connections[j].fd - server.fd - 1].addr.sin_addr), buf, sizeof(buf))) {
						case -2:
							perror("[-] failed to open file");
							break;
						case -1:
							perror("[-] fclose() failed");
							return 1;
							break;
						case 0:
							printf("\n[+] File sent successfully.\n");
							break;
						default:
							break;
					}


					// send remote prompt
					rc = send_msg(open_connections[j].fd, remote_prompt, strlen(remote_prompt));
					if(rc == -1) {
						perror("[-] write() failed");
					}
				}

			}
		}

	}


	return 0; // end of main
}
