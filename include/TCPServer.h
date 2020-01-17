#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "Server.h"
#include <netinet/in.h>

class TCPServer : public Server 
{
public:
   TCPServer();
   ~TCPServer();

   void bindSvr(const char *ip_addr, unsigned short port) override;
   void listenSvr() override;
   void shutdown() override;

   int server_fd, new_socket, valread;
   struct sockaddr_in address;
   int addrlen = sizeof(address);

private:

};


#endif
