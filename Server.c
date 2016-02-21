#include "Server.h"
#include "list.h"

#include<process.h>
#include<stdio.h>
#include<winsock2.h>

#define MAXRECV 256
const unsigned short port = 8888;

//#pragma comment(lib,"ws2_32.lib") //Winsock Library

void Clear(char* buffer);
AcceptArgs* GetAcceptArgs(SOCKET* sock, List* list);

int main()
{
    WSADATA wsa;
    SOCKET serverSocket;

    List* list = ListCreate(sizeof(SOCKET));

    ServerInit(&wsa, &serverSocket);
    ServerBindSocket(&serverSocket, port);

    listen(serverSocket , 3);

    _beginthread(ServerWaitForConnection, 0, (void*) GetAcceptArgs(&serverSocket, list));

    ServerReadAndSendInput(list);

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}

void ServerReadAndSendInput(List* list)
{
    char buffer[MAXRECV];
    Clear(buffer);
    while (buffer[0] != 0xFF) // ctrl + c
    {
        gets(buffer);
        int i = strlen(buffer);

        buffer[i] = '\r';
        buffer[i + 1] = '\n';
        buffer[i + 2] = '\0';

        ServerSendToAll(list, buffer);
        Clear(buffer);
    }
}

int ServerInit(WSADATA* wsa, SOCKET* serverSocket)
{
    if (WSAStartup(MAKEWORD(2,2),wsa) != 0)
    {
        printf("WSAStartup fail, err: %d\n" ,WSAGetLastError());
        return 1;
    }

    if((*serverSocket = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket, err: %d\n" , WSAGetLastError());
        return 2;
    }

    return 0;
}

int ServerBindSocket(SOCKET* serverSocket, u_short port)
{
    //Prepare the sockaddr_in structure
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( port );

    if( bind(*serverSocket, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d\n" , WSAGetLastError());
        return 1;
    }

    return 0;
}

void ServerWaitForConnection(void* arg_acceptArgs)
{
    AcceptArgs* args = (AcceptArgs*) arg_acceptArgs;
    SOCKET serverSocket = *(args->sock);
    List* list = args->list;
    free(args);

    SOCKET clientSocket;
    sockaddr_in clientAddr;

    //Accept and incoming connection
    puts("Waiting for incoming connections...");

    int c;
    c = sizeof(struct sockaddr_in);
    while( (clientSocket = accept(serverSocket , (struct sockaddr *)&clientAddr, &c)) != INVALID_SOCKET )
    {
        printf("Connection accepted with id: %d\n", clientSocket);
        ListInsert(list, (void*) &clientSocket);
        _beginthread(ClientThread, 0, (void*) GetAcceptArgs(&clientSocket, list));
    }

    if (clientSocket == INVALID_SOCKET)
    {
        printf("Accept failed, err : %d\n" , WSAGetLastError());
        exit(1);
    }
}

void ClientThread(void* arg_acceptArgs)
{
    AcceptArgs* args = (AcceptArgs*) arg_acceptArgs;
    SOCKET clientSocket= *(args->sock);
    //List* list = args->list;
    free(args);

    ServerRead(clientSocket);
}

void ServerSendToAll(List* list, char* message)
{
    while (ListGetEnumerator(list))
    {
        void* currData = ListPointed(list);
        SOCKET client = *((SOCKET*) currData);

        if (ServerSendToClient(client, message) == 1)
        {
            ListRemove(list, currData);
        }
    }
}

int ServerSendToClient(SOCKET clientSocket, char* message)
{
    int i;
    if ((i = send(clientSocket , message , strlen(message) , 0)) != strlen(message))
    {
        if (i == SOCKET_ERROR)
        {
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

void ServerRead(SOCKET clientSocket)
{
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

AcceptArgs* GetAcceptArgs(SOCKET* sock, List* list)
{
    AcceptArgs* args = (AcceptArgs*) malloc(sizeof(AcceptArgs));
    args->list = list;
    args->sock = sock;
    return args;
}
