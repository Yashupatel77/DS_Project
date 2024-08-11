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
Parcel* InitializeKeyValuePair(char* country, int weight, float value) {
    Parcel* keyPairs = (Parcel*)malloc(sizeof(Parcel));
    if (keyPairs == NULL) {
        printf("EOM");
        exit(EXIT_FAILURE);
    }
    keyPairs->country = (char*)malloc(strlen(country) + 1);
    if (keyPairs->country == NULL) {
        printf("EOM");
        exit(EXIT_FAILURE);
    }
    strcpy(keyPairs->country, country);
    keyPairs->weight = weight;
    keyPairs->value = value;
    keyPairs->LeftChild = NULL;
    keyPairs->RightChild = NULL;
    return keyPairs;
}
void clear(char* removeBuf) {
    char* temp = strchr(removeBuf, '\n');
    if (temp != NULL) {
        *temp = '\0';  // replaces the '/n' to '/0'
    }
}
void InsertInHashTable(HashTable* table, char* country, int weight, float value) {
    // Generate the hash value for the given country
    int hash = GenerateHash(country);

    // Create a new parcel node for the entry
    Parcel* newParcel = InitializeKeyValuePair(country, weight, value);

    // Check if the slot at the hash index is empty
    if (table->root[hash] == NULL) {
        table->root[hash] = newParcel;
        // Insert the parcel directly into the hash table
    }
    else {
        // Handle collision by chaining (linked list)
        Parcel* current = table->root[hash];

        // Traverse to the end of the linked list at this index
        while (current->RightChild != NULL) {
            current = current->RightChild;
        }

        // Insert the new parcel at the end of the linked list
        current->RightChild = newParcel;
    }
}