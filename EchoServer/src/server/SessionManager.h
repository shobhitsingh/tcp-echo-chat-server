/*
 * SessionManager.h
 *
 *  Created on: Mar 13, 2016
 *      Author: shobhit
 */

#ifndef SESSIONMANAGER_H_
#define SESSIONMANAGER_H_

#include <map>
#include <mutex>
#include <condition_variable>

#include "SessionInfo.h"

namespace es {

class SessionManager
{
public:
	SessionManager(): sessionMap(), mutex(), cond(){}
	void AddSession(int socket , SessionInfo sessionInfo)
	{
		std::lock_guard<std::mutex> lock(mutex);
		sessionMap.insert(std::map<int, SessionInfo>::value_type(socket, sessionInfo));
		cond.notify_one();
	}

	SessionInfo GetSessionInfo(int socket)
	{
		return sessionMap.find(socket)->second;
	}

	void RemoveSessionInfo(int sockfd)
	{
		std::lock_guard<std::mutex> lock(mutex);

		std::map<int,SessionInfo>::iterator it;
		it = sessionMap.find (sockfd);
		sessionMap.erase(it, sessionMap.end());
		cond.notify_one();
	}

	void AddUserName(int sockfd, std::string userName)
	{
		std::map<int, SessionInfo>::iterator it = sessionMap.find(sockfd);
		if (it != sessionMap.end())
			it->second.userName = userName;
	}

	std::map<int,SessionInfo>& GetSessionMap()
	{
		return sessionMap;
	}

private:
	std::map<int,SessionInfo> sessionMap;
	mutable std::mutex mutex;
	std::condition_variable cond;
};

}



#endif /* SESSIONMANAGER_H_ */
