/*
 * ServerThreadMediator.cc
 *
 *  Created on: Mar 5, 2016
 *      Author: shobhit
 */
#include <thread>

#include <list>
#include "ServerThreadMediator.h"
#include "EventLoop.h"
#include "MessageProcessor.h"


namespace es {

/*
 * IO Worker event loop and each thread has their own epoll descriptors
 */
void RunEventLoop(int sockfd, SessionMgrPtr p, MsgQueue& msgQueue)
{
	EventLoop evLoop(sockfd,p, msgQueue);
	if(evLoop.CreateEventLoop() >= 0)
	{
		evLoop.RunEventLoop();
	}
}

void RunMessageProcessorLoop(SessionMgrPtr p, MsgQueue& msgQueue)
{
	MessageProcessor msgLoop(p, msgQueue);
	msgLoop.StartProcessing();
}


void ServerThreadMediator::CreateIOThreads(const int &count)
{

}

/*
 * Create first backend thread which reads messages and sends back to the client
 * Then create IO thread which waits on epoll fd and waits for incoming connection
 * and adds to the Message queue
 */
void ServerThreadMediator::CreateIOAndBackendThreads(const int &count)
{
	//start first backend thread

	MsgQueue msgQueue;

	//trying out c++ shared ptr concept
	SessionMgrPtr sessionMgr = std::make_shared<SessionManager>();

	std::thread messageProcessor(std::bind(RunMessageProcessorLoop, sessionMgr, std::ref(msgQueue)));

	//CreateIOThreads(count,p);
	std::thread ioWorkers[count];

	for (int i = 0; i < count; ++i)
	{
		ioWorkers[i] = std::thread(std::bind(RunEventLoop, sockfd,sessionMgr,std::ref(msgQueue)));
	}

	for (int i = 0; i < count; ++i)
	{
		ioWorkers[i].join();
	}

	messageProcessor.join();

}

}


