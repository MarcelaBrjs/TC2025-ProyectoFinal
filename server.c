#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

char sequences[1000][20000];
char *reference;

int main(int argc, char const* argv[]) {
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[2048];
	
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

		while ((valread = read(new_socket, buffer, 2048))) {
			char *sp = ";";
            int option = atoi(strtok(buffer, sp));
			printf("Option => %d\n", option);

			if (option == 1) {
				// Salvar length del archivo.
                long int fileLengthReference = atoi(strtok(NULL, sp));
                memset(buffer, 0, sizeof(buffer));
				printf("fileLengthReference => %ld\n", fileLengthReference);

				// Asignar memoria a la variable 'reference', donde se guardarán los datos de reference.txt
				reference = malloc(fileLengthReference * sizeof(char));

				// Todos los datos que se reciban hasta que se llene la variable 'reference' pertenecen a reference.txt
				char data[2048];
				while (strlen(reference) < fileLengthReference) {
					read(new_socket, data, 2048);
					strcat(reference, data);
					memset(data, 0, sizeof(data));
				}

				// Confirmar al cliente la correcta recepción de los datos.
				send(new_socket, "Reference data stored successfully.", strlen("Reference data stored successfully."), 0);

            } else if (option == 2) {
				// Salvar length del archivo.
                int fileLengthSequences = atoi(strtok(NULL, sp));
                memset(buffer, 0, sizeof(buffer));
				printf("fileLengthSequences => %d\n", fileLengthSequences);

				int sequencesCont = 0;
                char data[2049] = "";
                int p = 0;

				// !! CAMBIAR 3 POR 1000 AL USAR EL ARCHIVO FINAL.
                while (sequencesCont < fileLengthSequences) {
                    recv(new_socket, data, 2048, 0);
					data[2048] = '\0';

					for (int i = 0; i < strlen(data); i++) {
						if (data[i] == '\n') {
							sequences[sequencesCont][p] = '\0';
							sequencesCont = sequencesCont + 1;
							p = 0;
						} else {
							sequences[sequencesCont][p] = data[i];
							p = p + 1;
						}
					}

					if (strlen(data) < 2048) {
                        sequencesCont = sequencesCont + 1;
                    }
                    
                    memset(data, 0, sizeof(data));
                }
				// Confirmar al cliente la correcta recepción de los datos.
				send(new_socket, "Sequences data stored successfully.", strlen("Sequences data stored successfully."), 0);

			} else if (option == 3) {
                // Retornar los resultados.

				// PRUEBAS DE VARIABLES
				// printf("Reference => %s\n", reference);
				printf("S1 => %s\n", sequences[0]);
				printf("S2 => %s\n", sequences[1]);
				printf("S3 => %s\n", sequences[2]);

			} else if (option == 0) {
                // Salir.
				printf("Sesión completada.\n");
			}
		}
	}

	if (new_socket < 0) {
		perror("Accept failed.");
    }

// closing the connected socket
	close(new_socket);
// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}

