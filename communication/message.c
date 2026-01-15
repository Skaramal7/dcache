#include"message.h"
#include <stdlib.h>
#include<string.h>

Message createMessage(char type, char id[], char key[], char data[]) {
    Message msg;

    msg.type = type;
    strcpy(msg.id, id);
    strcpy(msg.key, key);
    strcpy(msg.data, data);

    return msg;
}

char* buildMessage(Message m) {
    char* msg = strcat(&m.type, m.id);
    strcat(msg, m.key);
    strcat(msg, m.data);

    return msg;
}

Message unwindMessage(char* m) {
    Message msg;

    msg.type = m[0];

    char id[11] = "";
    for (int i = 1; i <= 10; i++) {
        id[i-1] = m[i];
    }
    strcpy(msg.id, id);

    char key[17] = "";
    for (int i = 11; i <= 26; i++) {
        key[i-11] = m[i];
    }
    strcpy(msg.key, key);

    char* data = (char *) malloc(sizeof(char));
    for (int i = 27; m[i] != '\0'; i++) {
        data = realloc(data, (i-25) * sizeof(char));
        if (data == NULL) {
            break;
        }
        data[i-27] = m[i];
    }

    free(data);
    strcpy(msg.data, data);

    return msg;
}