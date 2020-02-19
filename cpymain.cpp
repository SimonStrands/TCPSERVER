#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

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
int server()
{
	//information about what the scoket should take in
	sockaddr_in serverHint;
	//serverHint.sin_addr.S_un.S_addr = INADDR_ANY;
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(54000);
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	//Bind
	if (bind(sockfd, (sockaddr*)&serverHint, sizeof(serverHint)) == SO_ERROR) {
		std::cout << "cant bind socker " << std::endl;
	}

	listen(sockfd, SOMAXCONN);
	sockaddr_in client;
	int clientLeng = sizeof(client);

	int clientSocket = accept(sockfd, (sockaddr*)&client, &clientLeng);


	std::string whatToSend;
	bool gameOver = false;
	while (!gameOver)
	{
		char buff[1024];
		int bytesIn = recv(clientSocket, buff, 1024, 0);
		if (bytesIn > -1) {
			//first contact
			if (strcmp(buff, "magicWordSinep") == 0) {
				whatToSend = " TheMagic Word Was Sinep!";
				send(clientSocket, whatToSend.c_str(), whatToSend.size() + 1, 0);
				std::cout << "client connect" << std::endl;
			}//quit
			else if (strcmp(buff, "Q") == 0) {
				return 0;
			}
			else {
				std::cout << "client sent: " << buff << std::endl;
				whatToSend = getCommand(buff);
				send(clientSocket, whatToSend.c_str(), whatToSend.size() + 1, 0);
				std::cout << "Response: " << whatToSend << std::endl;
			}
		}
		else {
			std::cout << "error" << bytesIn << std::endl;
			getchar();
			return -1;
		}
	}
	close(clientSocket);
	close(sockfd);
	return 0;
}
int client()
{



	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(54000);
	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	int connector = connect(sockfd, (sockaddr*)&server, sizeof(server));
	if (connector == SO_ERROR) {
		std::cout << "oh shit 2" << std::endl;
		getchar();
	}
	//first word to server
	std::string whatToSend = "magicWordSinep";
	send(sockfd, whatToSend.c_str(), whatToSend.size() + 1, 0);
	//listen if it exist
	char buff[1024];
	int serverLeng = sizeof(server);
	int bytesIn = recv(sockfd, buff, 1024, 0);
	if (bytesIn > -1) {
		std::cout << "server sent" << buff << std::endl;
	}
	else {
		std::cout << "Error bytein" << bytesIn << std::endl;
		getchar();
		return -1;
	}

	bool gameOver = false;
	bool once = true;
	while (!gameOver) {
		std::cout << "what you wanna write?" << std::endl;
		if (once) {
			std::cin.ignore();
			once = false;
		}//send information to server
		std::getline(std::cin, whatToSend);
		int sendOk = send(sockfd, whatToSend.c_str(), whatToSend.size() + 1, 0);
		if (sendOk == SO_ERROR) {
			std::cout << "that didnt work" << std::endl;
		}//get information back
		bytesIn = recv(sockfd, buff, 1024, 0);
		std::cout << "server sent: " << buff << std::endl;
		if (whatToSend == "Q") {
			gameOver = true;
		}
	}
	close(sockfd);
	return 0;
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



