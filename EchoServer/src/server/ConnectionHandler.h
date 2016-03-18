/*
 * ConnectionHandler.h
 *
 *  Created on: Mar 5, 2016
 *      Author: shobhit
 */

#ifndef CONNECTIONHANDLER_H_
#define CONNECTIONHANDLER_H_


namespace es {

class ConnectionHandler
{
private:
	ConnectionHandler(){}
	ConnectionHandler(ConnectionHandler const&);
	void operator=(ConnectionHandler const&);
public:
	static ConnectionHandler& getInstance()
	{
		static ConnectionHandler instance;
	    return instance;
	}


	int CreateAndBind(std::string& port);
	int CreateAndConnect(std::string& host, std::string& port);
	int SendData(int sockfd, std::string msg);

private:

	void SetExitMode(const std::string& msg);

};

}



#endif /* CONNECTIONHANDLER_H_ */
