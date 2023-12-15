#ifndef SRC_SOCKET_H_
#define SRC_SOCKET_H_

#include <netinet/ip.h>
#include <string>
#include <iostream>
#include <iosfwd>
#include <netinet/in.h>
#include <sys/socket.h>


std::ostream& operator << (std::ostream& os, struct addr_in* adr) {
	char host[1024];
	getnameinfo((struct sockaddr*) adr, sizeof(adr), host, 1024,
	nullptr, 0, 0);
	os << "host" << host << std::endl;
	std::cout << met_ntoa(adr->sin_addr) << ":" << ntohs(adr->sin_port) << std::end;
	return os;
}



namespace pr {

class Socket {
	int fd;

public :
	Socket():fd(-1){}
	Socket(int fd):fd(fd){}

	// tente de se connecter à l'hôte fourni
	void connect(const std::string & host, int port);
	void connect(in_addr ipv4, int port);

	bool isOpen() const {return fd != -1;}
	int getFD() { return fd ;}

	void close();
};

std::ostream & operator<< (std::ostream & os, struct sockaddr_in * addr);

}

#endif /* SRC_SOCKET_H_ */
