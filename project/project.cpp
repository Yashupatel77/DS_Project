
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
void DisplayCheapestAndMostExpensive(HashTable* hashTable, char* country);
void DisplayLightestAndHeaviest(HashTable* hashTable, char* country);
void FreeMemory(HashTable* hashTable);
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
    int choice = 0;
    do {
        printf("\nMenu:\n");
        printf("1. Enter country name and display all the parcels details\n");
        printf("2. Enter country and weight pair\n");
        printf("3. Display the total parcel load and valuation for the country\n");
        printf("4. Enter the country name and display cheapest and most expensive parcel’s details\n");
        printf("5. Enter the country name and display lightest and heaviest parcel for the country\n");
        printf("6. Exit the application\n");
        printf("Enter your choice: ");

        if (fgets(inputBuffer, 100, stdin) != NULL) {
            clear(inputBuffer);
            sscanf(inputBuffer, "%d", &choice);
            switch (choice) {
            case 1:
                printf("Enter country name: ");
                if (fgets(country, MAX_COUNTRY_LENGTH, stdin) != NULL) {
                    // Remove the newline character if present
                    clear(country);
                    DisplayAllParcels(hashTable, country);
                }
                break;
            case 2:
                printf("Enter country name: ");
                if (fgets(country, MAX_COUNTRY_LENGTH, stdin) != NULL) {
                    clear(country);
                    printf("Enter weight: ");
                    if (fgets(inputBuffer, 100, stdin) != NULL) {
                        clear(inputBuffer);
                        sscanf(inputBuffer, "%d", &weight);
                        DisplayParcelByWeight(hashTable, country, weight, 1);
                        DisplayParcelByWeight(hashTable, country, weight, 0);
                    }
                }
                break;
            case 3:
                printf("Enter country name: ");
                if (fgets(country, MAX_COUNTRY_LENGTH, stdin) != NULL) {
                    clear(country);
                    DisplayTotalParcelLoad(hashTable, country);
                }
                break;
            case 4:
                printf("Enter country name: ");
                if (fgets(country, MAX_COUNTRY_LENGTH, stdin) != NULL) {
                    clear(country);
                    DisplayCheapestAndMostExpensive(hashTable, country);
                }
                break;
            case 5:
                printf("Enter country name: ");
                if (fgets(country, MAX_COUNTRY_LENGTH, stdin) != NULL) {
                    clear(country);
                    DisplayLightestAndHeaviest(hashTable, country);
                }
                break;
            case 6:
                printf("Exiting the application...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
            }
        }

    } while (choice != 6);
    FreeMemory(hashTable);
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
void DisplayTotalParcelLoad(HashTable* hashTable, char* country) {
    unsigned long hash = GenerateHash(country);
    Parcel* current = hashTable->root[hash];

    if (current == NULL) {
        printf("No parcels found for country: %s\n", country);
        return;
    }

    int totalWeight = 0;
    float totalValue = 0.0;

    while (current != NULL) {
        totalWeight += current->weight;
        totalValue += current->value;
        current = current->RightChild;
    }

    printf("Total parcel load for %s: %d grams\n", country, totalWeight);
    printf("Total parcel valuation for %s: $%.2f\n", country, totalValue);
}

// Display cheapest and most expensive parcel for a country
void DisplayCheapestAndMostExpensive(HashTable* hashTable, char* country) {
    unsigned long hash = GenerateHash(country);
    Parcel* current = hashTable->root[hash];

    if (current == NULL) {
        printf("No parcels found for country: %s\n", country);
        return;
    }

    Parcel* cheapest = current;
    Parcel* mostExpensive = current;

    while (current != NULL) {
        if (current->value < cheapest->value) {
            cheapest = current;
        }
        if (current->value > mostExpensive->value) {
            mostExpensive = current;
        }
        current = current->RightChild;
    }

    printf("Cheapest parcel for %s: Destination: %s, Weight: %d, Value: %.2f\n", country, cheapest->country, cheapest->weight, cheapest->value);
    printf("Most expensive parcel for %s: Destination: %s, Weight: %d, Value: %.2f\n", country, mostExpensive->country, mostExpensive->weight, mostExpensive->value);
}
void FreeMemory(HashTable* hashTable) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Parcel* current = hashTable->root[i];
        while (current != NULL) {
            Parcel* next = current->RightChild;
            free(current->country); // Free the country string memory
            free(current); // Free the parcel memory
            current = next;
        }
    }
    free(hashTable); // Free the hash table memory
}