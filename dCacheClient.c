#include"communication/message.h"
#include"storage/datastorage.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main() {
    printf("\n\nInitializing dCache client...\n");
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(9095);
    char buffer[BUFFER_SIZE] = {0};

    char serverIp[16];
        printf("Server IP address: ");
        scanf("%s", serverIp);
    Message lastMessage = {'\0', "", "", ""};

    inet_aton("127.0.0.1", (struct in_addr *) &(address.sin_addr.s_addr));

    char keyBuffer[17];
    char dataBuffer[513];
    int len;                // read string len
    char sendmsg[BUFFER_SIZE];
    Message rcvdmsg;
    Message newMsg;
    char newId[12];

    connect(serverSocket, (struct sockaddr *) &address, sizeof(address));

    while (1) {
        int opt = 5;

        printf("\nSelect option:\n");
        printf("\n1. Set object\n2. Get object\n3. Flush data\n4. Exit.\nOption: ");
        scanf("%d", &opt); getchar();

        switch (opt)
        {
            case 1:
                printf("\nObject key (max 16): ");
                fgets(keyBuffer, sizeof(keyBuffer), stdin);
                len = strlen(keyBuffer);
                if (len > 0 && keyBuffer[len - 1] == '\n') {
                    keyBuffer[len - 1] = '\0';
                }

                printf("Object data (max 512): ");
                fgets(dataBuffer, sizeof(dataBuffer), stdin);
                len = strlen(dataBuffer);
                if (len > 0 && dataBuffer[len - 1] == '\n') {
                    dataBuffer[len - 1] = '\0';
                }

                if (strcmp(lastMessage.id, "") == 0) {
                    newMsg = createMessage('a', "0000000001", keyBuffer, dataBuffer);
                    strcpy(sendmsg, buildMessage(newMsg));
                    send(serverSocket, sendmsg, strlen(sendmsg), 0);
                    lastMessage = newMsg;
                } else {
                    int numId = atoi(lastMessage.id);
                    numId++;
                    snprintf(newId, sizeof(newId), "%010d", numId);
                    newMsg = createMessage('a', newId, keyBuffer, dataBuffer);
                    strcpy(sendmsg, buildMessage(newMsg));
                    send(serverSocket, sendmsg, strlen(sendmsg), 0);
                    lastMessage = newMsg;
                    printf("%s, %s, %s", newMsg.id, newMsg.key, newMsg.data);
                }
                break;

            case 2:
                printf("\nObject key (max 16): ");
                scanf("%s", keyBuffer);
                len = strlen(keyBuffer);
                if (len > 0 && keyBuffer[len - 1] == '\n') {
                    keyBuffer[len - 1] = '\0';
                }

                if (strcmp(lastMessage.id, "") == 0) {
                    newMsg = createMessage('b', "0000000001", keyBuffer, "");
                    strcpy(sendmsg, buildMessage(newMsg));
                    send(serverSocket, sendmsg, strlen(sendmsg), 0);
                    lastMessage = newMsg;
                } else {
                    int numId = atoi(lastMessage.id);
                    numId++;
                    snprintf(newId, sizeof(newId), "%010d", numId);
                    newMsg = createMessage('a', newId, keyBuffer, "");
                    strcpy(sendmsg, buildMessage(newMsg));
                    send(serverSocket, sendmsg, strlen(sendmsg), 0);
                    lastMessage = newMsg;
                }

                int valread = read(serverSocket, buffer, BUFFER_SIZE);
                if (valread > 0) {
                    buffer[valread] = '\0';
                    rcvdmsg = unwindMessage(buffer);
                    printf("%s", rcvdmsg.data);
                } else {
                    printf("Unable to get data... ");
                }
                break;
        }
    }

    return 0;
}