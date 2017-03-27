#ifndef _HEADER_H_
#define _HEADER_H_

#include <sys/types.h>

#define ACK 1
#define FIN 2
#define HEADER_SIZE 8
#define PACKET_SIZE 1024
#define PAYLOAD_SIZE (PACKET_SIZE - HEADER_SIZE)

typedef struct header_ {

	u_short seq;
	u_short ack;
	u_short offset:10;
	u_short flag:6;

} header_t;

typedef struct packet_ {
	header_t header;
	u_char payload[PAYLOAD_SIZE];
} packet_t;


int fill_header(u_short seq, u_short ack, u_short offset, u_short flag, packet_t* packet);

int add_to_packet(u_char* src, packet_t* packet, u_short size);

int read_header(header_t * head, packet_t * packet);

int read_packet(u_char* dest, packet_t* packet, u_short size);

#endif
