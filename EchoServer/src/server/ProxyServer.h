/*
 * ProxyServer.h
 *
 *  Created on: Mar 6, 2016
 *      Author: shobhit
 */

#ifndef PROXYSERVER_H_
#define PROXYSERVER_H_

#include "Server.h"

namespace es {

class ProxyServer : public Server
{
private:
	const int threadCount = 1;
public:
	ProxyServer(std::string &port): Server(port){}

	void Execute(int sockfd);
};

}


#endif /* PROXYSERVER_H_ */
