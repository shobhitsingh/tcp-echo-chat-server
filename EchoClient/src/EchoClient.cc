/*
 * EchoClient.cc

 *
 *  Created on: Mar 14, 2016
 *      Author: shobhit
 */
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <string>
#include <utility>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <thread>

#define BUF_SIZE 100

static void ShowUsage(std::string name)
{
	std::cerr << "Usage: " << name << " <option(s)> SOURCES"
			<< "Options:\n"
			<< "\t-h,--help\t\tShow this help message\n"
			<< "\t-p,--port PORT\tPort Number of remote Server\n"
			<< "\t-u,--username USERNAME\tUser Name\n"
			<< "\t-d,--destination DESTINATION\tServer Addresses separted by space "
					"for failover"
			"{First one gest picked first}\n"
			<< std::endl;
}
//Duplicated the same function from EchoServer
//Todo use socket.h from EchoServer
void* GetAddr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int Connect(int sockfd, const std::string& host, const std::string& port)
{
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if ((rv = getaddrinfo(host.c_str(), port.c_str(), &hints, &servinfo)) != 0) {
		return -1;
	}

	for(p = servinfo; p != NULL; p = p->ai_next)
	{
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			continue;
		}
		break;
	}

	if (p == NULL)
	{
		return -1;
	}
	inet_ntop(p->ai_family, GetAddr((struct sockaddr *)p->ai_addr),
			s, sizeof s);
	freeaddrinfo(servinfo);
	return 0;

}

int CreateAndConnect(std::string& host, std::string& port)
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if ((rv = getaddrinfo(host.c_str(), port.c_str(), &hints, &servinfo)) != 0) {
		return -1;
	}

	for(p = servinfo; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1)
		{
			continue;
		}
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			continue;
		}
		break;
	}

	if (p == NULL)
	{
		return -1;
	}
	inet_ntop(p->ai_family, GetAddr((struct sockaddr *)p->ai_addr),
			s, sizeof s);
	freeaddrinfo(servinfo);
	return sockfd;
}

/*
 * Not tested with multiple backends,
 * basically from the lists of address itr is trying to connect
 * to another server.
 */
void GetNewBackendAddress(std::vector <std::string>& destinationAddress,std::string& active)
{
	int max = destinationAddress.size();
	if(max == 1)
	{
		//only one address, try with the same again
		return;
	}
	int min =0;
	while(true)
	{
		int index = min + (rand() % (int)(max - min + 1));
		std::string remoteAddress = destinationAddress[index];
		if(remoteAddress.compare(active) != 0)
		{
			active = remoteAddress;
			return;
		}
	}
}

void ConnectNew(int sockfd, std::string port, std::vector <std::string> destinationAddress,
		std::string& active)
{
	//disconnect, connect to another server
	GetNewBackendAddress(destinationAddress,active);
	std::cout<<"socket="<<sockfd<<std::endl;
	std::cout<<"port="<<port<<std::endl;
	std::cout<<"active="<<active<<std::endl;

	if(Connect(sockfd,active,port) < 0)
	{
		std::cout<<"Cannot connect to another server"<<std::endl;
	}

}



void ReceiveMessage(int sockfd, std::string port, std::vector <std::string> destinationAddress) {
	int ret;
	std::string active(destinationAddress[0]);
	char buffer[BUF_SIZE];
	memset(buffer, 0, BUF_SIZE);
	for (;;) {
		ret = recv(sockfd, buffer, BUF_SIZE, 0);
		if (ret < 0) {
			std::cout<<"Connecting to new server!"<<std::endl;
			ConnectNew(sockfd, port, destinationAddress, active);
		}
		else if(ret == 0)
		{
			ConnectNew(sockfd, port, destinationAddress, active);
		}
		else {
			fputs(buffer, stdout);
			memset(buffer, 0, BUF_SIZE);
		}
	}
}

void packi16(unsigned char *buf, unsigned int i)
{
	*buf++ = i>>8; *buf++ = i;
}


int main(int argc, char**argv) {
	char buffer[BUF_SIZE];
	int sockfd, ret;

	if (argc < 3)
	{
		ShowUsage(argv[0]);
		std::exit(EXIT_FAILURE);
	}

	std::vector <std::string> destinationAddress;
	std::string port;
	std::string userName;
	for(int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];
		if((arg == "-h") || (arg == "--help"))
		{
			ShowUsage(argv[0]);
			std::exit(EXIT_SUCCESS);
		}
		else if((arg == "-p") || (arg == "--port"))
		{
			port = argv[++i];
		}
		else if((arg == "-u") || (arg == "--username"))
		{
			userName = argv[++i];
		}
		else if((arg == "-d") || (arg == "--destination"))
		{
			for(int j = i + 1; j < argc; ++j)
			{
				destinationAddress.push_back(argv[j]);
			}

		}
	}

	sockfd = CreateAndConnect(destinationAddress[0], port);
	std::cout<<"Connected to the server..."<<std::endl;

	memset(buffer, 0, BUF_SIZE);
	std::cout<<"Enter your messages one by one and press return key"<<std::endl;

	//creating a new thread for receiving messages from the server
	std::thread t(std::bind(ReceiveMessage, sockfd, port, destinationAddress));


	while (fgets(buffer, BUF_SIZE, stdin) != NULL) {
		unsigned char buf[BUF_SIZE] = {0};
		int msgLen = strlen(buffer);
		int userNameLen = userName.length();

		unsigned int totalLength = msgLen + userNameLen +4;
		packi16(buf,totalLength);
		int offset = 0;
		offset += 2;

		packi16(buf+offset,userNameLen);
		offset += 2;
		memcpy(buf+offset,userName.c_str(),userNameLen);
		offset += userNameLen;
		memcpy(buf+offset,buffer,msgLen);

		ret = send(sockfd, buf, totalLength, 0);
		if (ret < 0) {
			printf("Error sending data!\n\t-%s", buffer);
		}
		memset(&buffer, 0, sizeof buffer);
	}

	std::cout<<"Exiting & closing"<<std::endl;
	close(sockfd);

	return 0;
}



