#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include "Server.hpp"

int main(int ac, char **av)
{
	if (ac == 3)
	{
		Server ser;
		std::cout << "---- SERVER ----" << std::endl;
		try{
			signal(SIGINT, Server::SignalHandler); //-> catch the signal (ctrl + c)
			signal(SIGQUIT, Server::SignalHandler); //-> catch the signal (ctrl + \)
			ser.ServerInit(atoi(av[1])); //-> initialize the server
		}
		catch(const std::exception& e){
			ser.CloseFds(); //-> close the file descriptors
			std::cerr << e.what() << std::endl;
		}
		std::cout << "The Server Closed!" << std::endl;
	}
	else
		std::cout << "Valid Args : './ircserv <port> <password>'";
}
