//============================================================================
// Name        : echoServer.cpp
// Author      : Shobhit
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <iostream>
#include <cstdlib>

#include "server/ServerFactory.h"
#include "server/Server.h"

using namespace es;

static void ShowUsage(std::string name)
{
    std::cerr << "Usage: " << name << " <option(s)> SOURCES"
              << "Options:\n"
              << "\t-h,--help\t\tShow this help message\n"
              << "\t-p,--port PORT\tPort Number For running Server\n"
              << std::endl;
}


int main(int argc, char *argv[]) {

	if (argc < 2)
	{
		ShowUsage(argv[0]);
		std::exit(EXIT_FAILURE);
	}

	std::string port;

	//Adding for loop here, in future I will be adding proxy here with more params
	for(int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];
		if((arg == "-p") || (arg == "--port"))
		{
			port = argv[++i];
		}
	}

	std::cout<<"Server Listening on: "<<port<<std::endl;

	ServerFactory factory;
	Server *main = factory.GetServer(port);
	main->StartServer();
	delete main;

	return EXIT_SUCCESS;

}
