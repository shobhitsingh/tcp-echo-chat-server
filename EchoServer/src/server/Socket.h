/*
 * socket.h
 *
 *  Created on: Mar 4, 2016
 *      Author: shobhit
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <string>
#include <utility>

#ifdef WIN32
#include <winsock.h>
#else

#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <fcntl.h>


#endif

namespace es
{

class Socket
{
private:
	Socket(){}
	Socket(Socket const&);
	void operator=(Socket const&);
public:

	static Socket& GetInstance()
	{
		static Socket instance;
		return instance;
	}

    int Listen(int sockfd, int backlog = 5);
    std::pair<int,struct sockaddr_in> Accept(int sockfd);
    int Connect(int sockfd, const std::string& host, const std::string& port);
    int Send(int clientfd, const void *buf, size_t len);
    int Recv(int clientfd, void *buf, int len);
    void Close(int sockfd);
    int Shutdown(int sockfd, int type);
    int CreateAndBind(std::string& port);
    int CreateAndConnect(std::string& host, std::string& port);
    bool SetSocketNonBlocking(int sockfd);

 private:
    static void* GetAddr(struct sockaddr *sa);
};

}


#endif /* SOCKET_H_ */
