#include "Server.hpp"
#include <string.h>

bool Server::Signal = false;
void Server::SignalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::Signal = true;
}

void Server::CloseFds(){
	for(size_t i = 0; i < clients.size(); i++)
	{
		std::cout << RED << "Client <" << this->clients[i].GetFd() << "> Disconnected" << WHI << std::endl;
		close(this->clients[i].GetFd());
	}
	if (this->ServSocket != -1)
	{
		std::cout << RED << "Server <" << this->ServSocket << "> Disconnected" << WHI << std::endl;
		close(this->ServSocket);
	}
}

void Server::SerSocket()
{
	struct sockaddr_in add;
	struct pollfd NewPoll;
	add.sin_family = AF_INET;
	add.sin_port = htons(this->Port);
	add.sin_addr.s_addr = INADDR_ANY;
	int en = 1;

	this->ServSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(this->ServSocket == -1)
		throw(std::runtime_error("faild to create socket"));

	if(setsockopt(this->ServSocket, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
		throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
	if (fcntl(this->ServSocket, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
	if (bind(this->ServSocket, (struct sockaddr *)&add, sizeof(add)) == -1)
		throw(std::runtime_error("faild to bind socket"));
	if (listen(this->ServSocket, SOMAXCONN) == -1)
		throw(std::runtime_error("listen() faild"));

	NewPoll.fd = ServSocket;
	NewPoll.events = POLLIN;
	NewPoll.revents = 0;
	this->fds.push_back(NewPoll);
}

void Server::AcceptIncomingClient()
{
	Client cli;
	struct sockaddr_in cliadd;
	struct pollfd NewPoll;
	socklen_t len = sizeof(cliadd);

	int incofd = accept(this->ServSocket, (sockaddr *)&(cliadd), &len);
	if (incofd == -1)
	{
			std::cout << "accept() failed" << std::endl; 
			return;
	}
	if (fcntl(incofd, F_SETFL, O_NONBLOCK) == -1)
	{
			std::cout << "fcntl() failed" << std::endl; 
			return;
	}
	NewPoll.fd = incofd;
	NewPoll.events = POLLIN;
	NewPoll.revents = 0;

	cli.SetFd(incofd);
	cli.setIpAdd(inet_ntoa((cliadd.sin_addr)));
	this->clients.push_back(cli);
	this->fds.push_back(NewPoll);
	std::cout << GRE << "Client <" << incofd << "> Connected" << WHI << std::endl;
}

void Server::HandleCmd(int fd, std::string str, int i, char *buff)
{
	if (str == "CAP")
		return ;
	else if (str == "PASS")
	{
		std::string arg;
		while (buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\0')
		{
			arg += buff[i];
			i++;
		}
		arg[strlen(arg.c_str()) - 1] = '\0';
		if (strcmp((const char *)arg.c_str(), (const char *)this->Mdp))
			std::cout << "Wrong password" << std::endl;
	}
	else if (str == "NICK")
	{
		std::string arg;
		while (buff[i] != '\n' && buff[i] != '\0')
		{
			arg += buff[i];
			i++;
		}
		std::cout << arg << std::endl;
		for(size_t i = 0; i < this->clients.size(); i++)
		{
			if (this->clients[i].GetFd() == fd)
			{
					this->clients[i].Setnick(arg);
					break;
			}
		}	
	}
	else if (str == "USER")
	{
		std::string arg;
		while (buff[i] != '\n' && buff[i] != '\0')
		{
			arg += buff[i];
			i++;
		}
		std::cout << arg << std::endl;
		for(size_t i = 0; i < this->clients.size(); i++)
		{
			if (this->clients[i].GetFd() == fd)
			{
					this->clients[i].Setuser(arg);
					break;
			}
		}	
	}
}

int Server::ParseData(int fd, char *buff)
{
	std::string str;
	int i = 0;
	while (buff[i] != '\0')
	{
		while (buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\0')
		{
			str += buff[i];
			i++;
		}
		i++;
		HandleCmd(fd, str, i, buff);
		while (buff[i] != '\n' && buff[i] != '\0')
			i++;
		str = "";
		i++;
	}
	return (0);
}

void Server::ReceiveDataClient(int fd)
{
	char buff[1024];
	memset(buff, 0, sizeof(buff));

	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1 , 0);

	if(bytes <= 0)
	{
		std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
		ClearClients(fd);
		close(fd);
	}
	else
	{
		buff[bytes] = '\0';
		// std::cout << YEL << "Client <" << fd << "> Data: "<< std::endl << WHI << buff;
		//here you can add your code to process the received data: parse, check, authenticate, handle the command, etc...
		ParseData(fd, buff);
	}
}

void Server::ServerInit(int port, char *mdp)
{
	this->Port = port;
	this->Mdp = mdp;
	SerSocket();

	std::cout << GRE << "Server <" << this->ServSocket << "> Connected" << WHI << std::endl;
	std::cout << "Waiting to accept a connection...\n";

    while (Server::Signal == false)
	{
		if((poll(&this->fds[0],this->fds.size(),-1) == -1) && Server::Signal == false)
			throw(std::runtime_error("poll() faild"));

		for (size_t i = 0; i < this->fds.size(); i++)
		{
			if (this->fds[i].revents & POLLIN)
			{
				if (this->fds[i].fd == this->ServSocket)
					AcceptIncomingClient();
				else
					ReceiveDataClient(this->fds[i].fd); 
			}
		}
	}
	CloseFds();
}

void Server::ClearClients(int fd)
{ 
	for(size_t i = 0; i < this->fds.size(); i++)
	{
		if (this->fds[i].fd == fd)
		{
				this->fds.erase(this->fds.begin() + i); 
				break;
		}
	}
	for(size_t i = 0; i < this->clients.size(); i++)
	{
		if (this->clients[i].GetFd() == fd)
		{
				this->clients.erase(this->clients.begin() + i); 
				break;
		}
	}
}
