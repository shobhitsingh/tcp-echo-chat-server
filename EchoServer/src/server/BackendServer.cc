/*
 * BackendServer.cc
 *
 *  Created on: Mar 6, 2016
 *      Author: shobhit
 */

#include "BackendServer.h"
#include "ServerThreadMediator.h"


namespace es {

void BackendServer::Execute(int sockfd)
{
	//create std thread
	ServerThreadMediator worker(sockfd);
	worker.CreateIOAndBackendThreads(ioThreadCount);
}

}
