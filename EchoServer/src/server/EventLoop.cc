/*
 * EventLoop.cc
 *
 *  Created on: Mar 5, 2016
 *      Author: shobhit
 */
#include <iostream>
#include <cassert>
#include <utility>

#include "EventLoop.h"
#include "Socket.h"


namespace es{

int EventLoop::CreateEventLoop()
{
#ifdef WIN32
	//IOCP code
#else
	if((pollfd = epoll_create(1)) < 0)
	{
		return -1;
	}
	struct epoll_event epEvent;
	epEvent.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLET;
	epEvent.data.fd = sockfd;

	if(epoll_ctl(pollfd, EPOLL_CTL_ADD, sockfd, &epEvent) < 0)
	{
		return -1;
	}

#endif
	return 0;
}

/*
 * Main Event Loop. Each thread has their own Epoll File descriptor
 */
void EventLoop::RunEventLoop()
{
	const int eventBufSize = 256;
#ifdef WIN32
	//IOCP code
#else
	struct epoll_event *events = (epoll_event*)malloc(sizeof(*events)*eventBufSize);
	if (events == NULL)
	{
		return;
	}
	int eventsCount;
	while ((eventsCount = epoll_wait(pollfd, events, eventBufSize, -1)) > 0)
	{
		int i;
		for (i = 0; i < eventsCount; i++)
		{
			if ((events[i].events & EPOLLERR) ||
					(events[i].events & EPOLLHUP))
			{
				CloseConnection(pollfd, events[i].data.fd);
				continue;
			}
			if (events[i].data.fd == sockfd)
			{
				//Received something on listening socket
				AcceptNewClient();
			}
			else
			{
				//Received Data from accepted socket
				if (HandleClientData(events[i].data.fd) < 0)
				{
					CloseConnection(pollfd, events[i].data.fd);
				}
			}
		}
	}
	close(sockfd);
#endif

}

/*
 * Accept New Client, which will create a new file descriptor
 */
int EventLoop::AcceptNewClient()
{
	std::pair<int,struct sockaddr_in> pair = Socket::GetInstance().Accept(sockfd);

	if (pair.first < 0)
	{
		return -1;
	}

	char ip_buff[INET_ADDRSTRLEN+1];
	if (inet_ntop(AF_INET, &pair.second.sin_addr, ip_buff, sizeof(ip_buff)) == NULL)
	{
		Socket::GetInstance().Close(pair.first);
		return -1;
	}

	SessionInfo info;
	info.address = ip_buff;
	info.port = ntohs(pair.second.sin_port);

	//Add to Connection Manager Map
	sessionPtr.get()->AddSession(pair.first,info);


	std::cout<<"Client connected from "<<ip_buff<<":"<<ntohs(pair.second.sin_port)<<std::endl;

	struct epoll_event epevent;
	epevent.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLET;
	epevent.data.fd = pair.first;

	if (epoll_ctl(pollfd, EPOLL_CTL_ADD, pair.first, &epevent) < 0)
	{
		Socket::GetInstance().Close(pair.first);
		return -1;
	}

	return 0;

}

/*
 * Handle Client Data and adds the client details to session
 * and add the client message to the MessageQueue which will
 * be processed by the MessageProcessor
 */
int EventLoop::HandleClientData(int clientfd)
{
	unsigned char readbuff[512];
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	ssize_t size;

	if ((size = Socket::GetInstance().Recv(clientfd, readbuff, sizeof(readbuff)-1)) < 0)
	{
		return -1;
	}

	if (getpeername(clientfd, (struct sockaddr *) &addr, &addrlen) < 0)
	{
		return -1;
	}

	char ip_buff[INET_ADDRSTRLEN+1];
	if (inet_ntop(AF_INET, &addr.sin_addr, ip_buff, sizeof(ip_buff)) == NULL)
	{
		return -1;
	}

	if (size == 0)
	{
		return 0;
	}

	//packet Handling [ {TotalLength=2},{UserNameLength=2},{<UserName>},{Variable Message} ]

	while(size >= length)
	{
		//total message length
		int packetLen = UnpackData(readbuff);
		if((size < packetLen) || (packetLen == 0))
			break;
		int userNameLen = UnpackData(readbuff+length);

		char userName[userNameLen];
		std::memset(userName, 0, sizeof(char)*userNameLen);
		std::memcpy(userName,readbuff+length*length,userNameLen);
		userName[userNameLen] = '\0';

		int msgLen = packetLen -(length*length) - userNameLen;
		char msg[msgLen];
		std::memset(msg, 0, sizeof(char)*msgLen);
		std::memcpy(msg,readbuff+(length*length)+userNameLen,msgLen);
		msg[msgLen] = '\0';

		//Add to Connection Manager Map
		sessionPtr.get()->AddUserName(clientfd,userName);

		std::string message = msg;
		//Add socket and Msg to queue
		msgQueue.Push(std::make_pair(clientfd,message));

		size = size - (length + packetLen);
		if(size > 0)
		{
			std::memmove(readbuff,readbuff + length+ packetLen, size);
		}
	}
	return 0;
}

/*
 * Remove client socket from epoll and remove session info
 */
void EventLoop::CloseConnection(int efd, int cfd)
{
	std::cout<<"Connection Close "<<cfd<<std::endl;
	sessionPtr.get()->RemoveSessionInfo(cfd);
	epoll_ctl(efd, EPOLL_CTL_DEL, cfd, NULL);
	close(cfd);
}

/*
 * Utility for Integer packing
 */
int EventLoop::UnpackData(unsigned char *buf)
{
	unsigned int i2 = ((unsigned int)buf[0]<<8) | buf[1];
	int i;

	if (i2 <= 0x7fffu) { i = i2; }
	else { i = -1 - (unsigned int)(0xffffu - i2); }

	return i;
}


}

