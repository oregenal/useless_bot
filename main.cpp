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


void error_check(const char *reason, int err)
{
	if(err == -1) {
		perror(reason);
		exit(1);
	}
}

int main(void)
{
	struct sockaddr_in servaddr;
#define BUFFER_SIZE 102400
	static char *buffer[BUFFER_SIZE];

	int sockfds[2];
	sockfds[0] = STDIN_FILENO;

	std::cout << "Connecting to: " << SERV_ADDR << ':' << PORT << '\n';

	sockfds[1] = socket(AF_INET, SOCK_STREAM, 0);
	error_check("Socked failed", sockfds[1]);

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
	error_check("Connection failed", check);

	for(;;) {
		fd_set readfds;
		int max_d = sockfds[1];

		FD_ZERO(&readfds);
		FD_SET(sockfds[0], &readfds);
		FD_SET(sockfds[1], &readfds);

		int res = select(max_d+1, &readfds, NULL, NULL, NULL);
		if(res == -1) {
			if(errno == EINTR) {
				// recived signal error
			} else {
				// select error
			}
			continue;
		}

		if(res == 0) {
			// timeout
			continue;
		}

		if(FD_ISSET(sockfds[0], &readfds)) {
			std::string message;
			std::cin >> message;
			message.push_back('\n');

			//TODO: May be better send() & reciv().
			check = write(sockfds[1], message.data(), message.size());
			error_check("Data send fail", check);
		}

		if(FD_ISSET(sockfds[1], &readfds)) {
			check = read(sockfds[1], buffer, BUFFER_SIZE);
			error_check("Data recive fail", check);
			if(check == 0)
				break;

			std::cout << reinterpret_cast<char*>(buffer);
		}
	}

	close(sockfds[1]);

	return 0;
}
