/*
 * EventLoop.h
 *
 *  Created on: Mar 5, 2016
 *      Author: shobhit
 */

#ifndef EVENTLOOP_H_
#define EVENTLOOP_H_

#include <utility>

#include "ServerThreadMediator.h"

#ifdef WIN32
//IOCP includes
#else
#include <sys/epoll.h>
#endif

namespace es{

class EventLoop
{
public:
	EventLoop(int fd, SessionMgrPtr session, MsgQueue& msgQueue): pollfd(int()), sockfd(fd),
				sessionPtr(session),msgQueue(msgQueue){}
	int CreateEventLoop();
	void RunEventLoop();

private:
#ifdef WIN32
//IOCP includes
#else
	int AcceptNewClient();
	int HandleClientData(int clientfd);
	void CloseConnection(int efd, int cfd);
	void HandleBackendRead(){}
#endif
	int UnpackData(unsigned char *buf);



#ifdef WIN32
	//ICOP variables
#else
	int pollfd;
#endif
	int sockfd;
	SessionMgrPtr sessionPtr;
	MsgQueue& msgQueue;
	const int length =2;
};

}


#endif /* EVENTLOOP_H_ */
