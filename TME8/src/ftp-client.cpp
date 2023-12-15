// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <signal.h>

int client_fd;

void handler(int _) {
	close(client_fd);
	exit(0);
}

int main(int argc, char const* argv[])
{
	int size;
	struct sockaddr_in serv_addr;
    int cmd;
	char ip_address[16];
	int port;

	signal(SIGINT, handler);

	std::cout << "Enter IP address: ";
	std::cin >> ip_address;

	// Get port number from the user
	std::cout << "Enter port number: ";
	std::cin >> port;
	
	while(true) {

		client_fd = socket(AF_INET, SOCK_STREAM, 0);
		// Get an integer from the user
		std::cout << "Enter the command (1: ls, 2: get, 3: post): " << std::endl;
		std::cin >> cmd;
		
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(PORT);

		// Convert IPv4 address from text to binary form
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
			// char filename[100] = "Pool.h";
			char filename[100];
			std::cout << "Enter the filename to download" << std::endl;
			std::cin >> filename;
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
			// char filename[100] = "Pool.h";
			char filename[100];
			std::cout << "Enter the filename to upload" << std::endl;
			std::cin >> filename;
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
		close(client_fd);
	}
    	
}
