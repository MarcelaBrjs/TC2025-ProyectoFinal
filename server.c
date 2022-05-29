#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

int main(int argc , char *argv[])
{
    int socket_desc, new_socket, c;
    struct sockaddr_in server, client;
    char *message, client_data[2000];
    
    // CREATE SOCKET
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Error creating socket.");
    }
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8080 );
    
    // BIND SOCKET
    if( bind(socket_desc,(struct sockaddr *)&server, sizeof(server)) < 0) {
        puts("Bind failed.");
        return 1;
    }
    puts("Bound.");
    
    // LISTEN FOR CONNECTIONS
    listen(socket_desc, 3);
    
    // ACCEPT CONNECTIONS
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ) {
        puts("Connection accepted.");
        
        // Receive data from client.
        recv(new_socket, client_data, 2000, 0);
        printf("%s", client_data);

        // Reply to the client.
        message = "Hi client, you connected successfully!\n";
        send(new_socket, message, strlen(message), 0);
    }
    
    if (new_socket<0) {
        perror("Accept failed.");
        return 1;
    }
    
    return 0;
}
