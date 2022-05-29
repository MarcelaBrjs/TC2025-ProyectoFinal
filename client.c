#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main(int argc, char *argv[])
{
    int socket_desc;
    struct sockaddr_in server;
    char *message, server_reply[2000];
    
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
    
    // SEND DATA TO SERVER
    message = "Hello server!\r\n\r\n";
    if(send(socket_desc, message, strlen(message), 0) < 0)
    {
        puts("Send failed.");
        return 1;
    }
    puts("Data sent.");
    
    // RECEIVE DATA FROM SERVER
    if(recv(socket_desc, server_reply, 2000 , 0) < 0)
    {
        puts("Recv failed.");
    }
    puts("Reply received.");
    puts(server_reply);
    
    return 0;
}
