#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <winsock2.h>
#include "List.h"

typedef struct sockaddr_in sockaddr_in;

typedef struct _AcceptArgs {
    SOCKET* sock;
    List* list;
} AcceptArgs;

int ServerInit(WSADATA* wsa, SOCKET* serverSocket);
int ServerBindSocket(SOCKET* serverSocket, u_short port);

void ServerWaitForConnection(void* arg_acceptArgs);
void ClientThread(void* arg_acceptArgs);

void ServerReadAndSendInput(List* list);s
void ServerSendToAll(List* list, char* message);
int ServerSendToClient(SOCKET clientSocket, char* message);
void ServerRead(SOCKET clientSocket);

#endif // SERVER_H_INCLUDED

