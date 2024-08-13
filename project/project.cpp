/*
* FILE: project.cpp
* PROJECT : SENG1050 - Project
* PROGRAMMER : Virajsinh Dharmendrasinh Solanki & Yash Pareshkumar Patel
* FIRST VERSION : 2024 - 08 - 11
* DESCRIPTION : This is the project for data structures where with
*				the use of Binary Search tree and Hash Table the countries
*				their parcel weight as well as monetary values and display them 
*				as required by the user from the menu.
* GITHUB lINK: https://github.com/Yashupatel77/DS_Project
*/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#define HASH_TABLE_SIZE 127
#define MAX_COUNTRY_LENGTH 20
#pragma warning(disable:4996)

// Structure representing a parcel with country, weight, and value
typedef struct Parcel {
	char* country;
	int weight;
	float value;
	struct Parcel* LeftChild;
	struct Parcel* RightChild;
} Parcel;

// Structure representing a hash table with an array of parcel roots
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

// Main function to manage parcel operations
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

		// Display menu options to the user and prompt for a choice
		printf("\nMenu:\n");
		printf("1. To display all the parcels details enter the country\n");
		printf("2. To display by sorting the weight enter country and weight pair\n");
		printf("3. To Display the total parcel load and value for the country\n");
		printf("4. To display cheapest and most expensive parcel details\n");
		printf("5. To display lightest and heaviest parcel for the country\n");
		printf("6. Exit the application\n");
		printf("Enter your choice: ");
		if (fgets(inputBuffer, 100, stdin) != NULL) {
			clear(inputBuffer);

			// Parse the user's choice from input
			if (sscanf(inputBuffer, "%d", &choice) == 1) {
				switch (choice) {
				case 1:
					printf("Enter country name: ");
					if (fgets(country, MAX_COUNTRY_LENGTH, stdin) != NULL) {

						// Remove the newline character if present
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
								// Validate and parse weight input
								if (sscanf(inputBuffer, "%d", &weight) == 1) {
									if (weight > 0) {
										// Display parcels based on weight condition
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
					// Handle invalid menu choice
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
	// Free all dynamically allocated memory
	FreeMemory(hashTable);
	return 0;
}

/*
* FUNCTION: GenerateHash(char* str)
* DESCRIPTION: Generates a hash value for a given string using a simple hashing algorithm.
* PARAMETERS:  char* str - The string to hash.
* RETURNS:     unsigned long - The generated hash value.
*/
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

/*
* FUNCTION: clear(char* removeBuf)
* DESCRIPTION: Clears the input buffer by replacing newline character with null terminator.
* PARAMETERS:  char* removeBuf - The buffer to clear.
* RETURNS:     void
*/
void clear(char* removeBuf) {
	char* temp = strchr(removeBuf, '\n');
	if (temp != NULL) {

		// replaces the '/n' to '/0'
		*temp = '\0';
	}
}

/*
* FUNCTION: InitializeKeyValuePair(char* country, int weight, float value)
* DESCRIPTION: Initializes a parcel node with the given country, weight, and value.
* PARAMETERS:  char* country - The country for the parcel.
*              int weight - The weight of the parcel.
*              float value - The value of the parcel.
* RETURNS:     Parcel* - Pointer to the initialized parcel node.
*/
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


/*
* FUNCTION: InsertElementIntoBST(Parcel* parent, char* country, int weight, float value)
* DESCRIPTION: Inserts a parcel into a binary search tree (BST) based on weight.
* PARAMETERS:  Parcel* parent - Pointer to the parent node.
*              char* country - The country of the parcel.
*              int weight - The weight of the parcel.
*              float value - The value of the parcel.
* RETURNS:     Parcel* - Pointer to the root of the updated subtree.
*/
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

/*
* FUNCTION: InitializeHashTable(void)
* DESCRIPTION: Initializes a hash table with all root pointers set to NULL.
* PARAMETERS:  void
* RETURNS:     HashTable* - Pointer to the initialized hash table.
*/

// Initialize the hash table with all roots set to NULL
HashTable* InitializeHashTable(void) {
	HashTable* table = (HashTable*)malloc(sizeof(HashTable));
	if (table == NULL) {
		printf("Memory allocation failed.\n");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < HASH_TABLE_SIZE; i++) {

		// Initialize each root to NULL
		table->root[i] = NULL;
	}
	return table;
}
/*
* FUNCTION: InitializeNode(char* country, int weight, float value)
* DESCRIPTION: Allocates memory for a new parcel node and initializes it with given values.
* PARAMETERS:  char* country - The country of the parcel.
*              int weight - The weight of the parcel.
*              float value - The value of the parcel.
* RETURNS:     Parcel* - Pointer to the initialized node.
*/

// Corrected InitializeNode function to allocate memory for country
Parcel* InitializeNode(char* country, int weight, float value) {
	Parcel* node = (Parcel*)malloc(sizeof(Parcel));
	if (node == NULL) {
		printf("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	// Allocate memory for country
	node->country = (char*)malloc(strlen(country) + 1);
	if (node->country == NULL) {
		printf("Memory allocation failed");
		exit(EXIT_FAILURE);
	}

	// Copy the country string
	strcpy(node->country, country);
	node->weight = weight;
	node->value = value;
	node->LeftChild = NULL;
	node->RightChild = NULL;
	return node;
}
/*
* FUNCTION: DisplayAllParcels(HashTable* hashTable, char* country)
* DESCRIPTION: Displays all parcels for a specified country.
* PARAMETERS:  HashTable* hashTable - Pointer to the hash table.
*              char* country - The country for which to display parcels.
* RETURNS:     void
*/

// Display all parcels for a given country
void DisplayAllParcels(HashTable* hashTable, char* country) {
	unsigned long hash = GenerateHash(country);
	Parcel* current = hashTable->root[hash];

	if (current == NULL) {
		printf("No parcels found for country: %s\n", country);
		return;
	}
	if (strcmp(country, current->country) != 0) {
		printf("Country not found: % s\n", country);
		return;
	}
	printf("Parcels for country: %s\n", country);
	while (current != NULL) {
		printf("Destination: %s, Weight: %d, Value: %.2f\n", current->country, current->weight, current->value);
		current = current->RightChild;
	}
}
/*
* FUNCTION: DisplayParcelByWeight(HashTable* hashTable, char* country, int weight, int isHigher)
* DESCRIPTION: Displays parcels for a specified country filtered by weight.
* PARAMETERS:  HashTable* hashTable - Pointer to the hash table.
*              char* country - The country for which to display parcels.
*              int weight - The weight for comparison.
*              int isHigher - Flag indicating whether to display parcels heavier or lighter than the specified weight.
* RETURNS:     void
*/

// Display parcels by weight for a given country
void DisplayParcelByWeight(HashTable* hashTable, char* country, int weight, int isHigher) {
	unsigned long hash = GenerateHash(country);
	Parcel* current = hashTable->root[hash];

	if (current == NULL) {
		printf("No parcels found for country: %s\n", country);
		return;
	}
	if (strcmp(country, current->country) != 0) {
		printf("Country not found: % s\n", country);
		return;
	}
	int found = 0;
	printf("Parcels for country: %s with weight %s than %d\n", country, isHigher ? "greater" : "less", weight);
	while (current != NULL) {

		// Ensure current is valid before accessing its weight
		if ((isHigher && current->weight > weight) || (!isHigher && current->weight < weight)) {
			printf("Destination: %s, Weight: %d, Value: %.2f\n", current->country, current->weight, current->value);
			found = 1;
		}
		// Move to the next right child in the BST
		current = current->RightChild;
	}

	if (!found) {
		printf("No parcels found for country: %s with weight %s than %d\n", country, isHigher ? "greater" : "less", weight);
	}
}

/*
* FUNCTION: DisplayTotalParcelLoad(HashTable* hashTable, char* country)
* DESCRIPTION: Displays the total parcel load and valuation for a specified country.
* PARAMETERS:  HashTable* hashTable - Pointer to the hash table.
*              char* country - The country for which to calculate total load and valuation.
* RETURNS:     void
*/


// Display total parcel load and valuation for a country
void DisplayTotalParcelLoad(HashTable* hashTable, char* country) {
	unsigned long hash = GenerateHash(country);
	Parcel* current = hashTable->root[hash];

	// Check if there are any parcels for the given country
	if (current == NULL) {
		printf("No parcels found for country: %s\n", country);
		return;
	}
	if (strcmp(country, current->country) != 0) {
		printf("Country not found: % s\n", country);
		return;
	}
	int totalWeight = 0;
	float totalValue = 0.0;

	// Traverse the BST and sum the weight and valuation of all parcels
	while (current != NULL) {
		totalWeight += current->weight;
		totalValue += current->value;
		current = current->RightChild;
	}

	printf("Total parcel load for %s: %d grams\n", country, totalWeight);
	printf("Total parcel valuation for %s: $%.2f\n", country, totalValue);
}

/*
* FUNCTION: DisplayCheapestAndMostExpensive(HashTable* hashTable, char* country)
* DESCRIPTION: Displays the cheapest and most expensive parcels for a specified country.
* PARAMETERS:  HashTable* hashTable - Pointer to the hash table.
*              char* country - The country for which to find parcels.
* RETURNS:     void
*/

// Display cheapest and most expensive parcel for a country
void DisplayCheapestAndMostExpensive(HashTable* hashTable, char* country) {
	// Generate hash value to find the corresponding index in the hash table
	unsigned long hash = GenerateHash(country);
	Parcel* current = hashTable->root[hash];
	// Check if there are any parcels for the given country
	if (current == NULL) {
		printf("No parcels found for country: %s\n", country);
		return;
	}
	if (strcmp(country, current->country) != 0) {
		printf("Country not found: % s\n", country);
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
	// Output the details of the cheapest and most expensive parcels
	printf("Cheapest parcel for %s: Destination: %s, Weight: %d, Value: %.2f\n", country, cheapest->country, cheapest->weight, cheapest->value);
	printf("Most expensive parcel for %s: Destination: %s, Weight: %d, Value: %.2f\n", country, mostExpensive->country, mostExpensive->weight, mostExpensive->value);
}

/*
* FUNCTION: DisplayLightestAndHeaviest(HashTable* hashTable, char* country)
* DESCRIPTION: Displays the lightest and heaviest parcels for a specified country.
* PARAMETERS:  HashTable* hashTable - Pointer to the hash table.
*              char* country - The country for which to find parcels.
* RETURNS:     void
*/

// Display lightest and heaviest parcel for a country
void DisplayLightestAndHeaviest(HashTable* hashTable, char* country) {
	unsigned long hash = GenerateHash(country);
	Parcel* current = hashTable->root[hash];
	// Check if there are any parcels for the given country
	if (current == NULL) {
		printf("No parcels found for country: %s\n", country);
		return;
	}
	if (strcmp(country, current->country) != 0) {
		printf("Country not found: % s\n", country);
		return;
	}
	Parcel* lightest = current;
	Parcel* heaviest = current;
	// Traverse the BST to find the lightest and heaviest parcels
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

/*
* FUNCTION: FreeMemory(HashTable* hashTable)
* DESCRIPTION: Frees all dynamically allocated memory for parcels and the hash table.
* PARAMETERS:  HashTable* hashTable - Pointer to the hash table.
* RETURNS:     void
*/

// Free all dynamically allocated memory
void FreeMemory(HashTable* hashTable) {
	for (int i = 0; i < HASH_TABLE_SIZE; i++) {
		Parcel* current = hashTable->root[i];
		while (current != NULL) {
			Parcel* next = current->RightChild;
			// Free the country string memory
			free(current->country);
			// Free the parcel memory
			free(current);
			current = next;
		}
	}
	// Free the hash table memory
	free(hashTable);
}