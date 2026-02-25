#include"communication/message.h"
#include"storage/datastorage.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PORT 9095
#define MAX_QUEUE 4
#define BUFFER_SIZE 1024
#define BUCKETSNUM 997

ObjectNode* getObject(char *clientId, char key[17], ClientEntry* clientArray);

int main(){
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0), clientSocket;
  char buffer[BUFFER_SIZE] = {0};
  ClientEntry clientArray[BUCKETSNUM];

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(PORT);
  address.sin_addr.s_addr = INADDR_ANY;
  int addrlen = sizeof(address);

  // bind serverSocket
  bind(serverSocket, (struct sockaddr *) &address, sizeof(address));

  // listening for clients
  listen(serverSocket, MAX_QUEUE);
  printf("Server listening on port %d...\n", PORT);

  while (1) {
    if ((clientSocket = accept(serverSocket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
      perror("accept failed");
      exit(1);
    }
    printf("connection established...\n");

    int valread = read(clientSocket, buffer, BUFFER_SIZE);
    if (valread > 0) {
      struct sockaddr_in peerAddr;
      socklen_t peerAddrlen = sizeof(peerAddr);
      char *clientIp, clientPort[6];

      if (getpeername(clientSocket, (struct sockaddr*)&peerAddr, &peerAddrlen) == 0) {
        strcpy(clientIp, inet_ntoa(peerAddr.sin_addr));
        sprintf(clientPort, "%d", ntohs(peerAddr.sin_port));
      } else {
        // handle error
        exit(1);
      }

      char* clientId = buildClientId(clientIp, clientPort);

      Message rcvdmsg = unwindMessage(buffer);
      switch (rcvdmsg.type) {
        case 'a':
          storeObject(clientArray, clientId, rcvdmsg.key, rcvdmsg.data, BUCKETSNUM);
          //char* sendmsg = buildMessage(createMessage('a', rcvdmsg.id, rcvdmsg.key, ""));  //acknowledge msg sending - not implemented yet
          break;

        case 'b':
          ObjectNode* node = getObject(clientId, rcvdmsg.key, clientArray);
          if (node == NULL) {
            char* sendmsg = buildMessage(createMessage('b', rcvdmsg.id, rcvdmsg.key, ""));
            send(clientSocket, sendmsg, strlen(sendmsg), 0);
          }
          char* sendmsg = buildMessage(createMessage('b', rcvdmsg.id, rcvdmsg.key, node->data));
          send(clientSocket, sendmsg, strlen(sendmsg), 0);
          break;

        case 'c':
          deleteObject(clientArray, clientId, rcvdmsg.key, BUCKETSNUM);
          break;

        case 'z':
          int index = hashClientId(clientId, BUCKETSNUM);
          while (clientArray[index].objectList != NULL) {
            deleteObject(clientArray, clientId, clientArray[index].objectList->key, BUCKETSNUM);
          }
          break;
      }
    } else if (valread == 0) {
      printf("connection unsuccessful...\n");
    } else {
      perror("read error");
    }

    close(clientSocket);
    memset(buffer, 0, BUFFER_SIZE);
  }

  return 0;
}

ObjectNode* getObject(char *clientId, char key[17], ClientEntry* clientArray) {
  int index = hashClientId(clientId, BUCKETSNUM);

  if (strcmp(clientArray[index].clientId, clientId) == 0) {
    for (ObjectNode* node = clientArray[index].objectList; node != NULL; node = node->next) {
      if (strcmp(node->key, key) == 0) {
        return node;
      }
    }
    return NULL;
  } else {
    for (int i = index + 1; i < BUCKETSNUM; i++) {
      if (strcmp(clientArray[i].clientId, clientId) == 0) {
        for (ObjectNode* node = clientArray[index].objectList; node != NULL; node = node->next) {
          if (strcmp(node->key, key) == 0) {
            return node;
          }
        }
        return NULL;
      }
    }
  }

  return NULL;
}