#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include<winsock.h>

typedef struct sockaddr_in sockaddr_in;

int ClientInit(WSADATA* wsa, SOCKET* clientSocket);
int ClientConnect(SOCKET* clientSocket, char* ip, u_short port);

void ClientReadAndSendInput(SOCKET* clientSocket);
int ClientSendToServer(SOCKET clientSocket, char* message);

void ClientRead(void* arg_clientSocket);

#endif // CLIENT_H_INCLUDED
