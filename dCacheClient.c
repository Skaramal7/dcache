#include"communication/message.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);

    // Set address to your computer's local address
    inet_aton("127.0.0.1", (struct in_addr *) &(address.sin_addr.s_addr));

    // Establish a connection to address on client_socket
    connect(clientSocket, (struct sockaddr *) &address, sizeof(address));


    Message msg = createMessage('a', "0000000001", "object", "2-10tjg-sdgidnsdajo");
    char* message = buildMessage(msg);

    char any;
    scanf("%s", &any); // stop so it doesnt run automatically for testing

    send(clientSocket, message, strlen(message), 0);

    // Close the connection
    close(clientSocket);

    return 0;
}