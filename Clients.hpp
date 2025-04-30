#include <iostream>

class Client
{
	private:
		int Fd;
		std::string IPadd;
		std::string nick;
		std::string user;

	public:
		Client(){};
		int GetFd(){return Fd;}

		void SetFd(int fd){Fd = fd;}
		void Setnick(std::string nick) {this->nick = nick;}
		void Setuser(std::string user) {this->user = user;}
		void setIpAdd(std::string ipadd){IPadd = ipadd;}
};
