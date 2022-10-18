//
// Created by shakya on 10/18/22.
//
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>

#define PORT 8080

void clearBuf(char *ack){
    for (int i=0;i<1024;i++){
        *(ack+i) = 0;
    }
    *ack = '[';
    *(ack+1) = 'E'; *(ack+2) = 'C'; *(ack+3) = 'H'; *(ack+4) = 'O'; *(ack+5) = ']'; *(ack+6) = ' ';
}

int main(int argc, char const* argv[])
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int bufSize = 1024;
    char buffer[1024] = { 0 };
    bool isExit = false;
    char ack[1024] = "[ECHO] ";


    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("setsocketopt");
        exit(EXIT_FAILURE);
    } else {
        std::cout << "Socket has been created..." << std::endl;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address,sizeof(address))< 0)
    {
        perror("bind failed...");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listening failed...");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address,(socklen_t*)&addrlen))< 0)
    {
        perror("accept failed...");
        exit(EXIT_FAILURE);
    }

    while (new_socket > 0)
    {
        strcpy(buffer, "=> server connected....\n");
        send(new_socket, buffer, bufSize, 0);
        std::cout << "=> Connected with the client....\n" << std::endl;

        while (!isExit)
        {
            recv(new_socket, buffer, bufSize, 0);
            std::cout << "Client message: " << buffer << std::endl;

            if (*buffer != 42)
            {
                strcat(ack,buffer);
                send(new_socket,ack,bufSize,0);
                clearBuf(ack);
            } else
            {
                isExit = true;
            }
        }
        close(new_socket);
        std::cout << "\n\n=> Connection terminated with IP " << inet_ntoa(address.sin_addr) << "\n";
        isExit = false;
        exit(1);
    }
//  closing the listening socket
    close(server_fd);
    return 0;
}