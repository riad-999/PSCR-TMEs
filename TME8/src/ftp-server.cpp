// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <cstdlib> 
#include <fstream>


#define PORT 8080


int main(int argc, char const* argv[])
{
	int server_fd, new_socket;
	int opt = 1;
    int cmd;
	int size;
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);

	// Creating socket file descriptor
	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	// Forcefully attaching socket to the port 8080 (optional)
	setsockopt(server_fd, SOL_SOCKET,
				SO_REUSEADDR | SO_REUSEPORT, &opt,
				sizeof(opt));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	bind(server_fd, (struct sockaddr*)&address,sizeof(address));
	listen(server_fd, 3);
	
    while(true) {
        new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        read(new_socket, &cmd, sizeof(int)); // subtract 1 for the null terminator at the end
        std::cout << "command: " << cmd << std::endl;
		// size = sizeof(char) * strlen(hello);
        // write(new_socket, &size, sizeof(int));
		// write(new_socket, hello, size);

		char buffer[1024];
		int bytesRead = 0;

		if (cmd == 1) {
			// ls 
			std::system("ls > result.txt");
			// Send the file size first
			std::ifstream file("result.txt", std::ios::binary);
			file.seekg(0, std::ios::end);
			std::streampos fileSize = file.tellg();
			file.seekg(0, std::ios::beg);

			write(new_socket, &fileSize, sizeof(fileSize));

			// Send the file data
			while (!file.eof()) {
				file.read(buffer, sizeof(buffer));
				bytesRead = file.gcount();
				write(new_socket, buffer, bytesRead);
			}
		}
		if (cmd == 2) {
			// download 
			char filename[100];
			read(new_socket, &size, sizeof(int));
			read(new_socket, filename, size);

			std::ifstream file(filename, std::ios::binary);
			file.seekg(0, std::ios::end);
			std::streampos fileSize = file.tellg();
			file.seekg(0, std::ios::beg);

			write(new_socket, &fileSize, sizeof(fileSize));

			while (!file.eof()) {
				file.read(buffer, sizeof(buffer));
				bytesRead = file.gcount();
				write(new_socket, buffer, bytesRead);
			}
		}
		if (cmd == 3) {
			// upload
			char filename[100];
			read(new_socket, &size, sizeof(int));
			read(new_socket, filename, size);

			std::streampos fileSize;
    		read(new_socket, &fileSize, sizeof(fileSize));

			char path[100] = "./uploads/";
			strcat(path, filename);
			std::ofstream outputFile(path, std::ios::binary);
			while (bytesRead < fileSize) {
				std::streamsize recvSize = read(new_socket, buffer, sizeof(buffer));
				outputFile.write(buffer, recvSize);
				bytesRead += recvSize;
			}
		}
    }

	// closing the connected socket
	// close(new_socket);
	// // closing the listening socket
	// close(server_fd);
	return 0;
}
