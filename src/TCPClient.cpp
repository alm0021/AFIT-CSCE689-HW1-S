/****************************************************************************************
 * TCPClient - program that sets up a basic TCP client
 *
 *sites used: https://www.geeksforgeeks.org/socket-programming-cc/
 * http://www.linuxhowtos.org/C_C++/socket.htm
 * https://simpledevcode.wordpress.com/2016/06/16/client-server-chat-in-c-using-sockets/
 ****************************************************************************************/

#include "TCPClient.h"
#include <stdio.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/time.h>
#include <iostream>

using namespace std;

/**********************************************************************************************
 * TCPClient (constructor) - Creates a Stdin file descriptor to simplify handling of user input. 
 *
 **********************************************************************************************/

TCPClient::TCPClient() {
}

/**********************************************************************************************
 * TCPClient (destructor) - No cleanup right now
 *
 **********************************************************************************************/

TCPClient::~TCPClient() {

}

char* hello = (char*)"Hello from client";
char buffer[1024],msg[1024] = { 0 };

/**********************************************************************************************
 * connectTo - Opens a File Descriptor socket to the IP address and port given in the
 *             parameters using a TCP connection.
 *
 *    Throws: socket_error exception if failed. socket_error is a child class of runtime_error
 **********************************************************************************************/
void TCPClient::connectTo(const char *ip_addr, unsigned short port) {

	if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
	}

	this->serv_addr.sin_family = AF_INET;
	this->serv_addr.sin_port = htons(port);

	// Convert IPv4 and IPv6 addresses from text to binary form 
	if (inet_pton(AF_INET, "127.0.0.1", &this->serv_addr.sin_addr) <= 0)
	{
		printf("\nInvalid address/ Address not supported \n");
	}

	if (connect(sock, (struct sockaddr*) &this->serv_addr, sizeof(this->serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
	}
	cout << "CONNECTED TO SERVER" << endl;
	send(this->sock, hello, strlen(hello), 0);
	printf("Hello message sent\n");
	this->valread = read(this->sock, buffer, 1024);
	printf("%s\n", buffer);
}

/**********************************************************************************************
 * handleConnection - Performs a loop that checks if the connection is still open, then 
 *                    looks for user input and sends it if available. Finally, looks for data
 *                    on the socket and sends it.
 * 
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/
void TCPClient::handleConnection() {
	int bytesRead, bytesWritten = 0;
	struct timeval start1, end1;
	gettimeofday(&start1, NULL);

	//loop to check for open connection
	while (true) {
		cout << ">";
		string data;
		getline(cin, data);
		memset(&msg, 0, sizeof(msg)); //clear buffer
		strcpy(msg, data.c_str());
		if (data == "exit")
		{
			send(this->sock, (char*)&msg, strlen(msg), 0);
			break;
		}
		bytesWritten += send(this->sock, (char*)&msg, strlen(msg), 0);
		cout << "Awaiting server response..." << endl;
		memset(&msg, 0, sizeof(msg)); //clear buffer
		bytesRead += recv(this->sock, (char*)&msg, sizeof(msg), 0);
		if (!strcmp(msg, "exit"))
		{
			cout << "Server has quit the session" << endl;
			break;
		}
		cout << "Server: " << msg << endl;
	} //end loop
	cout << "********Session********" << endl;
	cout << "Bytes written: " << bytesWritten <<
		" Bytes read: " << bytesRead << endl;
	cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec)
		<< " secs" << endl;
	cout << "Connection closed" << endl;
}

/**********************************************************************************************
 * closeConnection - Your comments here
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPClient::closeConn() {
	if (close(this->sock) < 0 ) {
		perror("client shutdown");
		exit(EXIT_FAILURE);
	}
}


