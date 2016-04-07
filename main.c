#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#define STD_IN		0

#define MAX_SERVERS	2
#define BUFSIZE		1024
#define MAX_DUTY	100
#define MIN_DUTY	0

struct {
	char *hostname;
	int port;
	int duty;
	struct sockaddr_in serveraddr;
	struct hostent *host;
} server[MAX_SERVERS];

int servers;
int step;

void error(char *msg)
{
	perror(msg);
	exit(0);
}

static inline void clear_screen(void)
{
	const char* cmd = "\e[1;1H\e[2J";
	int ret;

	write(2, cmd, strlen(cmd));
}

static inline void print_config(void)
{
	int i;

	printf("Use the UP/DOWN cursor keys to move servo \n");
	printf("Step %d:\n ", step);
	for (i = 0; i < servers ; i++) {
		printf("Server %d:\n", i);
		printf(" ip	    : %s\n", server[i].hostname);
		printf(" port       : %d\n", server[i].port);
		printf(" duty cycle : %d\n", server[i].duty);
	}
}

char kbhit_irq(void)
{
	struct termios oldt, newt;
	int maxfd = 0;
	fd_set fds;
	char c[3];

	for(;;) {

		FD_ZERO(&fds);
		FD_SET(STD_IN, &fds);

		tcgetattr(STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);

		select(maxfd + 1, &fds, NULL, NULL, NULL);
		read(STD_IN, &c, 3);

		if (c[0] == 0x1b && c[1] == 0x5b) {
			switch(c[2]) {
			case 'A':
			case 'B':
				goto done;
			case 'C':
			case 'D':
				if (servers > 1)
					goto done;
			}
		}
	}

done:
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return c[2];
}

static void process(int sockfd)
{
	char buf[BUFSIZE];
	int serverlen;
	int id = 0;
	int duty, n;
	char c;

	bzero(buf, BUFSIZE);

	c = kbhit_irq();
	if (c == 'C' || c == 'D')
		id = 1;

	duty = server[id].duty;

	switch(c) {
	case 'A':
	case 'C':
		duty = duty + step <= MAX_DUTY ? duty + step : MAX_DUTY;
		break;
	case 'B':
	case 'D':
		duty = duty - step >= MIN_DUTY ? duty - step : MIN_DUTY;
		break;
	default:
		return;
	}

	snprintf(buf, sizeof(buf), "%d", duty);
	serverlen = sizeof(server[id].serveraddr);

	n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*) &server[id].serveraddr, serverlen);
	if (n < 0)
		error("ERROR in sendto");

	server[id].duty = duty;

}

int main(int argc, char **argv)
{
	int sockfd;
	int i;

	if (argc != 4 && argc != 6) {
		fprintf(stderr,"usage: %s <step> <hostname> <port> (err: argc=%d)\n", argv[0], argc);
		return -1;
	}

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	servers = argc == 6 ? 2 : 1;

	step = atoi(argv[1]);

	for (i = 0; i < servers; i++ ) {
		server[i].hostname = argv[2 + i * MAX_SERVERS];
		server[i].port = atoi(argv[3 + i * MAX_SERVERS]);
		server[i].host = gethostbyname(server[i].hostname);
		if (server[i].host == NULL) {
			fprintf(stderr, "ERROR, no such host as %s\n", server[i].hostname);
			exit(0);
		}
		bzero((char *) &server[i].serveraddr, sizeof(server[i].serveraddr));
		bcopy((char *) server[i].host->h_addr, (char *) &server[i].serveraddr.sin_addr.s_addr, server[i].host->h_length);
		server[i].serveraddr.sin_port = htons(server[i].port);
		server[i].serveraddr.sin_family = AF_INET;
		server[i].duty = MIN_DUTY;
	}

	for (;;) {
		clear_screen();
		print_config();
		process(sockfd);
	}

	return 0;
}
