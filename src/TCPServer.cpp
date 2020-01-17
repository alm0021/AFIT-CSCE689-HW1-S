/****************************************************************************************
 * TCPServer - program that sets up a basic TCP server
 *
 *sites used: https://www.geeksforgeeks.org/socket-programming-cc/
 * http://www.linuxhowtos.org/C_C++/socket.htm
 * https://simpledevcode.wordpress.com/2016/06/16/client-server-chat-in-c-using-sockets/
 ****************************************************************************************/

#include "TCPServer.h"
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <sys/time.h>
#include <string>
#include <iostream>
#define PORT 8080

using namespace std;

TCPServer::TCPServer() {
}


TCPServer::~TCPServer() {

}

char buffer[1024] = { 0 };
char msg[1024] = { 0 };
char* hello = (char*)"Hello from server";

/**********************************************************************************************
 * bindSvr - Creates a network socket and sets it nonblocking so we can loop through looking for
 *           data. Then binds it to the ip address and port
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/
void TCPServer::bindSvr(const char* ip_addr, short unsigned int port) {
	printf("STARTING SERVER\n");
	int opt = 1;

	// Creating socket file descriptor 
	printf("SOCKET\n");
	if ((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080 
	printf("SETSOCKOPT\n");
	if (setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(port);

	// Forcefully attaching socket to the port 8080 
	printf("BINDING\n");
	if (bind(this->server_fd, (struct sockaddr*) & this->address, sizeof(this->address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	
}

/**********************************************************************************************
 * listenSvr - Performs a loop to look for connections and create TCPConn objects to handle
 *             them. Also loops through the list of connections and handles data received and
 *             sending of data. 
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/
void TCPServer::listenSvr() {
	printf("LISTENING\n");
	if (listen(this->server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	int pid;

	printf("ACCEPTING\n");
	if ((this->new_socket = accept(this->server_fd, (struct sockaddr*) & this->address, (socklen_t*)&this->addrlen)) < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	printf("CONNECTED TO CLIENT\n");
	//Keep track of session time
	struct timeval start1, end1;
	gettimeofday(&start1, NULL);
	//Keep track of amount of data sent
	int bytesRead, bytesWritten = 0;

	//send Hello message
	//this->valread = read(this->new_socket, buffer, 1024);
	//printf("%s\n", buffer);
	//send(this->new_socket, hello, strlen(hello), 0);
	//printf("Hello message sent\n");

	//send menu
	this->valread = read(this->new_socket, buffer, 1024);
	printf("%s\n", buffer);
	char* menu = (char*)"Hello, please choose an option.\n1. Send Message\n2. Set Password\n3. Option X\n4. Option Y\n5. Option Z\nShow Menu\nexit\n";
	send(this->new_socket, menu, strlen(menu), 0);
	printf("Menu sent\n");

	//Loop to look for connections
	while(true) {
		//receive a message from the client (listen)
		printf("Awaiting client response...\n");
		memset(&msg, 0, sizeof(msg)); //clear the buffer
		bytesRead += recv(this->new_socket, (char*)&msg, sizeof(msg), 0);
		//Process client selection
		if (!strcmp(msg, "exit")) //Client chooses to exit
		{
			cout << "Client has quit the session" << endl;
			break;
		}
		else if (!strcmp(msg, "menu")) //Client chooses menu
		{
			cout << "Client has chosen Menu " << endl;
			cout << "Displaying Menu" << endl;
			
			cout << "Client: " << msg << endl;
			cout << ">";

			memset(&msg, 0, sizeof(msg)); //clear the buffer
			strcpy(msg, menu);
			bytesWritten += send(this->new_socket, (char*)&msg, strlen(msg), 0);
			continue;
		}
		else if (!strcmp(msg, "2"))
		{
			cout << "Client has chosen 2 " << endl;
			cout << "Enter New Password: " << endl;

			cout << "Client: " << msg << endl;
			cout << ">";

			string data = "Password changed! \n";
			memset(&msg, 0, sizeof(msg)); //clear the buffer
			strcpy(msg, data.c_str());
			bytesWritten += send(this->new_socket, (char*)&msg, strlen(msg), 0);
			continue;
		}
		else if (!strcmp(msg, "1") || !strcmp(msg, "3") || !strcmp(msg, "4") || !strcmp(msg, "5"))
		{
			cout << "Client has chosen Option " << msg << endl;
			cout << "Do Option " << msg <<  " Stuff" << endl;

			cout << "Client: " << msg << endl;
			cout << ">";

			string data = "Did the stuff\n";
			memset(&msg, 0, sizeof(msg)); //clear the buffer
			strcpy(msg, data.c_str());
			bytesWritten += send(this->new_socket, (char*)&msg, strlen(msg), 0);
			continue;
		}
		else
		{
			cout << "Client has chosen an invalid ption " << msg << endl;
			cout << "Do Nothing " << endl;

			cout << "Client: " << msg << endl;
			cout << ">";

			string data = "Invalid option\n";
			memset(&msg, 0, sizeof(msg)); //clear the buffer
			strcpy(msg, data.c_str());
			bytesWritten += send(this->new_socket, (char*)&msg, strlen(msg), 0);
			continue;
		}
		//cout << "Client: " << msg << endl;
		//cout << ">";
		//string data;
		//getline(cin, data);
		//memset(&msg, 0, sizeof(msg)); //clear the buffer
		//strcpy(msg, data.c_str());
		//if (data == "exit")
		//{
			//send to the client that server has closed the connection
			//send(this->new_socket, (char*)&msg, strlen(msg), 0);
			//break;
		//}
		//send the message to client
		//bytesWritten += send(this->new_socket, (char*)&msg, strlen(msg), 0);


		/*if (pid = fork() < 0) {
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0) {
			close(this->server_fd);
			//send Hello message
			this->valread = read(this->new_socket, buffer, 1024);
			printf("%s\n", buffer);
			send(this->new_socket, hello, strlen(hello), 0);
			printf("Hello message sent\n");
			exit(0);
		}
		else {
			close(this->new_socket);
		}*/

	} //end loop
	gettimeofday(&end1, NULL);
	cout << "********Session********" << endl;
	cout << "Bytes written: " << bytesWritten << " Bytes read: " << bytesRead << endl;
	cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec)
		<< " secs" << endl;
	cout << "Connection closed..." << endl;
}

/**********************************************************************************************
 * shutdown - Cleanly closes the socket FD.
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPServer::shutdown() {
	if (close(this->new_socket) < 0 || close(this->server_fd) < 0) {
		perror("server shutdown");
		exit(EXIT_FAILURE);
	}
}
