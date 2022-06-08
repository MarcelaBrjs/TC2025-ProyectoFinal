#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#define _OPEN_SYS_ITOA_EXT
#define PORT 8080

long int findSize(char file_name[]) {
	FILE* fp = fopen(file_name, "r");

	if (fp == NULL) {
		printf("File Not Found!\n");
		return -1;
	}

	fseek(fp, 0L, SEEK_END);
	long int size = ftell(fp);

	fclose(fp);

	return size;
}

int main(int argc, char const* argv[]) {
	int sock = 0, opcion, valread, client_fd;
	struct sockaddr_in serv_addr;
	char buffer[2048] = {};
	char fileNameReference[] = { "referenceTest.txt" };
	char fileNameSequence[] = { "sequencesTest.seq" };
	char line[500];
	char ch;
	long size;
	
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
				size = findSize(fileNameReference);

				send(sock, "1;7736;", strlen("1;7736;"), 0);

				// Leer archivo de referencia.
				FILE *ptrFileReference;
				ptrFileReference = fopen(fileNameReference,"r");

				// Si el archivo no está disponible:
                if (ptrFileReference == NULL) {
                    printf("reference.txt error.\n");
                };

				int count = 0;
				char segment[2048];
				while ((ch = fgetc(ptrFileReference)) != EOF) {
					if ( count == 2048 ) {
						if(send(sock, segment, strlen(segment), 0) < 0) {
							puts("Send failed.");
							return 1;
						}
						count = 0;

					}
					strcpy(&segment[count], &ch);
					count++;
				}
				if(send(sock, segment, strlen(segment), 0) < 0) {
					puts("Send failed.");
					return 1;
				}

				fclose(ptrFileReference);

				// Recibir confirmación del servidor.
				read(sock, buffer, 2048);
				printf("\n%s\n", buffer);

				break;
            case 2:
				// Tamaño total del archivo.
				size = findSize(fileNameSequence);
				send(sock, "2;29205;", strlen("2;29205;"), 0);
				
				// Leer archivo de sequences.
				FILE *ptrFileSequences;
				ptrFileSequences = fopen("sequences.seq","r");

				// Si el archivo no está disponible:
                if (ptrFileSequences == NULL) {
                    printf("sequences.seq error.\n");
                };

				// Lectura por línea del archivo.
                // while (fgets(line, sizeof(line), ptrFileSequences)) {
                //     if(send(sock, line, strlen(line), 0) < 0) {
                //         puts("Send failed.");
                //         return 1;
                //     }
                //     printf("Data sent - Sequences.\n");
				// 	memset(buffer, 0, sizeof(buffer));
                // }

				fclose(ptrFileSequences);

				// Recibir confirmación del servidor.
				// read(sock, buffer, 2048);
				// printf("\n%s\n", buffer);

				break;
			case 3:
				// Recibir resultados.
				send(sock, "3;", strlen("3;"), 0);

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

	send(sock, "0;", strlen("0;"), 0);

	// Closing the connected socket.
	close(client_fd);
	return 0;
}

