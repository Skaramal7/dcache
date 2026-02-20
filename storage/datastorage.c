#include"datastorage.h"

#include <stdio.h>
#include<stdlib.h>
#include <string.h>

ClientEntry* initStorage(int bucketsNum) {
    ClientEntry* newArray = (ClientEntry*) malloc(sizeof(ClientEntry) * bucketsNum);
    if (newArray == NULL) {
        return NULL;
    }

    return newArray;
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

int main() {
    int index = hashClientId("17769145414504", 200);
}