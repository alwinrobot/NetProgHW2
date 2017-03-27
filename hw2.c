#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdarg.h>
#include <assert.h>
#include <fcntl.h>
#include "header.h"

#define RRQ 0x01
#define WRQ 0x02
#define DATA 0x03
#define ACK 0x04
#define ERROR 0x05

#define BUFFER_LENGTH 516
#define DATA_SIZE 512
#define SEND_TIMEOUT 1000
#define RECV_TIMEOUT 10000

int create_req_packet(int opcode, char* file_name, char* mode, char buf[]) {


}

int create_ack_packet(int block, char buf[]) {

}

/*
int send_file(int socket_fd, struct sockaddr_in *client_addr, char* file_name) {


	
	return 0;
}
*/

void get_packet(char* file_name_ptr, struct sockaddr_in client_addr, char* mode_ptr, int tid) {

	// variables
	int sock_fd, len, opcode, n, datasize, errno, recv;
	struct sockaddr_in client_data;
	socklen_t client_length;
	unsigned short int count = 0;
	unsigned short int rcount = 0;

	unsigned char file_buffer[BUFFER_LENGTH + 1];
	unsigned char packet_buffer[BUFFER_LENGTH + 12]; // used to send 
	char file_name[23], mode[12], path[128], ack_buf[512];
	char* buf_ptr;
	FILE* fp;

	strcpy(file_name, file_name_ptr);
	strcpy(mode, mode_ptr);

	sock_fd = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock_fd < 0) {
		printf("Could not establish connection\n");
		return;
	}

	if (!strncasecmp(mode, "octet", 5)) {
		printf("Wrong mode!\n");
		return ;
	}

	fp = fopen(file_name, "w");

	if (fp == NULL) {

		// null file pointer
		printf("NULL File\n");
		return;
	}

	

	else {

		n = BUFFER_LENGTH;
		memset(file_buffer, 0, sizeof(file_buffer));

		do {

	        // zero init buffers
	        memset (packet_buffer, 0, sizeof (packet_buffer));
	        memset (ack_buf, 0, sizeof (ack_buf));
		     

		    if (count == 0 || (count % 1) == 0 || n != BUFFER_LENGTH) {
				len = sprintf(ack_buf, "%c%c%c%c", 0x00, ACK, 0x00, 0x00);
				ack_buf[2] = (count & 0xFF00) >> 8;	
				ack_buf[3] = (count & 0x00FF);
			} 

			if (sendto(sock, ack_buf, len, 0, (struct sockaddr *) &client_addr, sizeof(client_addr)) != len) {
				perror("Wrong number of bytes sent!\n");
				return;
			}


		    if (n != BUFFER_LENGTH) {

		    	printf("Wrong length\n");
				return;
			}

		    count++;
			client_length = sizeof(client_data);

			errno = EAGAIN;	/* this allows us to enter the loop */
			n = -1;
			while (i <= RECV_TIMEOUT && errno == EAGAIN && & n < 0) {

				// receive from packet
			    n = recvfrom (sock_fd, packet_buffer, sizeof(packet_buffer) - 1, 0, (struct sockaddr *) &client_data, client_length);
			    usleep (1000);
			}

			// no timeout
			if (n < 0) {
				printf ("The server could not receive from the client\n");
				exit(1);

			}
			else {


			    if (client_addr.sin_addr.s_addr != client_data.sin_addr.s_addr) {
				    printf("Error recieving file (data from invalid address)\n");
				    return;
				}

			    if (tid != ntohs (client_addr.sin_port))	{
				    printf ("Error recieving file (data from invalid tid)\n");
				    
				    len = sprintf ((char *) packet_buffer, "%c%c%c%cBad/Unknown TID%c", 0x00, ERROR, 0x00, 0x05, 0x00);
				    
				    if (sendto(sock, packet_buffer, len, 0, (struct sockaddr *) &client_addr, sizeof(client_addr)) != len) {
				    	printf("Could not send error packet\n");
				    	return;
				    }
				}
				// format the buffer
			    bu_ptr = (char *) packet_buffer;

			   	if (buf_ptr++[0] != 0x00) {
					printf("Packet must begin with 0x00\n");
					return;
				}
			    opcode = *bufindex++;
		        rcount = *bufindex++ << 8;
		      	rcount &= 0xff00;
		      	rcount += (*bufindex++ & 0x00ff);



			    memcpy((char *) filebuf, buf_ptr, n - 4);	/* copy the rest of the packet (data portion) into our data array */
	
				if (n > 516) {
					datasize = n - 4;
				}
			    	
				}
			    if (opcode != 3 || rcount != count) {
			    	printf("Badly ordered/invalid data packet, sending error packet\n");

	/* sending error message */
			  		if (opcode > 5) {
					    len = sprintf ((char *) packet_buffer, "%c%c%c%cIllegal operation%c", 0x00, ERROR, 0x00, 0x04, 0x00);
					    
					    if (sendto(sock_fd, packet_buffer, len, 0, (struct sockaddr *) &client, sizeof(client)) != len) {
						    printf("Could not send error packet\n");
						    return;
						}
					}
				}

			    else {
					break;
				}
			    


				if (sendto(sock, ack_buf, len, 0, (struct sockaddr *) &client_addr, sizeof (client_addr)) != len) {

					printf ("Mismatch in number of sent bytes\n");
				    return;
				}

			}


		} while (fwrite(file_buffer, 1, n - 4, fp) == n - 4);
	


		fclose (fp);
		sync ();
		printf ("fclose and sync successful. File received successfully\n");

	  	return;
	}
}

int main() {

	// variables
	pid_t pid;
	int socket_fd, new_fd, child_fd, i, bound, send_sock_opt, recv_sock_opt, local_port, ret, opcode, tid;
	int recv_timeout = RECV_TIMEOUT;
	int send_timeout = SEND_TIMEOUT;

	struct addrinfo *server_info;
	struct addrinfo hints;
	struct sockaddr_in serv_addr, client_addr;
	struct timeval sv, rv;
	socklen_t client_length;

	unsigned char buf[BUFFER_LENGTH + 1];
	unsigned char* buf_ptr;
	char file_name[23], mode[12] = "octet";

	//static const char MODE[] = "octet";

	sv.tv_sec = 0;
	sv.tv_usec = 1000;

	rv.tv_sec = 0;
	rv.tv_usec = 10000;

	// connect to the socket
	socket_fd = socket(PF_INET, SOCK_DGRAM, 0);
	if (socket_fd < 0) {
		perror("Could not connect to socket\n");
		exit(1);
	}

	// set memory
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(0);


    
    // set timeouts for retransmitting message and closing socket
	recv_sock_opt = setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &rv, sizeof(rv));
    if (recv_sock_opt < 0) {
    	perror("Could not set timeout sock option\n");
    	exit(1);
    }
    

    send_sock_opt = setsockopt(socket_fd, SOL_SOCKET, SO_SNDTIMEO, &sv, sizeof(sv));
    if (send_sock_opt < 0) {
    	perror("Could not set timeout sock option\n");
    	exit(1);
    }
   

    // bind to socket
    bound = bind(socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (bound < 0) {
    	perror("Count not bind to socket\n");
    	exit(1);
    }

    // get length of client address
    client_length = sizeof(client_addr);
    ret = getsockname(socket_fd, (struct sockaddr*) &client_addr, &client_length);
    if (ret < 0) {
    	perror("Problem with getsockname\n");
    	exit(1);
    }


    // get port and print
    local_port = ntohs(client_addr.sin_port);
    printf("Port is %d\n", local_port);




	while (1) {

		
		new_fd = recvfrom(socket_fd, buf, BUFFER_LENGTH, 0, (struct sockaddr_in *) &client_addr, client_length);

		if (new_fd < 0) {
			perror("Could not receive client request\n");
		}

		// get pointer to loop through the buffer
		buf_ptr = buf;

		// first bit must be 0
		if (*buf_ptr++ != 0) {
			perror("Improperly formed packet\n");
			exit(1);
		}

		opcode = *buf_ptr++;
		if (opcode == 1 || opcode == 2) {
			strncpy(file_name, buf_ptr, sizeof(file_name) - 1);

			// move ptr
			buf_ptr += sizeof(file_name) - 1;

			// copy mode
			strncpy(mode, buf_ptr, sizeof(mode) - 1);

			buf_ptr += sizeof(mode) - 1;
		}

		// handle the different types of opcodes
		switch (opcode) {
			case 1: //if packet is RRQ
				pid = fork();
	
				if (pid == -1) {

					perror("Could not create child process\n");
				}

				// child process
				else if (pid == 0) {

					// we can close the parent socket now!
					close(socket_fd);

					printf("opcode suggests read request\n");

					get_packet(file_name, client_addr, mode, tid);

					sendto(socket_fd, buf, BUFFER_LENGTH, 0, (struct sockaddr_in *) &client_addr, &client_length);

					close(child_fd);
					exit(0);

				}
				break;

			case 2: // if packet is WRQ
				pid = fork();
	
				if (pid == -1) {

					perror("Could not create child process\n");
				}

				// child process
				else if (pid == 0) {

					// we can close the parent socket now!
					close(socket_fd);

					// connect to socket in child process
					child_fd = socket(PF_INET, SOCK_DGRAM, 0);
					if (child_fd < 0) {
						printf("Could not connect to socket on child end\n");
						return;
					}

					printf("Received connection\n");

					// check for illegal characters

					// 

					sendto(socket_fd, buf, BUFFER_LENGTH, 0, (struct sockaddr *) &client_addr, client_length);

					close(child_fd);
					exit(0);

				}
				break;

			default:

				printf("Invalid opcode, ignoring packet\n");

		}
		
		
	}

	return 0;

}