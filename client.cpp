//
// Created by shakya on 10/17/22.
//
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

#define PORT 8080

int main(int argc, char const* argv[])
{
    int sock = 0, client_fd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = { 0 };
    char inBuff[1024] = {0};
    bool isExit = false;
    int bufSize = 1024;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<= 0) {
        printf(
                "\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((client_fd = connect(sock, (struct sockaddr*)&serv_addr,sizeof(serv_addr))) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    std::cout << "=> Awaiting confirmation from the server..." << std::endl;
    recv(sock, buffer, bufSize, 0);

    printf("%s\n", buffer);

    std::cout << "=> Connection confirmed. You're good to go.." << std::endl;
    std::string temp;
    while (!isExit)
    {
        std::cout << "Client message: ";
        std::getline(std::cin, temp, '\n');
        std::strcpy(inBuff, temp.c_str());
        //std::cin >> inBuff;
        send(sock, inBuff, bufSize, 0);
        if (*inBuff != 42){
            recv(sock, buffer, bufSize, 0);
            std::cout << "Server response: " << buffer << std::endl;
        } else {
            isExit = true;
        }
    }
    std::cout << "\n=> Connection terminated..\n";

    // closing the connected socket
    close(client_fd);
    return 0;
}