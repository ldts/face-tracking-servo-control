#include <sys/select.h>
#include <stdio.h>
 #define STDIN 0
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


atexit()

int main()
 {
	struct termios oldt, newt;
	char c;

	fd_set fds;
    	int maxfd = 0;

    while(1){

        FD_ZERO(&fds);
        FD_SET(sd, &fds); 
        FD_SET(STDIN, &fds); 

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
//	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
//	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

        select(maxfd+1, &fds, NULL, NULL, NULL); 
	//ch = getchar();

	//tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	//fcntl(STDIN_FILENO, F_SETFL, oldf);

	read(0, &c, 1);
	printf("%c\n", c);

	
        }

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	}
