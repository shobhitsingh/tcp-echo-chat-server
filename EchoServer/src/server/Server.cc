/*
 * Server.cc
 *
 *  Created on: Mar 12, 2016
 *      Author: shobhit
 */

#include "Server.h"
#include "ConnectionHandler.h"

namespace es {
void Server::StartServer()
{
	Execute(ConnectionHandler::getInstance().CreateAndBind(port));
}
void Server::Execute(int sockfd){}
}



