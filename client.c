// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char const* argv[]) {
	int sock = 0, opcion, valread, client_fd;
	struct sockaddr_in serv_addr;
	char* hello = "Hello from client";
	char buffer[1024] = { 0 };
	char line[500];
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form.
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if ((client_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}

	printf("Connected.\n");

	printf("\nAnálisis de ADN\n");
    printf("Opciones:\n");
    printf("1. Leer archivo de referencia.\n");
    printf("2. Leer archivo de secuencias.\n");
    printf("3. Recibir resultados.\n");
    printf("0. Salir.\n");
    printf("Ingrese la opción deseada [0-3]: ");
    scanf("%d", &opcion);

	do {
        switch (opcion) {
            case 1:
				// Tamaño total del archivo.
				send(sock, "1;368;", strlen("1;368;"), 0);

				// Leer archivo de referencia.
				FILE *ptrFileReference;
				ptrFileReference = fopen("reference.txt","r");

				// Si el archivo no está disponible:
                if (ptrFileReference == NULL) {
                    printf("reference.txt error.\n");
                };

				// Lectura por línea del archivo.
                while (fgets(line, sizeof(line), ptrFileReference)) {
                    if(send(sock, line, strlen(line), 0) < 0) {
                        puts("Send failed.");
                        return 1;
                    }
                    printf("Data sent - Reference.\n");
					memset(buffer, 0, sizeof(buffer));
                }

				fclose(ptrFileReference);

				// Recibir confirmación del servidor.
				// read(sock, buffer, 1024);
				// printf("%s\n", buffer);

				break;
            case 2:
				// Tamaño total del archivo.
				send(sock, "2;144;", strlen("2;144;"), 0);
				
				// Leer archivo de sequences.
				FILE *ptrFileSequences;
				ptrFileSequences = fopen("sequences.seq","r");

				// Si el archivo no está disponible:
                if (ptrFileSequences == NULL) {
                    printf("sequences.seq error.\n");
                };

				// Lectura por línea del archivo.
                while (fgets(line, sizeof(line), ptrFileSequences)) {
                    if(send(sock, line, strlen(line), 0) < 0) {
                        puts("Send failed.");
                        return 1;
                    }
                    printf("Data sent - Sequences.\n");
					memset(buffer, 0, sizeof(buffer));
                }
				fclose(ptrFileSequences);

				// Recibir confirmación del servidor.
				// read(sock, buffer, 1024);
				// printf("%s\n", buffer);

				break;
			case 3:
				// Recibir resultados.

				break;
			default:
				printf("Opción inválida, intente de nuevo.\n");
        }
        printf("\nAnálisis de ADN\n");
        printf("Opciones:\n");
        printf("1. Leer archivo de archivo de referencia.\n");
        printf("2. Leer archivo de secuencias.\n");
        printf("3. Recibir resultados.\n");
        printf("0. Salir.\n");
        printf("Ingrese la opción deseada [0-3]: ");
        scanf("%d", &opcion);
    } while (opcion != 0);

	// valread = read(sock, buffer, 1024);
	// printf("%s\n", buffer);
	// send(sock, hello, strlen(hello), 0);
	// printf("Hello message sent\n");
	// valread = read(sock, buffer, 1024);
	// printf("%s\n", buffer);

	// Closing the connected socket.
	close(client_fd);
	return 0;
}

