/*
 * SessionInfo.h
 *
 *  Created on: Mar 13, 2016
 *      Author: shobhit
 */

#ifndef SESSIONINFO_H_
#define SESSIONINFO_H_

#include <string>

namespace es {
struct SessionInfo
{
	mutable std::string userName;
	std::string address;
	std::string port;
};

}


#endif /* SESSIONINFO_H_ */
