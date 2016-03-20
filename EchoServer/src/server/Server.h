/*
 * Server.h
 *
 *  Created on: Mar 6, 2016
 *      Author: shobhit
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <string>

namespace es{

class Server
{
private:

public:
	virtual ~Server(){}
	void StartServer();
	virtual void Execute(int sockfd);
protected:
	std::string port;
	Server(std::string& port): port(port){}
};
}



#endif /* SERVER_H_ */
