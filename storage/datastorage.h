#ifndef DCACHE_DATASTORAGE_H
#define DCACHE_DATASTORAGE_H

struct objectNode {
    char* key;
    char* data;
    struct objectNode* next;
};
typedef struct objectNode ObjectNode;

struct clientEntry {
    char* clientId;
    ObjectNode* objectList;
};
typedef struct clientEntry ClientEntry;

ClientEntry* initStorage(int bucketsNum);                                           // returns empty array of ClientEntry ready for use

int freeStorage(ClientEntry* clientArray, int bucketsNum);                                          // frees all storage on clientArray; returns 0 for success

//int insertClient(ClientEntry* clientArray, char* clientId, int bucketsNum);                         // inserts clientId in clientArray by hashing its ID; returns 0 for success, 2 if client already exists

//int removeClient(ClientEntry* clientArray, char* clientId);                         // removes clientId in clientArray

int storeObject(ClientEntry* clientArray, char* clientId, char* key, char* data, int bucketsNum);   // stores new object with key in clientId; returns 0 for success, 2 if object already exists

int deleteObject(ClientEntry* clientArray, char* clientId, char* key, int bucketsNum);              // deletes object with key from clientId's list

int hashClientId(char* clientId, int bucketsNum);                                                     // returns hashed array position for given clientId

const char* buildClientId(char* ipaddr, char* portnum);                                   // returns given ip address and port number into a formatted id

#endif