#include "../headers/networking.h"


// init_server: creates socket (endpoint) for connection to <hostname> and <port>
// PARAMS:
//           fd: socket's file descriptor
//          buf: received message
//         addr: socket's address
//      bufsize: size of received message
// RETURN:
//       0: success
//      -1: failed to close file
//      -2: failed to open file
//
int action(int fd, char *addr, char *buf, size_t bufsize) {
	int rc = 0;
	FILE *fp;
	

	printf("\n[*] Fetching file %s for %s on socket %d...\n", buf, addr, fd);
	
	fp = fopen(buf, "r");
	if(fp == NULL) {
		char *err_msg = "failed to retrieve file.\n";

		write(fd, err_msg, strlen(err_msg));
		perror("[-] failed to open file");
		

		return -2;
	}
	
	while (memset(buf, 0, bufsize) != NULL && fgets(buf, BUFFERSIZE, fp) != NULL) {
		rc = send_msg(fd, buf, strlen(buf));
		if(rc == -1) {
			perror("send_msg() failed");
		}
	}	
	
	if(fclose(fp) == EOF) {
		perror("[-] fclose() failed");
		return -1;
	}

	return 0;
}
