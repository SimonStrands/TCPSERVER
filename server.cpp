#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "4950"  
#define BACKLOG 1



//server
//  1     2	   3 
//arg = things, addr, Port
//deafault is port 4950
int main(int argc, char* argv[]) {
	int sockfd, new_fd;
	addrinfo hints, * servinfo, * p;
	sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if (argc >= 2) {
		if ((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) {
			std::cout << "error" << std::endl;
		}
	}
	else {
		if((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
			std::cout << "error" << std::endl;
		}
	}
	
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
			p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		int yes = 1;
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
			sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}
	freeaddrinfo(servinfo);
	if (p == NULL) {
		std::cout << "error" << std::endl;
	}
	if (listen(sockfd, BACKLOG) == -1) {
		std::cout << "error" << std::endl;
	}
	std::cout << "can now listen" << std::endl;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		std::cout << "error" << std::endl;
	}
	bool gameOver = false;
	int readSize;

	sin_size = sizeof(their_addr);
	
	float answear;
	float x;
	float y;
	std::string Sanswear = "";
	std::string stringAnswear = "";
	while (!gameOver) {
		char msg[1500];
		int MAXSZ = sizeof(msg) - 1;
		memset(msg, 0, 1500);
		new_fd = accept(sockfd, (struct sockaddr*) & their_addr, &sin_size);
		if (new_fd == -1) {
			std::cout << "error" << std::endl;
		}
		else {
			Sanswear = "TEXT TCP SERVER 1.0";
			send(new_fd, Sanswear.c_str(), Sanswear.size() + 1, 0);
			readSize = recv(new_fd, &msg, MAXSZ, 0);
			std::cout << "Client sent: " << msg << std::endl;
			if ((strcmp(msg, "OK") == 0) || (strcmp(msg, "OK\n") == 0)) {
				//give question
				std::cout << "giving question" << std::endl;
				int rNrG = rand() % 8;
				x = ((rand() % 100) + 3);
				y = ((rand() % 100) + 3);
				std::cout << x << y << std::endl;
				x = x / 10;
				y = y / 10;
				std::cout << x << y << std::endl;
				switch (rNrG) {
				case 0:
					answear = (int)x + (int)y;
					Sanswear = "add " + std::to_string((int)x) + " " + std::to_string((int)y);
					break;
				case 1:
					answear = (int)x / (int)y;
					Sanswear = "div " + std::to_string((int)x) + " " + std::to_string((int)y);
					break;
				case 2:
					answear = (int)x * (int)y;
					Sanswear = "mul " + std::to_string((int)x) + " " + std::to_string((int)y);
					break;
				case 3:
					answear = (int)x - (int)y;
					Sanswear = "sub " + std::to_string((int)x) + " " + std::to_string((int)y);
					break;
				case 4:
					answear = x + y;
					Sanswear = "fadd " + std::to_string(x) + " " + std::to_string(y);
					break;
				case 5:
					answear = x / y;
					Sanswear = "fdiv " + std::to_string(x) + " " + std::to_string(y);
					break;
				case 6:
					answear = x * y;
					Sanswear = "fmul " + std::to_string(x) + " " + std::to_string(y);
					break;
				case 7:
					answear = x - y;
					Sanswear = "fsub " + std::to_string(x) + " " + std::to_string(y);
					break;
				}
				std::cout << "the question " << Sanswear << std::endl;
				std::cout << "the answear " << answear << std::endl;
				stringAnswear = std::to_string(answear);
				send(new_fd, Sanswear.c_str(), Sanswear.size() + 1, 0);
				readSize = recv(new_fd, &msg, MAXSZ, 0);
				if (strcmp(msg, "") == 0) {
					std::cout << "we got nothign" << std::endl;
				}
				else {
					//check answear
					float cAnswear;
					cAnswear = std::atof(msg);
					if (strcmp(stringAnswear.c_str(), msg) == 0 || cAnswear == answear) {
						Sanswear = "OK";
						std::cout << "OK" << std::endl;
					}
					else {
						cAnswear = std::atoi(msg);
						if (cAnswear == answear) {
							Sanswear = "OK";
							std::cout << "OK" << std::endl;
						}
						else {
							Sanswear = "Error";
							std::cout << "Error" << std::endl;
						}
					}
					send(new_fd, Sanswear.c_str(), Sanswear.size() + 1, 0);
					close(new_fd);
				}
			}
			else {
				close(new_fd);
			}
		}

	}
	std::cout << "quitting" << std::endl;
	close(sockfd);
	return 0;
}