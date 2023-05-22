#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent economic strength
typedef struct {
    double budget;
    double income;
    double spending;
} STRENGTH;

// Structure to represent each city
typedef struct cityRecord {
    char cityName[50];
    int population;
    STRENGTH economicStrength;
    struct cityRecord* left;
    struct cityRecord* right;
    int height;
} CITYRECORD;

// Function to get the maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Function to calculate the height of a node
int getHeight(CITYRECORD* node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// Function to calculate the balance factor of a node
int getBalance(CITYRECORD* node) {
    if (node == NULL)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// Function to perform a right rotation
CITYRECORD* rotateRight(CITYRECORD* y) {
    CITYRECORD* x = y->left;
    CITYRECORD* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    // Return the new root
    return x;
}

// Function to perform a left rotation
CITYRECORD* rotateLeft(CITYRECORD* x) {
    CITYRECORD* y = x->right;
    CITYRECORD* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    // Return the new root
    return y;
}

// Function to insert a city into the AVL tree
CITYRECORD* insertCity(CITYRECORD* node, char* cityName, int population, double budget, double income, double spending) {
    // Perform the normal BST insertion
    if (node == NULL) {
        CITYRECORD* newNode = (CITYRECORD*)malloc(sizeof(CITYRECORD));
        strcpy(newNode->cityName, cityName);
        newNode->population = population;
        newNode->economicStrength.budget = budget;
        newNode->economicStrength.income = income;
        newNode->economicStrength.spending = spending;
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->height = 1;
        return newNode;
    }

    if (strcmp(cityName, node->cityName) < 0)
        node->left = insertCity(node->left, cityName, population, budget, income, spending);
    else if (strcmp(cityName, node->cityName) > 0)
        node->right = insertCity(node->right, cityName, population, budget, income, spending);
    else
        return node; // Duplicate keys are not allowed

    // Update the height of the current node
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    // Check the balance factor and perform rotations if needed
    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && strcmp(cityName, node->left->cityName) < 0)
        return rotateRight(node);

    // Right Right Case
    if (balance < -1 && strcmp(cityName, node->right->cityName) > 0)
        return rotateLeft(node);

    // Left Right Case
    if (balance > 1 && strcmp(cityName, node->left->cityName) > 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Left Case
    if (balance < -1 && strcmp(cityName, node->right->cityName) < 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

// Function to write city records to a file
void writeRecordsToFile(FILE* file, CITYRECORD* node) {
    if (node != NULL) {
        writeRecordsToFile(file, node->left);
        fprintf(file, "%s\n", node->cityName);
        fprintf(file, "%d\n", node->population);
        fprintf(file, "%.2f\n", node->economicStrength.budget);
        fprintf(file, "%.2f\n", node->economicStrength.income);
        fprintf(file, "%.2f\n", node->economicStrength.spending);
        writeRecordsToFile(file, node->right);
    }
}

// Function to read city records from a file
CITYRECORD* readRecordsFromFile(FILE* file, CITYRECORD* node) {
    if (!feof(file)) {
        char cityName[50];
        int population;
        double budget, income, spending;

        fgets(cityName, sizeof(cityName), file);
        cityName[strcspn(cityName, "\n")] = '\0';

        fscanf(file, "%d\n", &population);
        fscanf(file, "%lf\n", &budget);
        fscanf(file, "%lf\n", &income);
        fscanf(file, "%lf\n", &spending);

        node = insertCity(node, cityName, population, budget, income, spending);
        node->left = readRecordsFromFile(file, node->left);
        node->right = readRecordsFromFile(file, node->right);
    }

    return node;
}

// Function to display city records in-order
void displayCities(CITYRECORD* node) {
    if (node != NULL) {
        displayCities(node->left);
        printf("City: %s\n", node->cityName);
        printf("Population: %d\n", node->population);
        printf("Economic Strength (Budget: %.2f, Income: %.2f, Spending: %.2f)\n",
               node->economicStrength.budget, node->economicStrength.income, node->economicStrength.spending);
        printf("\n");
        displayCities(node->right);
    }
}

// Function to find the city with the highest economic strength
CITYRECORD* findRichestCity(CITYRECORD* node, CITYRECORD* richest) {
    if (node != NULL) {
        if (node->economicStrength.income - node->economicStrength.spending >
            richest->economicStrength.income - richest->economicStrength.spending)
            richest = node;

        richest = findRichestCity(node->left, richest);
        richest = findRichestCity(node->right, richest);
    }

    return richest;
}

int main() {
    FILE* file = fopen("country_records.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    CITYRECORD* root = NULL;
    int choice;
    char cityName[50];
    int population;
    double budget, income, spending;

    do {
        printf("1. Add city record\n");
        printf("2. Display city records\n");
        printf("3. Find the city with the highest economic strength\n");
        printf("4. Write city records to file\n");
        printf("5. Read city records from file\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter city name: ");
                scanf("%s", cityName);
                printf("Enter population: ");
                scanf("%d", &population);
                printf("Enter budget: ");
                scanf("%lf", &budget);
                printf("Enter income: ");
                scanf("%lf", &income);
                printf("Enter spending: ");
                scanf("%lf", &spending);
                root = insertCity(root, cityName, population, budget, income, spending);
                break;

            case 2:
                displayCities(root);
                break;

            case 3:
                if (root != NULL) {
                    CITYRECORD* richestCity = findRichestCity(root, root);
                    printf("City with the highest economic strength:\n");
                    printf("City: %s\n", richestCity->cityName);
                    printf("Population: %d\n", richestCity->population);
                    printf("Economic Strength (Budget: %.2f, Income: %.2f, Spending: %.2f)\n",
                           richestCity->economicStrength.budget,
                           richestCity->economicStrength.income,
                           richestCity->economicStrength.spending);
                    printf("\n");
                } else {
                    printf("No city records found.\n");
                }
                break;

            case 4:
                writeRecordsToFile(file, root);
                printf("City records written to file.\n");
                break;

            case 5:
                fclose(file);
                file = fopen("country_records.txt", "r");
                if (file == NULL) {
                    printf("Error opening file.\n");
                    return 1;
                }
                root = readRecordsFromFile(file, root);
                printf("City records read from file.\n");
                break;

            case 6:
                fclose(file);
                printf("Exiting program.\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }

        printf("\n");
    } while (choice != 6);

    return 0;
}
