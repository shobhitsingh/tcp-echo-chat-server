/*
 * MessageProcessor.h
 *
 *  Created on: Mar 14, 2016
 *      Author: shobhit
 */

#ifndef MESSAGEPROCESSOR_H_
#define MESSAGEPROCESSOR_H_

#include "ServerThreadMediator.h"
#include "ConnectionHandler.h"

namespace es {

class MessageProcessor
{
public:
	MessageProcessor(SessionMgrPtr session, MsgQueue& queue):
		sessionPtr(session),msgQueue(queue){}
	void StartProcessing()
	{
		while(1)
		{
			std::pair<int,std::string> pair = msgQueue.Pop();
			//fetch user name
			SessionInfo info = sessionPtr.get()->GetSessionInfo(pair.first);
			std::string msg = info.userName + ":" + pair.second;

			std::map<int,SessionInfo>& m = sessionPtr.get()->GetSessionMap();;
			for(std::map<int,SessionInfo>::iterator it = m.begin(); it != m.end(); ++it)
			{
				if(it->first != pair.first)
				{
					ConnectionHandler::getInstance().SendData(it->first,msg);
				}
			}
		}
	}
private:
	SessionMgrPtr sessionPtr;
	MsgQueue& msgQueue;

};

}


#endif /* MESSAGEPROCESSOR_H_ */
