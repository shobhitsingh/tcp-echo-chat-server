/*
 * ServerThreadMediator.h
 *
 *  Created on: Mar 5, 2016
 *      Author: shobhit
 */

#ifndef SERVERTHREADMEDIATOR_H_
#define SERVERTHREADMEDIATOR_H_
#include <vector>
#include <string>
#include <utility>
#include <memory>
#include "SessionManager.h"
#include "MessageQueue.h"

namespace es {

typedef std::shared_ptr<SessionManager> SessionMgrPtr;
typedef MessageQueue <std::pair<int,std::string>> MsgQueue;

/*
 * Main Interface between Socket classes and IO worker and Message
 * processing worker. Waits for all thread to finish. Main Blocked
 * in CreateIOAndBackendThreads. Considered only 1 Message processor
 * and multiple IO Worker, which is passed from the BackednServer
 */

class ServerThreadMediator
{
public:
	ServerThreadMediator(int sockfd) : sockfd(sockfd)
	{}
	void CreateIOThreads(const int &count);

	void CreateIOAndBackendThreads(const int &count);

	void AddSession(int fd);
	void RemoveSession(int fd);

private:
	int sockfd;
};

}

#endif /* SERVERTHREADMEDIATOR_H_ */
