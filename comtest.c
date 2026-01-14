#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main(){
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);


  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(8080); // Port 8080
  address.sin_addr.s_addr = INADDR_ANY;

  // Bind serverSocket to address
  bind(serverSocket, (struct sockaddr *) &address, sizeof(address));

  // Listen for clients and allow the accept function to be used
  // Allow 4 clients to be queued while the server processes
  listen(serverSocket, 4);

  // Wait for client to connect, then open a socket
  int clientSocket = accept(serverSocket, NULL, NULL);

  char message[256];
  memset(message, 0, 256);

  recv(clientSocket, message, 255, 0);

  // Close the client socket
  close(clientSocket);

  printf("%s", message);

  return 0;
}