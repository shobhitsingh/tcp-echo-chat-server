/*
 * ConnectionHandler.cc
 *
 *  Created on: Mar 5, 2016
 *      Author: shobhit
 */
#include <stdexcept>
#include <iostream>
#include "ConnectionHandler.h"
#include "Socket.h"

namespace es {

int ConnectionHandler::CreateAndBind(std::string& port)
{
	//create listening socket and set up initial connection
	//initialize socket using wrapper
	//create OS specific

	int sockfd = Socket::GetInstance().CreateAndBind(port);

	if(sockfd == -1)
	{
		SetExitMode("cannot Bind or connect to Socket");
	}
	//set socket non blocking
	if(!Socket::GetInstance().SetSocketNonBlocking(sockfd))
	{
		SetExitMode("cannot set to non blocking! Server only works in Non Blocking Mode");
	}

	if(Socket::GetInstance().Listen(sockfd) == -1)
		SetExitMode("cannot set listening socket");

	return sockfd;
}

int ConnectionHandler::CreateAndConnect(std::string& host, std::string& port)
{
	int sockfd = Socket::GetInstance().CreateAndConnect(host,port);

	if(sockfd == -1)
	{
		SetExitMode("cannot Bind or connect to Socket");
	}
	//set socket non blocking
	if(!Socket::GetInstance().SetSocketNonBlocking(sockfd))
	{
		SetExitMode("cannot set to non blocking! Server only works in Non Blocking Mode");
	}

	return sockfd;
}

void ConnectionHandler::SetExitMode(const std::string& msg)
{
	std::cerr<<msg<<std::endl;
	std::exit(EXIT_FAILURE);
}

int ConnectionHandler::SendData(int sockfd, std::string msg)
{
	if (Socket::GetInstance().Send(sockfd,msg.c_str(),msg.length()) < 0)
	{
		return -1;
	}
	return 0;
}

}



