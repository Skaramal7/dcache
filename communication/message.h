#ifndef DCACHE_MESSAGE_H
#define DCACHE_MESSAGE_H

struct message;

struct message createMessage(char type, char id[], char key[], char data[]);

char* buildMessage(struct message m);

struct message unwindMessage(char* m);

#endif