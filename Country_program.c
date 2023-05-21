#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent economic strength
typedef struct {
    double budget;
    double income;
    double spending;
} STRENGTH;

// Structure to represent a country record
typedef struct Node {
    char countryName[50];
    int population;
    STRENGTH economicStrength;
    struct Node* left;
    struct Node* right;
    int height;
} COUNTRYRECORD;

// Function to get the height of a node
int getHeight(COUNTRYRECORD* node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// Function to get the maximum of two numbers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Function to create a new country node
COUNTRYRECORD* createNode(char* countryName, int population, double budget, double income, double spending) {
    COUNTRYRECORD* newNode = (COUNTRYRECORD*)malloc(sizeof(COUNTRYRECORD));
    strcpy(newNode->countryName, countryName);
    newNode->population = population;
    newNode->economicStrength.budget = budget;
    newNode->economicStrength.income = income;
    newNode->economicStrength.spending = spending;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;
    return newNode;
}

// Function to perform a right rotation
COUNTRYRECORD* rotateRight(COUNTRYRECORD* y) {
    COUNTRYRECORD* x = y->left;
    COUNTRYRECORD* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

// Function to perform a left rotation
COUNTRYRECORD* rotateLeft(COUNTRYRECORD* x) {
    COUNTRYRECORD* y = x->right;
    COUNTRYRECORD* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

// Function to get the balance factor of a node
int getBalance(COUNTRYRECORD* node) {
    if (node == NULL)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// Function to insert a country into the AVL tree
COUNTRYRECORD* insertCountry(COUNTRYRECORD* node, char* countryName, int population, double budget, double income, double spending) {
    if (node == NULL)
        return createNode(countryName, population, budget, income, spending);

    if (strcmp(countryName, node->countryName) < 0)
        node->left = insertCountry(node->left, countryName, population, budget, income, spending);
    else if (strcmp(countryName, node->countryName) > 0)
        node->right = insertCountry(node->right, countryName, population, budget, income, spending);
    else {
        node->population = population;
        node->economicStrength.budget = budget;
        node->economicStrength.income = income;
        node->economicStrength.spending = spending;
        return node;
    }

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    int balance = getBalance(node);

    if (balance > 1 && strcmp(countryName, node->left->countryName) < 0)
        return rotateRight(node);

    if (balance < -1 && strcmp(countryName, node->right->countryName) > 0)
        return rotateLeft(node);

    if (balance > 1 && strcmp(countryName, node->left->countryName) > 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && strcmp(countryName, node->right->countryName) < 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

// Function to find the richest country in the AVL tree
COUNTRYRECORD* findRichestCountry(COUNTRYRECORD* node, COUNTRYRECORD* maxCountry) {
    if (node == NULL)
        return maxCountry;

    double currentIncome = node->economicStrength.income - node->economicStrength.spending;
    double maxIncome = maxCountry->economicStrength.income - maxCountry->economicStrength.spending;

    if ((currentIncome / node->population) > (maxIncome / maxCountry->population))
        maxCountry = node;

    maxCountry = findRichestCountry(node->left, maxCountry);
    maxCountry = findRichestCountry(node->right, maxCountry);

    return maxCountry;
}

// Function to display all countries in the AVL tree
void displayCountries(COUNTRYRECORD* node) {
    if (node == NULL)
        return;

    displayCountries(node->left);

    printf("Country Name: %s\n", node->countryName);
    printf("Population: %d\n", node->population);
    printf("Budget: %.2lf\n", node->economicStrength.budget);
    printf("Income: %.2lf\n", node->economicStrength.income);
    printf("Spending: %.2lf\n\n", node->economicStrength.spending);

    displayCountries(node->right);
}

// Function to write country records to a file
void writeRecordsToFile(FILE* file, COUNTRYRECORD* node) {
    if (node == NULL)
        return;

    writeRecordsToFile(file, node->left);
    fprintf(file, "%s %d %.2lf %.2lf %.2lf\n", node->countryName, node->population, node->economicStrength.budget, node->economicStrength.income, node->economicStrength.spending);
    writeRecordsToFile(file, node->right);
}

// Function to read country records from a file
COUNTRYRECORD* readRecordsFromFile(FILE* file) {
    COUNTRYRECORD* root = NULL;
    char countryName[50];
    int population;
    double budget, income, spending;

    while (fscanf(file, "%s %d %lf %lf %lf\n", countryName, &population, &budget, &income, &spending) == 5) {
        root = insertCountry(root, countryName, population, budget, income, spending);
    }

    return root;
}

// Function to free the memory of the AVL tree
void freeTree(COUNTRYRECORD* node) {
    if (node == NULL)
        return;

    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

int main() {
    int choice;
    int numCountries;
    COUNTRYRECORD* root = NULL;

    printf("Enter the number of countries: ");
    scanf("%d", &numCountries);

    for (int i = 0; i < numCountries; i++) {
        printf("Enter information for Country %d:\n", i + 1);
        char countryName[50];
        int population;
        double budget, income, spending;

        printf("Country Name: ");
        scanf("%s", countryName);
        printf("Population: ");
        scanf("%d", &population);
        printf("Budget: ");
        scanf("%lf", &budget);
        printf("Income: ");
        scanf("%lf", &income);
        printf("Spending: ");
        scanf("%lf", &spending);

        root = insertCountry(root, countryName, population, budget, income, spending);
        printf("\n");
    }

    // Open the file in write mode
    FILE* file = fopen("country_records.txt", "w");

    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    // Write country records to the file
    writeRecordsToFile(file, root);

    // Close the file
    fclose(file);

    // Open the file in read mode
    file = fopen("country_records.txt", "r");

    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    // Read country records from the file
    root = readRecordsFromFile(file);

    // Close the file
    fclose(file);

    while (1) {
        printf("1. Find the richest country\n");
        printf("2. Display all countries\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (root != NULL) {
                    COUNTRYRECORD* richestCountry = findRichestCountry(root, root);
                    printf("Richest Country: %s\n", richestCountry->countryName);
                    printf("Population: %d\n", richestCountry->population);
                    printf("Budget: %.2lf\n", richestCountry->economicStrength.budget);
                    printf("Income: %.2lf\n", richestCountry->economicStrength.income);
                    printf("Spending: %.2lf\n\n", richestCountry->economicStrength.spending);
                } else {
                    printf("No countries found.\n\n");
                }
                break;
            case 2:
                printf("All Countries:\n");
                displayCountries(root);
                break;
            case 3:
                freeTree(root);
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n\n");
                break;
        }
    }
}
