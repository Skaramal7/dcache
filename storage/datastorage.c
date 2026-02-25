#include"datastorage.h"

#include <stdio.h>
#include<stdlib.h>
#include <string.h>

ClientEntry* initStorage(int bucketsNum) {
    ClientEntry* newArray = (ClientEntry*) malloc(sizeof(ClientEntry) * bucketsNum);

    return newArray;
}

int freeStorage(ClientEntry* clientArray, int bucketsNum) {
    for(int i = 0; i < bucketsNum; i++) {
        if (clientArray[i].objectList != NULL) {
            ObjectNode* lastNode = clientArray[i].objectList;
            for (ObjectNode* node = clientArray[i].objectList->next; node != NULL; node = node->next ) {
                free(lastNode);
                lastNode = node;
            }
            free(lastNode);
        }
    }
    free(clientArray);
    return 0;
}

int storeObject(ClientEntry* clientArray, char* clientId, char* key, char* data, int bucketsNum) {
    int index = hashClientId(clientId, bucketsNum);

    if(clientArray[index].clientId == NULL) {
        clientArray[index].clientId = (char *) malloc(strlen(clientId) + 1);
        strcpy(clientArray[index].clientId, clientId);

        ObjectNode* newNode = (ObjectNode *) malloc(sizeof(ObjectNode));
        if(newNode == NULL) return 1;

        newNode->key = key;
        newNode->data = data;
        newNode->next = clientArray[index].objectList;
        clientArray[index].objectList = newNode;
        return 0;
    } else {
        if(strcmp(clientArray[index].clientId, clientId) == 0) {
            ObjectNode* newNode = (ObjectNode*) malloc(sizeof(ObjectNode));
            if(newNode == NULL) return 1;

            newNode->key = key;
            newNode->data = data;
            newNode->next = clientArray[index].objectList;
            clientArray[index].objectList = newNode;
            return 0;
        } else {
            for(int i = index + 1; i < bucketsNum; i++) {
                if(clientArray[i].clientId == NULL) {
                    clientArray[i].clientId = (char *) malloc(strlen(clientId) + 1);
                    strcpy(clientArray[i].clientId, clientId);

                    ObjectNode* newNode = (ObjectNode *) malloc(sizeof(ObjectNode));
                    if(newNode == NULL) return 1;

                    newNode->key = key;
                    newNode->data = data;
                    newNode->next = clientArray[i].objectList;
                    clientArray[i].objectList = newNode;
                    return 0;
                } else {
                    if(strcmp(clientArray[i].clientId, clientId) == 0) {
                        ObjectNode* newNode = (ObjectNode*) malloc(sizeof(ObjectNode));
                        if(newNode == NULL) return 1;

                        newNode->key = key;
                        newNode->data = data;
                        newNode->next = clientArray[index].objectList;
                        clientArray[index].objectList = newNode;
                        return 0;
                    }
                }
            }
        }
    }
}

int deleteObject(ClientEntry* clientArray, char* clientId, char* key, int bucketsNum) {
    int index = hashClientId(clientId, bucketsNum);

    if (strcmp(clientArray[index].clientId, clientId) == 0) {
        if (strcmp(clientArray[index].objectList->key, key) == 0) {
            ObjectNode* node = clientArray[index].objectList;
            clientArray[index].objectList = clientArray[index].objectList->next;
            free(node);
            return 0;
        } else {
            ObjectNode* lastNode = clientArray[index].objectList;
            for(ObjectNode* node = clientArray[index].objectList->next; node != NULL; node = node->next) {
                if(strcmp(node->key, key) == 0) {
                    lastNode->next = node->next;
                    free(node);
                    return 0;
                }
            }
        }
        return 1;
    } else {
        for(int i = index + 1; i < bucketsNum; i++) {
            if (strcmp(clientArray[i].clientId, clientId) == 0) {
                if (strcmp(clientArray[i].objectList->key, key) == 0) {
                    ObjectNode* node = clientArray[i].objectList;
                    clientArray[i].objectList = clientArray[i].objectList->next;
                    free(node);
                    return 0;
                } else {
                    ObjectNode* lastNode = clientArray[i].objectList;
                    for(ObjectNode* node = clientArray[i].objectList->next; node != NULL; node = node->next) {
                        if(strcmp(node->key, key) == 0) {
                            lastNode->next = node->next;
                            free(node);
                            return 0;
                        }
                    }
                }
            }
        }
    }

    return 1;
}

int hashClientId(char* clientId, int bucketsNum) {
    int keyLength = strlen(clientId);
    char* half1 = (char *) malloc(sizeof(char));
    char* half2 = (char *) malloc(sizeof(char));

    strncpy(half1, clientId, (keyLength / 2));
    strncpy(half2, &clientId[(keyLength / 2)], keyLength);

    int half1int, half2int;
    sscanf(half1, "%d", &half1int);
    sscanf(half2, "%d", &half2int);

    int folded = half1int ^ half2int;
    int index = folded % bucketsNum;

    free(half1);
    free(half2);
    return index;
}

const char* buildClientId(char* ipaddr, char* portnum) {
    char pr[16], *resultString, *pw;
    resultString = (char *) malloc(18 * sizeof(char));
    strcpy(pr, ipaddr);

    pw = resultString;
    for (int i = 0; i < 16; i++) {
        if (pr[i] != '.') {
            *pw = pr[i];
            pw++;
            if (pr[i] == '\0') break;
        }
    }

    strcat(resultString, portnum);
    return resultString;
}