#include "Client.h"

#include<stdio.h>
#include<winsock2.h>
#include<process.h>

#define MAXRECV 256

#pragma comment(lib,"ws2_32.lib") //Winsock Library

void Clear(char* buffer);
void getsClearStream();

int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET clientSocket;

    while(1)
    {
        ClientInit(&wsa, &clientSocket);

        char ip[18];
        int port;

        printf("Enter ip and port number separated by space: ");
        scanf("%s", ip);
        scanf("%d", &port);
        getsClearStream(); // Clear the stream, after scanf, endline is left there.

        if (!ClientConnect(&clientSocket, ip, port))
        {
            _beginthread(ClientRead, 0, (void*) &clientSocket);
            ClientReadAndSendInput(clientSocket);
        }

        puts("");
        closesocket(clientSocket);
        WSACleanup();
    }

    return 0;
}

int ClientInit(WSADATA* wsa, SOCKET* clientSocket)
{
    if (WSAStartup(MAKEWORD(2,2),wsa) != 0)
    {
        printf("WSAStartup fail, err: %d\n" ,WSAGetLastError());
        return 1;
    }

    if((*clientSocket = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket, err: %d\n" , WSAGetLastError());
        return 2;
    }

    return 0;
}

int ClientConnect(SOCKET* clientSocket, char* ip, u_short port)
{
    sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (connect(*clientSocket, (struct sockaddr*) &server , sizeof(server)) < 0)
    {
        printf("Connect failed with error code : %d\n" , WSAGetLastError());
        return 1;
    }

    puts("Connected to remote server");
    return 0;
}

void ClientReadAndSendInput(SOCKET clientSocket)
{
    char buffer[MAXRECV];
    Clear(buffer);
    while (1)
    {
        gets(buffer);
        int i = strlen(buffer);

        buffer[i] = '\r';
        buffer[i + 1] = '\n';
        buffer[i + 2] = '\0';

        if (ClientSendToServer(clientSocket, buffer) != 0) break;
        Clear(buffer);
    }
}

int ClientSendToServer(SOCKET clientSocket, char* message)
{
    int i;
    if ((i = send(clientSocket , message , strlen(message) , 0)) != strlen(message))
    {
        if (i == SOCKET_ERROR)
        {
            printf("Socket refused connection, err: %d\n", WSAGetLastError());
            return 1;
        }
        else
        {
            printf("Send failed. Supposed to send: %d, actually sent: %d err: %d\n",
                strlen(message), i, WSAGetLastError());
            return 2;
        }
    }
    return 0;
}

void ClientRead(void* arg_clientSocket)
{
    SOCKET clientSocket = *((SOCKET*)arg_clientSocket);
    char buffer[MAXRECV];
    Clear(buffer);

    while (recv(clientSocket, buffer, MAXRECV, 0) > 0)
    {
        printf("%s", buffer);
        Clear(buffer);
    }
}

void Clear(char* buffer)
{
    int i;
    for (i = 0; i < MAXRECV; i++)
    {
        buffer[i] = 0;
    }
}

void getsClearStream()
{
    char buff[MAXRECV];
    gets(buff);
}
