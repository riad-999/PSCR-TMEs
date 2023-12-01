// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <fstream>


#define PORT 8080

int main(int argc, char const* argv[])
{
	int size, client_fd;
	struct sockaddr_in serv_addr;
    int cmd = 3;

	client_fd = socket(AF_INET, SOCK_STREAM, 0);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
	
	connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	write(client_fd, &cmd, sizeof(cmd));

	char buffer[1024];
    int bytesRead = 0;

	if (cmd == 1) {
		std::streampos fileSize;
    	read(client_fd, &fileSize, sizeof(fileSize));
		std::cout << "result: " << std::endl;
		while (bytesRead < fileSize) {
			std::streamsize recvSize = read(client_fd, buffer, sizeof(buffer));
			std::cout << buffer;
			bytesRead += recvSize;
		}
	}
	if (cmd == 2) {
		char filename[100] = "Pool.h";
		size = strlen(filename) * sizeof(char);
		write(client_fd, &size, sizeof(int));
		write(client_fd, filename, size);
		
		std::streampos fileSize;
    	read(client_fd, &fileSize, sizeof(fileSize));

		char path[100] = "./downloads/";
		strcat(path, filename);
		std::ofstream outputFile(path, std::ios::binary);
		while (bytesRead < fileSize) {
			std::streamsize recvSize = read(client_fd, buffer, sizeof(buffer));
			outputFile.write(buffer, recvSize);
			bytesRead += recvSize;
    	}
	}
	if (cmd == 3) {
		// upload
		char filename[100] = "Pool.h";
		size = strlen(filename) * sizeof(char);
		write(client_fd, &size, sizeof(int));
		write(client_fd, filename, size);

		std::ifstream file(filename, std::ios::binary);
		file.seekg(0, std::ios::end);
		std::streampos fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		write(client_fd, &fileSize, sizeof(fileSize));

		while (!file.eof()) {
			file.read(buffer, sizeof(buffer));
			bytesRead = file.gcount();
			write(client_fd, buffer, bytesRead);
		}
	}

	// closing the connected socket
	close(client_fd);
	return 0;
}
