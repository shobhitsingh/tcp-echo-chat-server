/*
 * ServerFactory.h
 *
 *  Created on: Mar 6, 2016
 *      Author: shobhit
 */

#ifndef SERVERFACTORY_H_
#define SERVERFACTORY_H_

#include <string>


#include "ProxyServer.h"
#include "BackendServer.h"

namespace es {

class ServerFactory {
public:
	Server* GetServer(std::string &port)
	{
		//return new ProxyServer(port) // in future;
		return new BackendServer(port) ;
	}

};
}


#endif /* SERVERFACTORY_H_ */
