#include <stdio.h>
#include <stdlib.h>
#include <string>
#define HASH_TABLE_SIZE 127
#define MAX_COUNTRY_LENGTH 20

typedef struct Parcel {
    char* destination;
    int weight;
    float value;
    struct Parcel* LeftChild;
    struct Parcel* RightChild;
} Parcel;

typedef struct HashTable {
    Parcel* root[HASH_TABLE_SIZE];
}HashTable;

unsigned long GenerateHash(char* str) {
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c;
    hash %= HASH_TABLE_SIZE;
    return hash;
}