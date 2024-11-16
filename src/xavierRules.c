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

// How to show CWE-483?? change cwe maybe

struct Item
{
    char name[MAX_NAME_LENGTH + 1];
    int quantity;
};

char inventory[MAX_ITEMS][MAX_NAME_LENGTH + 1];
struct Item itemList[MAX_ITEMS];

int findItemIndex(char *itemName)
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
void addItem() {
    char itemName[MAX_NAME_LENGTH + 1];
    int quantity;

    // Prompt the user for the item name
    printf("Enter item name: ");
    
    // Dangerous: Using gets(), which can lead to buffer overflow if input is larger than the buffer size
    // gets(itemName);  // CWE-242: Use of inherently dangerous function
    
    // Solution: Use fgets() to avoid overflow
    fgets(itemName, MAX_NAME_LENGTH + 1, stdin);  // Safe input using fgets
    itemName[strcspn(itemName, "\n")] = '\0';  // Remove newline character if present

    // Prompt the user for the quantity
    printf("Enter quantity: ");
    scanf("%d", &quantity);

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

int getItemQuantity(char *input)
{
    // CWE-704: Ensuring correct type conversion
    int quantity = atoi(input);
    
    if (quantity < 0)
    {
        printf("Error: Quantity cannot be negative.\n");
        return 0;
    }
    else if (quantity > MAX_QUANTITY)
    {
        printf("WARNING: Entered quantity exceeds maximum. Setting quantity to %d.\n", MAX_QUANTITY);
        return MAX_QUANTITY;
    }
    return quantity;
}

// CWE-478: Ensuring default case exists
void displayMenu(int choice)
{
    switch (choice)
    {
        case 1:
            prinf("Add Item\n");
            break;
        case 2:
            printf("Remove Item\n");
            break;
        // add more
    }
}

int isInventoryFull()
{
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (itemList[i].name[0] == '\0')
        {
            return 0; // full
        }
    }
    return 1; // not full
}

/*
void getItemName(char *name)
{

}
*/