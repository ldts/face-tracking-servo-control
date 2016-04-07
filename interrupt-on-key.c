#include <sys/select.h>
#include <stdio.h>

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


#define STDIN 0

int main()
{
	struct termios oldt, newt;
	char c;

	fd_set fds;
	int maxfd = 0;

	while (1) {

		FD_ZERO(&fds);
		FD_SET(STDIN, &fds);

		tcgetattr(STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);

		select(maxfd+1, &fds, NULL, NULL, NULL);

		read(STDIN, &c, 1);
		printf("0x%x\n", c);


	}

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

}
