/*
 * socket.cpp
 *
 *  Created on: Mar 4, 2016
 *      Author: shobhit
 */
#include<iostream>
#include "Socket.h"

#ifdef WIN32
      typedef int socklen_t;
#endif

namespace es
{

int Socket::CreateAndBind(std::string& port)
{
#ifdef WIN32
	WSADATA wsa_data;
	int n= WSAStartup(MAKEWORD(2, 0), &wsa_data);
	if(n == -1) error(n);
#endif
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int s, sfd;

	memset(&hints, 0, sizeof (struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	s = getaddrinfo(NULL, port.c_str(), &hints, &result);
	if (s != 0)
	{
		return -1;
	}

	for (rp = result; rp != NULL; rp = rp->ai_next)
	{
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
	    if (sfd == -1)
	    	continue;
	    s = bind(sfd, rp->ai_addr, rp->ai_addrlen);
	    if (s == 0)
	    {
	    	break;
	    }
	    close(sfd);
	 }

	 if (rp == NULL)
	 {
		 return -1;
	 }

	 freeaddrinfo (result);
	 return sfd;
}

int Socket::CreateAndConnect(std::string& host, std::string& port)
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if ((rv = getaddrinfo(host.c_str(), port.c_str(), &hints, &servinfo)) != 0) {
		return -1;
	}

	for(p = servinfo; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1)
		{
			continue;
		}
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			continue;
		}
		break;
	}

	if (p == NULL)
	{
		return -1;
	}
	inet_ntop(p->ai_family, GetAddr((struct sockaddr *)p->ai_addr),
	s, sizeof s);
	freeaddrinfo(servinfo);
	return sockfd;
}

bool Socket::SetSocketNonBlocking(int sockfd)
{
	bool ret = true;

#ifdef WIN32
	 u_long flags = is_blocking ? 0 : 1;
	 ret = NO_ERROR == ioctlsocket(sockfd, FIONBIO, &flags);
#else
	 int flags, s;

	 flags = fcntl(sockfd, F_GETFL, 0);
	 if (flags == -1)
	 {
		 ret = false;
	 }
	 else
	 {
		 flags |= O_NONBLOCK;
		 s = fcntl(sockfd, F_SETFL, flags);
		 if (s == -1)
			 ret = false;
	 }
#endif
	 return ret;
}

int Socket::Listen(int sockfd, int backlog)
{
	return listen(sockfd, backlog);
}


std::pair<int,struct sockaddr_in> Socket::Accept(int sockfd)
{
	int new_fd;
	std::pair<int,struct sockaddr_in> pair;
    struct sockaddr_in their_addr;

    socklen_t size = sizeof(their_addr);

    new_fd = accept(sockfd, (struct sockaddr*)&their_addr, &size);
    pair.first = new_fd;
    pair.second = their_addr;
    return pair;
}

int Socket::Connect(int sockfd, const std::string& host, const std::string& port)
{
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if ((rv = getaddrinfo(host.c_str(), port.c_str(), &hints, &servinfo)) != 0) {
		return -1;
	}

	for(p = servinfo; p != NULL; p = p->ai_next)
	{
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			continue;
		}
		break;
	}

	if (p == NULL)
	{
		return -1;
	}
	inet_ntop(p->ai_family, GetAddr((struct sockaddr *)p->ai_addr),
		s, sizeof s);
	freeaddrinfo(servinfo);
	return 0;

}

int Socket::Send(int clientfd, const void *buf, size_t len)
{
	int ret;
	ret = send(clientfd, buf, len, 0);
    return ret;
}

int Socket::Recv(int clientfd, void *buf, int len)
{
	int n = recv(clientfd, buf, len, 0);
    if(n <= 0) return -1;

    return n;
}

void Socket::Close(int sockfd)
{
#ifdef WIN32
	WSACleanup();
    closesocket(sockfd);
#else
    close(sockfd);
#endif
}

void* Socket::GetAddr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int Socket::Shutdown(int sockfd, int type)
{
	int n;
#ifdef WIN32
	WSACleanup();
#endif

    n = shutdown(sockfd, type);
    return n;

}

}



