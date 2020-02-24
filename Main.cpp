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

std::string clientCalc(char msg[]){
	char command[10];
	std::string theReturn;
	int optionint1 = 0;
	int optionint2 = 0;
	float optionfloat1 = 0;
	float optionfloat2 = 0;
	
	int rv=sscanf(msg,"%s ",command);
	if(strcmp(command, "add") == 0){
		sscanf(msg, "%s %d %d", command, &optionint1, &optionint2);
		theReturn = std::to_string(optionint1 + optionint2); 
	}
	else if(strcmp(command, "mul") == 0){
		sscanf(msg, "%s %d %d", command, &optionint1, &optionint2);
		theReturn = std::to_string(optionint1 * optionint2); 
	}
	else if(strcmp(command, "sub") == 0){
		sscanf(msg, "%s %d %d", command, &optionint1, &optionint2);
		theReturn = std::to_string(optionint1 - optionint2); 
	}
	else if(strcmp(command, "div") == 0){
		sscanf(msg, "%s %d %d", command, &optionint1, &optionint2);
		theReturn = std::to_string(optionint1 / optionint2); 
	}
	else if(strcmp(command, "fadd") == 0){
		sscanf(msg, "%s %f %f", command, &optionfloat1, &optionfloat2);
		theReturn = std::to_string(optionfloat1 + optionfloat2); 
	}
	else if(strcmp(command, "fmul") == 0){
		sscanf(msg, "%s %f %f", command, &optionfloat1, &optionfloat2);
		theReturn = std::to_string(optionfloat1 * optionfloat2); 
	}
	else if(strcmp(command, "fdiv") == 0){
		sscanf(msg, "%s %f %f", command, &optionfloat1, &optionfloat2);
		theReturn = std::to_string(optionfloat1 / optionfloat2); 
	}
	else if(strcmp(command, "fsub") == 0){
		sscanf(msg, "%s %f %f", command, &optionfloat1, &optionfloat2);
		theReturn = std::to_string(optionfloat1 - optionfloat2); 
	}
	else{
		theReturn = "error";
	}
	
	return theReturn;
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
		std::cout << "can't connect" << std::endl;	
		getchar();
		
	}

	inet_ntop(p->ai_family, ((sockaddr *)p->ai_addr),
		  s, sizeof s);
	std::cout << "client: connecting to \n" << s << std::endl;


	freeaddrinfo(servinfo);
	int MAXDATASIZE = 1208;
	char buf[MAXDATASIZE];
	bool gameOver = false;
	bool first = true;
	std::string sendText = "GiveMeQuestion";
	if(send(sockfd, sendText.c_str(), sendText.size() + 1, 0)== -1){
		std::cout << "penis" << std::endl;
	}
	else{
		if ((recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
			std::cout << "error cant recv" << std::endl;
		}
		else{
			std::cout <<"server sent: " << buf << std::endl;
		}
		if ((recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
			std::cout << "error cant recv" << std::endl;
		}
		else{
			std::cout <<"server sent: " << buf << std::endl;
		}
		//change here
		sendText = clientCalc(buf);
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
	close(sockfd);
		
	
	
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
		float answear;
		float x; 
		float y;
		std::string Sanswear = "";
		std::string stringAnswear = "";
	while(!gameOver){
		Sanswear = "TEXT TCP SERVER 1.0\n";
		char msg[1500];
		int MAXSZ=sizeof(msg)-1;
		
		
		readSize=recv(new_fd,&msg,MAXSZ,0);
		std::cout << "Client sent: " << msg << std::endl;
		if(strcmp(msg, "GiveMeQuestion")==0){
			send(new_fd, Sanswear.c_str(), Sanswear.size() + 1, 0);
			//give question
			std::cout << "giving question" << std::endl;
			int rNrG = rand()%8;
			x = ((rand()%100) +3);
			y = ((rand()%100) +3);
			std::cout << x << y << std::endl;
			x = x/10;
			y = y/10;
			std::cout << x << y << std::endl;
			switch(rNrG){
				case 0 :
				answear = (int)x + (int)y;
				Sanswear = "add " + std::to_string((int)x) + " " + std::to_string((int)y);
				break;
				case 1 :
				answear = (int)x / (int)y;
				Sanswear = "div " + std::to_string((int)x) + " " + std::to_string((int)y);
				break;
				case 2 :
				answear = (int)x * (int)y;
				Sanswear = "mul " + std::to_string((int)x) + " " + std::to_string((int)y);
				break;
				case 3 :
				answear = (int)x - (int)y;
				Sanswear = "sub " + std::to_string((int)x) + " " + std::to_string((int)y);
				break;
				case 4 :
				answear = x + y;
				Sanswear = "fadd " + std::to_string(x) + " " + std::to_string(y);
				break;
				case 5 :
				answear = x / y;
				Sanswear = "fdiv " + std::to_string(x) + " " + std::to_string(y);
				break;
				case 6 :
				answear = x * y;
				Sanswear = "fmul " + std::to_string(x) + " " + std::to_string(y);
				break;
				case 7 :
				answear = x - y;
				Sanswear = "fsub " + std::to_string(x) + " " + std::to_string(y);
				break;
			}
			std::cout << "the question " << Sanswear<<std::endl;
			std::cout << "the answear " << answear << std::endl;
			stringAnswear = std::to_string(answear);
			send(new_fd, Sanswear.c_str(), Sanswear.size() + 1, 0);
		}
		else if(strcmp(msg, "") == 0){

		}
		else{
		//check answear
			float cAnswear;
			cAnswear = std::atof(msg);
			if(strcmp(stringAnswear.c_str(), msg)==0 || cAnswear == answear){
				Sanswear = "OK";
				std::cout << "OK" << std::endl;
			}
			else{
				cAnswear = std::atoi(msg);
				if(cAnswear == answear){
					Sanswear = "OK";
					std::cout << "OK" << std::endl;
				}
				else{
					Sanswear = "ERRORs";
					std::cout << "ERRORs" << std::endl;
				}
			}
			msg[readSize]=0;
			std::cout << "Sending: " << Sanswear << std::endl;
			send(new_fd, Sanswear.c_str(), Sanswear.size() + 1, 0);
			new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
			if (new_fd == -1) {
				std::cout << "error" << std::endl;
			}
		}
		
	}
	std::cout << "quitting"<< std::endl;
	
}

int main() {
	 srand(time(0)); 
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