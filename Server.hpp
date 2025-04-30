#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <csignal>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <poll.h>
#include "Clients.hpp"

#define RED "\e[1;31m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define YEL "\e[1;33m"

class Server
{
	private:
		int Port;
		int ServSocket;
		static bool Signal;
		char *Mdp;
		std::vector<Client> clients;
		std::vector<struct pollfd> fds;

	public:
		Server(){ServSocket = -1;} 

		void ServerInit(int port, char *mdp);
		void SerSocket();
		void AcceptIncomingClient();
		void ReceiveDataClient(int fd);
		int	ParseData(int fd, char *buff);
		void HandleCmd(int fd, std::string str, int i, char *buffer);

		static void SignalHandler(int signum);
	
		void CloseFds();
		void ClearClients(int fd);
};
