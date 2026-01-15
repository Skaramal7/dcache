#include"message.h"
#include <stdlib.h>
#include<string.h>

#define STR_PADDING = '#'

Message createMessage(char type, char id[], char key[], char data[]) {
    Message msg;
    char paddedString[17];

    if (!((type == 'a') || (type == 'b') || (type == 'c') || (type =='z'))) {
        msg.type = '0';
        return msg;
    }
    msg.type = type;

    if(strlen(id) != 10) {
        msg.type = '0';
        return msg;
    }
    strcpy(msg.id, id);

    if(strlen(key) > 16) {
        msg.type = '0';
        return msg;
    } else {
        if(strlen(key) < 16) {
            int strl = strlen(key);
            int paddingsize = 16 - strl;
            strcpy(paddedString, key);

            for(int i = strl - 1; i >= 0; i--) {
                paddedString[i+paddingsize] = paddedString[i];
            }
            paddedString[16] = '\0';

            for(int i = 0; i < paddingsize; i++) {
                paddedString[i] = '#';
            }

        }
    }
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

int main () {
    Message msg = createMessage('a', "safsafffaa", "object", "2-10tjg-sdgidnsdajo");
}