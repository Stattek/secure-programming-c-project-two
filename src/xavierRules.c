/**
 * Created by Xavier Zamora
 * Summary: Implementation for Xavier's rules.
 */

#include "xavierRules.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ITEMS 20       // Maximum number of items in the inventory
#define MAX_NAME_LENGTH 19 // Max length of each item name (leaving space for null terminator)
#define MAX_QUANTITY 255   // Max number of single item

// How to show CWE-483?? (delimitation) change cwe maybe

typedef struct Item
{
    char name[MAX_NAME_LENGTH + 1];
    int quantity;
} Item;

char inventory[MAX_ITEMS][MAX_NAME_LENGTH + 1];
struct Item itemList[MAX_ITEMS];

Item itemList[MAX_ITEMS] = {{"sword", 2}};

static int findItemIndex(char *itemName)
{
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (strcmp(itemList[i].name, itemName) == 0)
        {
            return i;
        }
    }
    return -1; // Item not found
}

// ccwe 787/462
void addItem() 
{
    char itemName[MAX_NAME_LENGTH + 1];
    char quantityInput[15]; // Buffer for quantity input as string
    int quantity;

    // Prompt the user for the item name
    printf("Enter item name to add: ");
    
    // Dangerous: Using gets(), which can lead to buffer overflow if input is larger than the buffer size
    // gets(itemName);  // CWE-242: Use of inherently dangerous function
    
    // Solution: Use fgets() to avoid overflow
    fgets(itemName, MAX_NAME_LENGTH + 1, stdin);  // Safe input using fgets
    itemName[strcspn(itemName, "\n")] = '\0';  // Remove newline character if present

    // Prompt the user for the quantity
    printf("Enter quantity: ");
    fgets(quantityInput, sizeof(quantityInput), stdin);

    //cwe704
    quantity = atoi(quantityInput);

    // Ensure the quantity does not exceed the max quantity
    if (quantity > MAX_QUANTITY)
    {
        printf("WARNING: Cannot exceed maximum quantity. Setting quantity to %d.\n", MAX_QUANTITY);
        quantity = MAX_QUANTITY;
    }

    int index = findItemIndex(itemName);

    if (index != -1)
    {
        int newQuantity = itemList[index].quantity + quantity;
        
        if (newQuantity > MAX_QUANTITY)
        {
            printf("WARNING: Cannot exceed maximum quantity. Setting quantity to %d.\n", MAX_QUANTITY);
            itemList[index].quantity = MAX_QUANTITY;
        }
        else
        {
            itemList[index].quantity = newQuantity;
        }
        printf("Updated %s with new quantity: %d\n", itemName, itemList[index].quantity);
    }
    else
    {
        for (int i = 0; i < MAX_ITEMS; i++)
        {
            if (itemList[i].name[0] == '\0')
            {
                strncpy(itemList[i].name, itemName, MAX_NAME_LENGTH);
                itemList[i].name[MAX_NAME_LENGTH] = '\0';
                itemList[i].quantity = quantity;
                printf("Added %s with quantity: %d\n", itemName, quantity);
                return;
            }
        }
        printf("Error: Inventory full, cannot add more items.\n");
    }
}

void removeItem()
{
    char itemName[MAX_NAME_LENGTH + 1];
    int quantity;

    // Prompt the user for the item name
    printf("Enter item name to remove: ");

    fgets(itemName, MAX_NAME_LENGTH + 1, stdin);
    itemName[strcspn(itemName, "\n")] = '\0';

    printf("Enter quantity: ");
    scanf("%d", &quantity);

    if (quantity > MAX_QUANTITY)
    {
        printf("WARNING: Attempting to remove more items than maximum quantity. Setting quantity to %d.\n", MAX_QUANTITY);
        quantity = MAX_QUANTITY;
    }

    int index = findItemIndex(itemName);

    if (index != -1)
    {
        int newQuantity = itemList[index].quantity - quantity;

        if (newQuantity < 0)
        {
            printf("WARNING: Removed more items than held. Setting quantity to zero.");
            itemList[index].quantity = 0;
        }
        else
        {
            itemList[index].quantity = newQuantity;
        }
        printf("Updated %s with new quantity: %d\n", itemName, itemList[index].quantity);
    }
    else
    {
        printf("Item does not exist in inventory.");
    }
}