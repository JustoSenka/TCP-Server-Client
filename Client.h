#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include<winsock.h>

typedef struct sockaddr_in sockaddr_in;
void ClientRead(void* arg_clientSocket);
int ClientInit(WSADATA* wsa, SOCKET* clientSocket);
int ClientConnect(SOCKET* clientSocket, char* ip, u_short port);

void ClientReadAndSendInput(SOCKET clientSocket);
int ClientSendToServer(SOCKET clientSocket, char* message);

#endif // CLIENT_H_INCLUDED
