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
#define BUFFER_SIZE 10240
	static char *buffer[BUFFER_SIZE];

	std::cout << "Connecting to: " << SERV_ADDR << ':' << PORT << '\n';

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	error_check("Socked failed", sockfd);

	servaddr.sin_family = AF_INET;
	int check = inet_aton(SERV_ADDR, &servaddr.sin_addr);
	if(check == 0) {
		std::cerr << "IP convertion failed.\n";
		return (1);
	}
	servaddr.sin_port = htons(PORT);

	check = connect(sockfd, 
					reinterpret_cast<struct sockaddr*>(&servaddr), 
					sizeof(servaddr));
	error_check("Connection failed", check);

	std::string message;
	for(;;) {
		std::cin >> message;
		message.push_back('\n');

		send(sockfd, message.data(), message.size(), 0);

		recv(sockfd, buffer, BUFFER_SIZE, 0);

		std::cout << reinterpret_cast<char*>(buffer) << '\n';
	}

	close(sockfd);

	return 0;
}
