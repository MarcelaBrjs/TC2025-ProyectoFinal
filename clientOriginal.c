#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main(int argc, char *argv[])
{
    int socket_desc;
    struct sockaddr_in server;
    char *message, server_reply[2000];
    char line[500];
    int opcion;
    int referenceSize = 368;
    
    // CREATE SOCKET
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        puts("Socket creation error.");
    }
        
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8080 );

    // CONNECT TO SERVER
    if (connect(socket_desc, (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("Connection failed.");
        return 1;
    }
    
    puts("Connected\n");

    printf("\nAnálisis de ADN\n");
    printf("Opciones:\n");
    printf("1. Leer archivo de archivo de referencia.\n");
    printf("2. Leer archivo de secuencias.\n");
    printf("3. Recibir resultados.\n");
    printf("0. Salir.\n");
    printf("Ingrese la opción deseada [0-3]: ");
    scanf("%d", &opcion);

    do
    {
        switch (opcion)
        {
            case 1:
                send(socket_desc, "1;368", strlen("1;368"), 0);

                char *filenameReference = "reference.txt";
                FILE *fpReference = fopen(filenameReference, "r");

                if (fpReference == NULL)
                {
                    printf("reference.txt error.\n");
                };

                while (fgets(line, sizeof(line), fpReference)) {
                    if(send(socket_desc, line, strlen(line), 0) < 0)
                    {
                        puts("Send failed.");
                        return 1;
                    }
                    puts("Data sent - reference.");
                }

                // recv(s, buffer, 1024, 0);
                // printf("\nTabla 1\n");
                // printf("%s\n", buffer);
                // memset(server_reply, 0, sizeof(server_reply));
                break;
            case 2:
                // send(s, "2;", strlen("2;"), 0);
                // recv(s, buffer, 1024, 0);
                // printf("\nTabla 2\n");
                // printf("%s\n", buffer);
                // memset(buffer, 0, sizeof(buffer));
                break;
            case 3:
                // printf("\nQuery SELECT\n");
                // printf("1. SELECT de Tabla 1 - Empleados\n");
                // printf("2. SELECT de Tabla 2 - Departamentos\n");
                // printf("0. Regresar al menú\n");
                // printf("Teclee la opción deseada (0-2): ");

                // int j = 0;
                // unsigned long m = 0;
                // scanf("%d", &j);

                // if (j == 0)
                // {
                //     break;
                // }

                // const char *str3 = case3(j, &m);
                // send(s, str3, m, 0);
                // recv(s, buffer, 1024, 0);
                // printf("\nResultado del SELECT:\n");
                // printf("%s\n", buffer);
                // memset(buffer, 0, sizeof(buffer));

                break;
            case 4:
                // printf("\nQuery JOIN\n");

                // unsigned long n = 0;

                // const char *str4 = case4(&n);
                // send(s, str4, n, 0);
                // recv(s, buffer, 1024, 0);
                // printf("\nResultado del JOIN:\n");
                // printf("%s\n", buffer);
                // memset(buffer, 0, sizeof(buffer));

                break;
            case 5:
                // printf("\nQuery INSERT\n");
                // printf("1. INSERT a Tabla 1 - Empleados\n");
                // printf("2. INSERT a Tabla 2 - Departamentos\n");
                // printf("0. Regresar al menú\n");
                // printf("Teclee la opción deseada (0-2): ");

                // int i = 0;
                // unsigned long l = 0;
                // scanf("%d", &i);

                // if (i == 0)
                // {
                //     break;
                // }

                // const char *str = case5(i, &l);
                // send(s, str, l, 0);
                // printf("\nRegistro insertado de forma exitosa.\n");
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


    // send(s, "0;", strlen("0;"), 0);
    // printf("Sesión cerrada.\n");



    // Leer reference.txt
    char *filenameReference = "reference.txt";
    FILE *fpReference = fopen(filenameReference, "r");

    if (fpReference == NULL)
    {
        printf("reference.txt error.\n");
    };

    // Leer sequences.txt
    char *filenameSequences = "sequences.txt";
    FILE *fpSecuences = fopen(filenameSequences, "r");

    if (fpSecuences == NULL)
    {
        printf("reference.txt error.\n");
    };

    while (fgets(line, sizeof(line), fpReference)) {
        if(send(socket_desc, line, strlen(line), 0) < 0)
        {
            puts("Send failed.");
            return 1;
        }
        puts("Data sent - reference.");
    }

    while (fgets(line, sizeof(line), fpSecuences)) {
        if(send(socket_desc, line, strlen(line), 0) < 0)
        {
            puts("Send failed.");
            return 1;
        }
        puts("Data sent - sequences.");
    }

    

    // SEND DATA TO SERVER
    // if(send(socket_desc, message, strlen(message), 0) < 0)
    // {
    //     puts("Send failed.");
    //     return 1;
    // }
    // puts("Data sent.");
    
    // RECEIVE DATA FROM SERVER
    if(recv(socket_desc, server_reply, 2000 , 0) < 0)
    {
        puts("Recv failed.");
    }
    puts("Reply received.");
    puts(server_reply);
    
    return 0;
}
