#define _XOPEN_SOURCE 600

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

void extract_ip_string(struct sockaddr *address, char *dst, size_t len);

int main(int argc, char *argv[]) {
	int in, out, from;
	struct sockaddr_in down, me;
	void* buffer;
	int size;

	down.sin_family = me.sin_family = AF_INET;
	down.sin_port = me.sin_port = htons(1111);
	inet_aton(gethostbyname("downstream")->h_addr_list[0],&down.sin_addr);
	inet_aton(gethostbyname("localhost")->h_addr_list[0],&me.sin_addr);

	out = socket(PF_INET, SOCK_DGRAM, 0);
	in = socket(AF_INET, SOCK_DGRAM, 0);

	bind(in, (struct sockaddr *)&me, 99);

	while(1) {
		from = accept(in, NULL, NULL);

		buffer = calloc(1, 65536);

		size = recvfrom(from, buffer, 65536, 0, NULL, NULL);

		//extract_ip_string(&remote, ip_as_str);
		//syslog(LOG_INFO, "%s is raining", ip_as_str);

		//
		sendto(out, buffer, size, 0, NULL, 0);
	}
}

void extract_ip_string(struct sockaddr *address, char *dst) {
	void *addr;

	if(address->sa_family ==AF_INET) {
		// IPv6 Land
		addr = &((struct sockaddr_in6 *)address)->sin6_addr;
	} else {
		// IPv4 Land
		addr = &(((struct sockaddr_in *)address)->sin_addr);
	}
	inet_ntop(address->sa_family, addr, dst, 99);
}


