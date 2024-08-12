#include <stdio.h>
#include <stdlib.h>
#include <string>
#define HASH_TABLE_SIZE 127
#define MAX_COUNTRY_LENGTH 20

typedef struct Parcel {
    char* country;
    int weight;
    float value;
    struct Parcel* LeftChild;
    struct Parcel* RightChild;
} Parcel;

typedef struct HashTable {
    Parcel* root[HASH_TABLE_SIZE];
}HashTable;

unsigned long GenerateHash(char* str);
Parcel* InitializeKeyValuePair(char* country, int weight, float value);
void InsertInHashTable(HashTable* hashTable, char* country, int weight, float value);
Parcel* InsertElementIntoBST(Parcel* parent, char* country, int weight, float value);
HashTable* InitializeHashTable(void);
Parcel* InitializeNode(char* country, int weight, float value);
void DisplayAllParcels(HashTable* hashTable, char* country);
void DisplayParcelByWeight(HashTable* hashTable, char* country, int weight, int isHigher);
int main(void) {
    HashTable* hashTable = InitializeHashTable();

    FILE* file = fopen("couriers.txt", "r");
    if (file == NULL) {
        printf("Failed to open file:\n");
        exit(EXIT_FAILURE);
    }

    char buffer[100];
    char inputBuffer[100];
    char country[MAX_COUNTRY_LENGTH];
    int weight;
    float value;

    while (fgets(buffer, 100, file) != NULL) {
        clear(buffer);
        if (sscanf(buffer, "%[^,],%d,%f", country, &weight, &value) == 3) {
            if (weight < 100 || weight > 50000 || value < 10 || value > 2000) {
                printf("Invalid data: %s, %d, %.2f\n", country, weight, value);
                continue;
            }
            Parcel* node = InitializeNode(country, weight, value);
            InsertInHashTable(hashTable, country, weight, value);
            InsertElementIntoBST(node, country, weight, value);
        }
        else {
            printf("Invalid line format: %s", buffer);
        }
        
    }

    fclose(file);
    return 0;
}
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
Parcel* InitializeNode(char* country, int weight, float value) {
    Parcel* node = (Parcel*)malloc(sizeof(Parcel));
    if (node == NULL) {
        printf("EOM");
        exit(EXIT_FAILURE);
    }
    node->country = (char*)malloc(strlen(country) + 1); // Allocate memory for country
    if (node->country == NULL) {
        printf("EOM");
        exit(EXIT_FAILURE);
    }
    strcpy(node->country, country); // Copy the country string
    node->weight = weight;
    node->value = value;
    node->LeftChild = NULL;
    node->RightChild = NULL;
    return node;
}
Parcel* InsertElementIntoBST(Parcel* parent, char* country, int weight, float value) {
    // If the parent node is NULL, it means we should insert a new node here
    if (parent == NULL) {
        return InitializeNode(country, weight, value);
    }

    // If the parent node is valid, proceed with the comparison
    if (weight < parent->weight) {
        // Recursively insert into the left subtree
        parent->LeftChild = InsertElementIntoBST(parent->LeftChild, country, weight, value);
    }
    else if (weight > parent->weight) {
        // Recursively insert into the right subtree
        parent->RightChild = InsertElementIntoBST(parent->RightChild, country, weight, value);
    }

    // Return the parent node (this allows the subtree to be reattached properly)
    return parent;
}
void DisplayAllParcels(HashTable* hashTable, char* country) {
    unsigned long hash = GenerateHash(country);
    Parcel* current = hashTable->root[hash];

    if (current == NULL) {
        printf("No parcels found for country: %s\n", country);
        return;
    }

    printf("Parcels for country: %s\n", country);
    while (current != NULL) {
        printf("Destination: %s, Weight: %d, Value: %.2f\n", current->country, current->weight, current->value);
        current = current->RightChild;
    }
}

// Display parcels by weight for a given country
void DisplayParcelByWeight(HashTable* hashTable, char* country, int weight, int isHigher) {
    unsigned long hash = GenerateHash(country);
    Parcel* current = hashTable->root[hash];

    if (current == NULL) {
        printf("No parcels found for country: %s\n", country);
        return;
    }

    printf("Parcels for country: %s with weight %s than %d\n", country, isHigher ? "greater" : "less", weight);
    while (current != NULL) {
        // Ensure current is valid before accessing its weight
        if ((isHigher && current->weight > weight) || (!isHigher && current->weight < weight)) {
            printf("Destination: %s, Weight: %d, Value: %.2f\n", current->country, current->weight, current->value);
        }

        // Move to the next right child in the BST
        current = current->RightChild;
    }
}

