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
//färdig?
std::string clientCalc(char msg[]) {
	char command[10];
	std::string theReturn;
	int optionint1 = 0;
	int optionint2 = 0;
	float optionfloat1 = 0;
	float optionfloat2 = 0;

	int rv = sscanf(msg, "%s ", command);
	if (strcmp(command, "add") == 0) {
		sscanf(msg, "%s %d %d", command, &optionint1, &optionint2);
		theReturn = std::to_string(optionint1 + optionint2);
	}
	else if (strcmp(command, "mul") == 0) {
	sscanf(msg, "%s %d %d", command, &optionint1, &optionint2);
	theReturn = std::to_string(optionint1 * optionint2);
	}
	else if (strcmp(command, "sub") == 0) {
	sscanf(msg, "%s %d %d", command, &optionint1, &optionint2);
	theReturn = std::to_string(optionint1 - optionint2);
	}
	else if (strcmp(command, "div") == 0) {
	sscanf(msg, "%s %d %d", command, &optionint1, &optionint2);
	theReturn = std::to_string(optionint1 / optionint2);
	}
	else if (strcmp(command, "fadd") == 0) {
	sscanf(msg, "%s %f %f", command, &optionfloat1, &optionfloat2);
	theReturn = std::to_string(optionfloat1 + optionfloat2);
	}
	else if (strcmp(command, "fmul") == 0) {
	sscanf(msg, "%s %f %f", command, &optionfloat1, &optionfloat2);
	theReturn = std::to_string(optionfloat1 * optionfloat2);
	}
	else if (strcmp(command, "fdiv") == 0) {
	sscanf(msg, "%s %f %f", command, &optionfloat1, &optionfloat2);
	theReturn = std::to_string(optionfloat1 / optionfloat2);
	}
	else if (strcmp(command, "fsub") == 0) {
	sscanf(msg, "%s %f %f", command, &optionfloat1, &optionfloat2);
	theReturn = std::to_string(optionfloat1 - optionfloat2);
	}
	else {
	theReturn = "error";
	}

	return theReturn;
}
//client
//  1     2	   3 
//arg = things, addr, Port 
//deafault is localhost and port 4950
int main(int argc, char* argv[]) {
	int sockfd;
	addrinfo hints, * servinfo, * p;
	int rv;
	char s[INET_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if (argc == 2) {
		if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
			std::cout << "error couldn't get address" << std::endl;
		}
	}
	else if (argc >= 3) {
		if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
			std::cout << "error couldn't get address" << std::endl;
		}
	}
	else {
		if ((rv = getaddrinfo("127.0.0.1", PORT, &hints, &servinfo)) != 0) {
			std::cout << "error couldn't get address" << std::endl;
		}
	}

	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			std::cout << "error cant create socket" << std::endl;
		}
		break;
	}

	if (p == NULL) {
		std::cout << "error p == null" << std::endl;
	}

	if (connect(sockfd, p->ai_addr, p->ai_addrlen) < 0) {
		std::cout << "can't connect" << std::endl;
		getchar();

	}

	inet_ntop(p->ai_family, ((sockaddr*)p->ai_addr),
		s, sizeof s);
	std::cout << "client: connecting to \n" << s << std::endl;

	freeaddrinfo(servinfo);
	int MAXDATASIZE = 1208;
	char buf[MAXDATASIZE];
	std::string sendText = "";
	if ((recv(sockfd, buf, MAXDATASIZE - 1, 0)) == -1) {
		std::cout << "error cant recv" << std::endl;
	}
	else {
		//Text TCP 1.0
		std::cout << "server sent: " << buf << std::endl;
	}
	if (strcmp(buf, "TEXT TCP SERVER 1.0") == 0) {
		sendText = "OK";
		if (send(sockfd, sendText.c_str(), sendText.size() + 1, 0) == -1) {
			std::cout << "can't send" << std::endl;
		}
		if (recv(sockfd, buf, MAXDATASIZE - 1, 0) == -1){
			std::cout << "cant get message" << std::endl;
		}
		else {
			std::cout << "Server sent " << buf << std::endl;
			sendText = clientCalc(buf);
			if (send(sockfd, sendText.c_str(), sendText.size() + 1, 0) == -1) {
				std::cout << "error couldnt send" << std::endl;
			}
			std::cout << "sent: " << sendText << std::endl;

			if ((recv(sockfd, buf, MAXDATASIZE - 1, 0)) == -1) {
				std::cout << "error cant recv" << std::endl;
			}
			else {
				std::cout << "server sent: " << buf << std::endl;
			}
		}
	}
	else {
		std::cout << "server sent " << buf << "shutting down" << std::endl;
	}
	close(sockfd);
	return 0;
}