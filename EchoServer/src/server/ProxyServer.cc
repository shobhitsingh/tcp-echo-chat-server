/*
 * ProxyServer.cc
 *
 *  Created on: Mar 6, 2016
 *      Author: shobhit
 */

#include "ProxyServer.h"
#include "ServerThreadMediator.h"
namespace es {


//Not used, in future will add proxy as well
void ProxyServer::Execute(int sockfd)
{
	//create std thread
/*	std::string host;

	if(!commandLineData.second.second.empty())
		host = commandLineData.second.second.front();

	int backfd = ConnectionHandler::getInstance().CreateAndConnect(host,commandLineData.second.first);

	std::pair<int, int> pair;
	pair.first = sockfd;
	pair.second = backfd;
	ServerThreadMediator worker(pair, commandLineData);
	worker.CreateIOAndBackendThreads(ioThreadCount);
	*/



}
}



