#ifndef DCACHE_MESSAGE_H
#define DCACHE_MESSAGE_H

typedef struct message {
    char type;
    char id[11];
    char key[17];
    char* data;
} Message;

typedef struct dCacheServer
{
    int lastId;
    char ipaddr[16];
    char portnum[6];
} DCacheServer;

Message createMessage(char type, char id[], char key[], char data[]);

char* buildMessage(Message m);

Message unwindMessage(char* m);

#endif