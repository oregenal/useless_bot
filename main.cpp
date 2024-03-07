#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#if 0

#define SERV_ADDR "127.0.0.1" // localhost
#define PORT 8888

#else

#define SERV_ADDR "173.194.73.94" // www.google.com
#define PORT 80

#endif

#define BUFFER_SIZE 102400
static char *buffer[BUFFER_SIZE];

bool end_of_file = false;

void data_transfer(int srcfd, int destfd) {
	int check = read(srcfd, buffer, BUFFER_SIZE);
	if(-1 == check) throw "Data recive fail";
	if(check == 0)
		end_of_file = true;

	check = write(destfd, buffer, check);
	if(-1 == check) throw "Data send fail";
}

int main(void)
{
	struct sockaddr_in servaddr;

	int sockfds[2];
	sockfds[0] = STDIN_FILENO;

	std::cout << "Connecting to: " << SERV_ADDR << ':' << PORT << '\n';

	try {
		sockfds[1] = socket(AF_INET, SOCK_STREAM, 0);
		if(-1 == sockfds[1]) throw "Socket failed";

		servaddr.sin_family = AF_INET;
		int check = inet_aton(SERV_ADDR, &servaddr.sin_addr);
		if(check == 0) {
			std::cerr << "IP convertion failed.\n";
			return (1);
		}
		servaddr.sin_port = htons(PORT);

		check = connect(sockfds[1], 
				reinterpret_cast<struct sockaddr*>(&servaddr), 
				sizeof(servaddr));
		if(-1 == check) throw "Connection failed";

		for(;;) {
			fd_set readfds;
			int max_d = sockfds[1];

			FD_ZERO(&readfds);
			FD_SET(sockfds[0], &readfds);	// stdin
			FD_SET(sockfds[1], &readfds);	// site

			int res = select(max_d+1, &readfds, NULL, NULL, NULL);
			if(res == -1) throw "Recived signal error";

			if(res == 0) continue;			// timeout 

			if(FD_ISSET(sockfds[0], &readfds)) 
				data_transfer(sockfds[0], sockfds[1]);

			if(FD_ISSET(sockfds[1], &readfds)) 
				data_transfer(sockfds[1], sockfds[0]);

			if(end_of_file)
				break;
		}

		close(sockfds[1]);

	} catch(const char *str) {
		close(sockfds[1]);
		perror(str);
		return 1;
	}

	return 0;
}
