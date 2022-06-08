// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
int main(int argc, char const* argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };
	char* hello = "Server says hello";

	// SOCKET CREATION
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("Socket creation failed"); 
		exit(EXIT_FAILURE);
	}

    printf("Socket has been created.\n");

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// BIND
	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}

    printf("Bound.\n");

	// LISTEN
	if (listen(server_fd, 3) < 0) {
		perror("Listen");
		exit(EXIT_FAILURE);
	}

    printf("Waiting for connections...\n");

	// ACCEPT
	while ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen))) {
		printf("Connection accepted.\n");

		while ((valread = read(new_socket, buffer, 1024))) {
			printf("%s\n", buffer);
			memset(buffer, 0, sizeof(buffer));
		}
	}

	if (new_socket < 0) {
		perror("Accept failed.");
    }

	// HOW TO SEND!
	// send(new_socket, hello, strlen(hello), 0);
	// printf("Hello message sent.\n");

// closing the connected socket
	close(new_socket);
// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}

