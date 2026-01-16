#include"message.h"
#include <stdlib.h>
#include<string.h>

#define STR_PADDING '#'

Message createMessage(char type, char id[], char key[], char data[]) {              // recieves strings and returns an Message object with a valid type if successful and with '0' type if not
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
                paddedString[i] = STR_PADDING;
            }
        }
        for (int i = 0; i < 17; i++) {
            msg.key[i] = paddedString[i];
        }
    }

    msg.data = (char *) malloc((strlen(data) + 1) * sizeof(char));
    if(msg.data == NULL) {
        msg.type = '0';
        return msg;
    }
    strcpy(msg.data, data);

    return msg;
}

char* buildMessage(Message m) {
    int total_size = 1 + 10 + 16 + strlen(m.data) + 1;

    char* msg = (char *) malloc(total_size * sizeof(char));
    if (msg == NULL) return NULL;

    msg[0] = m.type;
    msg[1] = '\0';
    strcat(msg, m.id);
    strcat(msg, m.key);
    strcat(msg, m.data);

    return msg;
}

Message unwindMessage(char* m) {
    Message msg;

    msg.type = m[0];

    strncpy(msg.id, &m[1], 10);
    msg.id[10] = '\0';

    int key_pos;
    for (int i = 11; i < 27; i++) {
        if (m[i] == STR_PADDING)
            continue;
        else {
            key_pos = i;
            break;
        }
    }
    int key_len = 16 - (key_pos - 11);
    strncpy(msg.key, &m[key_pos], key_len);
    msg.key[key_len] = '\0';

    int data_len = strlen(&m[27]);
    msg.data = (char *) malloc((data_len + 1) * sizeof(char));
    if (msg.data == NULL) {
        msg.type = '0';
        return msg;
    }
    strcpy(msg.data, &m[27]);

    return msg;
}