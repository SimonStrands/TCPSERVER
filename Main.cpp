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
#define SECRETSTRING "gimboid"

std::string getCommand(char s[])
{
	char command[10];
	sscanf(s, "%s", command);

	//getchar();
	std::string c(command);
	std::string stringReturn = c;
	//The Different commands
	if (strcmp(command, "add") == 0) {
		int x = 0;
		int y = 0;
		sscanf(s, "%s %d %d", command, &x, &y);
		stringReturn = std::to_string(x + y);
	}
	else if (strcmp(command, "div") == 0) {
		int x = 0;
		int y = 0;
		sscanf(s, "%s %d %d", command, &x, &y);
		if (y != 0) {
			stringReturn = std::to_string(x / y);
		}
		else {
			stringReturn = "infinity";
		}
	}
	else if (strcmp(command, "mul") == 0) {
		int x = 0;
		int y = 0;
		sscanf(s, "%s %d %d", command, &x, &y);
		stringReturn = std::to_string(x * y);
	}
	else if (strcmp(command, "sub") == 0) {
		int x = 0;
		int y = 0;
		sscanf(s, "%s %d %d", command, &x, &y);
		stringReturn = std::to_string(x - y);
	}
	else if (strcmp(command, "fadd") == 0) {
		float x = 0;
		float y = 0;
		sscanf(s, "%s %f %f", command, &x, &y);
		stringReturn = std::to_string(x + y);
	}
	else if (strcmp(command, "fdiv") == 0) {
		float x = 0;
		float y = 0;
		sscanf(s, "%s %f %f", command, &x, &y);
		if (y != 0) {
			stringReturn = std::to_string(x / y);
		}
		else {
			stringReturn = "infinity";
		}
	}
	else if (strcmp(command, "fmul") == 0) {
		float x = 0;
		float y = 0;
		sscanf(s, "%s %f %f", command, &x, &y);
		stringReturn = std::to_string(x * y);
	}
	else if (strcmp(command, "fsub") == 0) {
		float x = 0;
		float y = 0;
		sscanf(s, "%s %f %f", command, &x, &y);
		stringReturn = std::to_string(x - y);
	}
	else if (strcmp(command, "penis") == 0) {
		stringReturn = "Fuck yeah";
	}

	return stringReturn;
}

int client(){
	int sockfd;
	addrinfo hints, *servinfo, *p;
	sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	int rv;
	char s[INET_ADDRSTRLEN];
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo( "127.0.0.1", PORT, &hints, &servinfo)) != 0) {
		std::cout << "error couldn't get address" << std::endl;
	}
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			std::cout << "error cant create socket" << std::endl;
		}

		break;
	}

	if (p == NULL) {
		std::cout << "error p == null" << std::endl;
	}

	if (connect(sockfd, p->ai_addr, p->ai_addrlen) < 0 ) {
		std::cout << errno << std::endl;
		if(errno == ETIMEDOUT){
		std::cout << "penis" << std::endl;
		}	
		getchar();
		
	}

	inet_ntop(p->ai_family, ((sockaddr *)p->ai_addr),
		  s, sizeof s);
	std::cout << "client: connecting to %s\n" << s << std::endl;


	freeaddrinfo(servinfo);
	int MAXDATASIZE = 1208;
	char buf[MAXDATASIZE];
	bool gameOver = false;
	bool first = true;
	while (!gameOver){
	std::string sendText = "penis";
		if(first){
			std::cin.ignore();
			first = false;
		}
		std::getline(std::cin, sendText);
		if(send(sockfd, sendText.c_str(), sendText.size() + 1, 0)== -1){
			std::cout << "error couldnt send" << std::endl;
		}
		std::cout << "sent: " << sendText << std::endl;
		if ((recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
			std::cout << "error cant recv" << std::endl;
		}
		else{
			std::cout <<"server sent: " << buf << std::endl;
		}
	}
	
	
}

int server(){
	int sockfd, new_fd;
	addrinfo hints, *servinfo, *p;
	sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int rv;
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if((rv = getaddrinfo(NULL, PORT, &hints, &servinfo))!= 0){
	std::cout << "error" << std::endl;
	}
	for(p = servinfo; p != NULL; p = p->ai_next) {
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
	if(p == NULL){
	std::cout << "error" << std::endl;
	}
	if(listen(sockfd, BACKLOG) == -1){
	std::cout << "error" << std::endl;
	}
	std::cout << "can now listen" << std::endl;
	//sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		std::cout << "error" << std::endl;
	}
	bool gameOver = false;
	int readSize;
	
	sin_size = sizeof(their_addr);
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			std::cout << "error" << std::endl;
		}
	while(!gameOver){
		char msg[1500];
		int MAXSZ=sizeof(msg)-1;
		
		
		readSize=recv(new_fd,&msg,MAXSZ,0);
		std::cout << "Client sent: " << msg << std::endl;
		msg[readSize]=0;
		std::string answear = getCommand(msg);
		std::cout << "Sending: " << answear << std::endl;
		send(new_fd, answear.c_str(), answear.size() + 1, 0);
	}
}

int main() {
	std::string choice;
	std::cout << "S for server, C for Client" << std::endl;
	std::cin >> choice;

	if (choice == "S") {
		return server();
	}
	else if (choice == "C") {
		return client();
	}
	else {
		std::cout << "Command doesn't exist" << std::endl;
		getchar();
		return -1;
	}
	getchar();
	return 0;
}