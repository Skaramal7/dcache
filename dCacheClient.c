#include"communication/message.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main() {
    printf("\n\nInitializing dCache client...\n");
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(9095);

    char serverIp[16];
    printf("Server IP address: ");
    scanf("%s", serverIp);
    Message lastmessage;

    inet_aton(serverIp, (struct in_addr *) &(address.sin_addr.s_addr));

    while (1)
    {
        int opt = 5;
        printf("\nSelect option:\n");
        printf("\n1. Set object\n2. Get object\n3. Flush data\n4. Exit.\nOption:");
        scanf("%d", &opt);

        switch (opt)
        {
            case 1:
                printf("\nObject key (max 16): "); char key[17];
                scanf("%s", key);
                printf("\nObject data (max 512): "); char data[513];
                scanf("%s", data);

                connect(serverSocket, (struct sockaddr *) &address, sizeof(address));

                if (lastmessage == NULL)
                {
                    char* sendmsg = buildMessage(createMessage('a', "0000000001", key, data));
                    send(serverSocket, sendmsg, strlen(sendmsg), 0);
                }

                break;
        }
    }

    Message msg = createMessage('a', "0000000001", "object", "2-10tjg-sdgidnsdajo");
    char* message = buildMessage(msg);

    char any;
    scanf("%s", &any); // stop so it doesnt run automatically for testing

    printf("sending message: %s\n", message);
    send(serverSocket, message, strlen(message), 0);

    // Close the connection
    close(serverSocket);

    return 0;
}