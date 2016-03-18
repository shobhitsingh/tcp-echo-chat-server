/*
 * BackendServer.h
 *
 *  Created on: Mar 6, 2016
 *      Author: shobhit
 */

#ifndef BACKENDSERVER_H_
#define BACKENDSERVER_H_

#include "Server.h"

namespace es {

class BackendServer : public Server
{
private:
	const int threadCount = 3;
public:
	BackendServer(std::string port): Server(port){}
	void Execute(int sockfd);
};
}



#endif /* BACKENDSERVER_H_ */
