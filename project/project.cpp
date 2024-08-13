#include <stdio.h>
#include <stdlib.h>
#include <string>
#define HASH_TABLE_SIZE 127
#define MAX_COUNTRY_LENGTH 20
#pragma warning(disable:4996)
typedef struct Parcel {
	char* country;
	int weight;
	float value;
	struct Parcel* LeftChild;
	struct Parcel* RightChild;
} Parcel;

typedef struct HashTable {
	Parcel* root[HASH_TABLE_SIZE];
} HashTable;
// Function prototypes
unsigned long GenerateHash(char* str);
Parcel* InitializeKeyValuePair(char* country, int weight, float value);
void InsertInHashTable(HashTable* hashTable, char* country, int weight, float value);
Parcel* InsertElementIntoBST(Parcel* parent, char* country, int weight, float value);
HashTable* InitializeHashTable(void);
Parcel* InitializeNode(char* country, int weight, float value);
void DisplayAllParcels(HashTable* hashTable, char* country);
void DisplayParcelByWeight(HashTable* hashTable, char* country, int weight, int isHigher);
void DisplayTotalParcelLoad(HashTable* hashTable, char* country);
void DisplayCheapestAndMostExpensive(HashTable* hashTable, char* country);
void DisplayLightestAndHeaviest(HashTable* hashTable, char* country);
void FreeMemory(HashTable* hashTable);
void clear(char* removeBuf);
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
			if (sscanf(inputBuffer, "%d", &choice) == 1) {
				switch (choice) {
				case 1:
					printf("Enter country name: ");
					if (fgets(country, MAX_COUNTRY_LENGTH, stdin) != NULL) {
						clear(country);
						if (strlen(country) > 0) {
							DisplayAllParcels(hashTable, country);
						}
						else {
							printf("Invalid country name. Please try again.\n");
						}
					}
					else {
						printf("Error reading country name. Please try again.\n");
					}
					break;
				case 2:
					printf("Enter country name: ");
					if (fgets(country, MAX_COUNTRY_LENGTH, stdin) != NULL) {
						clear(country);
						if (strlen(country) > 0) {
							printf("Enter weight: ");
							if (fgets(inputBuffer, 100, stdin) != NULL) {
								clear(inputBuffer);
								if (sscanf(inputBuffer, "%d", &weight) == 1) {
									if (weight > 0) {
										DisplayParcelByWeight(hashTable, country, weight, 1);
										DisplayParcelByWeight(hashTable, country, weight, 0);
									}
									else {
										printf("Invalid weight. Please enter a positive integer.\n");
									}
								}
								else {
									printf("Invalid weight. Please enter a valid integer.\n");
								}
							}
							else {
								printf("Error reading weight. Please try again.\n");
							}
						}
						else {
							printf("Invalid country name. Please try again.\n");
						}
					}
					else {
						printf("Error reading country name. Please try again.\n");
					}
					break;
				case 3:
					printf("Enter country name: ");
					if (fgets(country, MAX_COUNTRY_LENGTH, stdin) != NULL) {
						clear(country);
						if (strlen(country) > 0) {
							DisplayTotalParcelLoad(hashTable, country);
						}
						else {
							printf("Invalid country name. Please try again.\n");
						}
					}
					else {
						printf("Error reading country name. Please try again.\n");
					}
					break;
				case 4:
					printf("Enter country name: ");
					if (fgets(country, MAX_COUNTRY_LENGTH, stdin) != NULL) {
						clear(country);
						if (strlen(country) > 0) {
							DisplayCheapestAndMostExpensive(hashTable, country);
						}
						else {
							printf("Invalid country name. Please try again.\n");
						}
					}
					else {
						printf("Error reading country name. Please try again.\n");
					}
					break;
				case 5:
					printf("Enter country name: ");
					if (fgets(country, MAX_COUNTRY_LENGTH, stdin) != NULL) {
						clear(country);
						if (strlen(country) > 0) {
							DisplayLightestAndHeaviest(hashTable, country);
						}
						else {
							printf("Invalid country name. Please try again.\n");
						}
					}
					else {
						printf("Error reading country name. Please try again.\n");
					}
					break;
				case 6:
					printf("Exiting the application...\n");
					break;
				default:
					printf("Invalid choice. Please try again.\n");
				}
			}
			else {
				printf("Invalid choice. Please try again.\n");
			}
		}
		else {
			printf("Error reading input. Please try again.\n");
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
void clear(char* removeBuf) {
	char* temp = strchr(removeBuf, '\n');
	if (temp != NULL) {
		*temp = '\0';  // replaces the '/n' to '/0'
	}
}
Parcel* InitializeKeyValuePair(char* country, int weight, float value) {
	Parcel* keyPairs = (Parcel*)malloc(sizeof(Parcel));
	if (keyPairs == NULL) {
		printf("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	keyPairs->country = (char*)malloc(strlen(country) + 1);
	if (keyPairs->country == NULL) {
		printf("Memory allocation failed");
		exit(EXIT_FAILURE);
	}

	strcpy(keyPairs->country, country);
	keyPairs->weight = weight;
	keyPairs->value = value;
	keyPairs->LeftChild = NULL;
	keyPairs->RightChild = NULL;
	return keyPairs;
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

// Initialize the hash table with all roots set to NULL
HashTable* InitializeHashTable(void) {
	HashTable* table = (HashTable*)malloc(sizeof(HashTable));
	if (table == NULL) {
		printf("Memory allocation failed.\n");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < HASH_TABLE_SIZE; i++) {
		table->root[i] = NULL; // Initialize each root to NULL
	}
	return table;
}

// Corrected InitializeNode function to allocate memory for country
Parcel* InitializeNode(char* country, int weight, float value) {
	Parcel* node = (Parcel*)malloc(sizeof(Parcel));
	if (node == NULL) {
		printf("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	node->country = (char*)malloc(strlen(country) + 1); // Allocate memory for country
	if (node->country == NULL) {
		printf("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	strcpy(node->country, country); // Copy the country string
	node->weight = weight;
	node->value = value;
	node->LeftChild = NULL;
	node->RightChild = NULL;
	return node;
}
// Display all parcels for a given country
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

	int found = 0;
	printf("Parcels for country: %s with weight %s than %d\n", country, isHigher ? "greater" : "less", weight);
	while (current != NULL) {
		if ((isHigher && current->weight > weight) || (!isHigher && current->weight < weight)) {
			printf("Destination: %s, Weight: %d, Value: %.2f\n", current->country, current->weight, current->value);
			found = 1;
		}

		current = current->RightChild;
	}

	if (!found) {
		printf("No parcels found for country: %s with weight %s than %d\n", country, isHigher ? "greater" : "less", weight);
	}
}

// Display total parcel load and valuation for a country
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

// Display lightest and heaviest parcel for a country
void DisplayLightestAndHeaviest(HashTable* hashTable, char* country) {
	unsigned long hash = GenerateHash(country);
	Parcel* current = hashTable->root[hash];

	if (current == NULL) {
		printf("No parcels found for country: %s\n", country);
		return;
	}

	Parcel* lightest = current;
	Parcel* heaviest = current;

	while (current != NULL) {
		if (current->weight < lightest->weight) {
			lightest = current;
		}
		if (current->weight > heaviest->weight) {
			heaviest = current;
		}
		current = current->RightChild;
	}
	printf("Lightest parcel for %s: Destination: %s, Weight: %d, Value: %.2f\n", country, lightest->country, lightest->weight, lightest->value);
	printf("Heaviest parcel for %s: Destination: %s, Weight: %d, Value: %.2f\n", country, heaviest->country, heaviest->weight, heaviest->value);
}

// Free all dynamically allocated memory
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