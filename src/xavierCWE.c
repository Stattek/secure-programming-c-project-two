/**
 * Created by Xavier Zamora
 * Summary: Implementation for Xavier's CWEs.
 */

#include "xavierCWE.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ITEMS 20       // Maximum number of items in the inventory
#define MAX_NAME_LENGTH 20 // Max length of each item name (leaving space for null terminator)
#define MAX_QUANTITY 255   // Max number of single item

/**
 * @brief Struct for an item that goes into player inventory.
 *
 * char name = name of item
 * int quantity = quantity of item
 */
typedef struct Item
{
    char name[MAX_NAME_LENGTH + 1];
    int quantity;
} Item;

// Initializing invventory and itemList
char inventory[MAX_ITEMS][MAX_NAME_LENGTH + 1];
struct Item itemList[MAX_ITEMS];

// Setting element of itemList to Item name sword, quantity 2
// Use this for functionality showcase
Item itemList[MAX_ITEMS] = {{"sword", 25}};

/**
 * @brief Searched itemList for item and returns its position in list, if it exists in list.
 * Doesn't exist -> return -1
 *
 * @param itemName name of item to search
 * @return int position, -1 if not found
 */
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

/**
 * @brief adds item to itemList inventory.
 * If item already exists, update current quantity without exceeding max quantity.
 * if item does not exist, add to inventory.
 * If no space in inventory, do not add.
 *
 */
void addItem(void)
{
    char itemName[MAX_NAME_LENGTH + 1];
    char quantityInput[15]; // Buffer for quantity input as string
    int quantity;

    // Prompt the user for the item name
    printf("Enter item name to add:\n>> ");

    // CWE-242: Use of inherently dangerous functions
    // Dangerous: Using gets(), which can lead to buffer overflow if input is larger than the buffer size
    // gets(itemName);

    // Solution: Use fgets() to avoid overflow
    // CWE-787: Out of bounds write
    // Use fgets to ensure no OOB write occurs to itemName
    fgets(itemName, MAX_NAME_LENGTH, stdin);  // Safe input using fgets
    itemName[strcspn(itemName, "\n")] = '\0'; // Remove newline character if present

    // Prompt the user for the quantity
    printf("Enter quantity:\n>> ");
    fgets(quantityInput, sizeof(quantityInput), stdin);

    // CWE-704: Ensure correct type conversion
    // Using atoi to convert quantityInput to an int value, ensuring we dont pass in a char value for later
    quantity = atoi(quantityInput);

    // Ensure the quantity does not exceed the max quantity
    if (quantity > MAX_QUANTITY)
    {
        printf("WARNING: Cannot exceed maximum quantity. Setting quantity to %d.\n", MAX_QUANTITY);
        quantity = MAX_QUANTITY;
    }

    int index = findItemIndex(itemName);

    if (index != -1) // Item found
    {
        // CWE-462: Duplicate key in associative list
        // In this if condition, the item is already found in the list.
        // Instead of adding a new item to the list with the same name, we simply update the current item with its new quantity.
        // User attempting to determine the number of a given item will ALWAYS work.
        int newQuantity = itemList[index].quantity + quantity;

        // Set item quantity to MAX_QUANTITY if exceeded
        if (newQuantity > MAX_QUANTITY)
        {
            printf("WARNING: Cannot exceed maximum quantity. Setting quantity to %d.\n", MAX_QUANTITY);
            itemList[index].quantity = MAX_QUANTITY;
        }
        else
        {
            // Correctly updating if MAX_QUANTITY not exceeded
            itemList[index].quantity = newQuantity;
        }
        printf("Updated %s with new quantity: %d\n", itemName, itemList[index].quantity);
    }
    else // Item not found
    {
        // Adding new item to list
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
        // If for loop is exceeded, inventory is completely full.
        printf("Error: Inventory full, cannot add more items.\n");
    }
}

/**
 * @brief Remove item from inventory
 * If item exists AND quantity to remove < actual quantity, update accordingly.
 * If item exists AND quantity to remove > actual quantity, set item quantity to zero.
 * If item does NOT exist, do nothing.
 *
 */
void removeItem(void)
{
    char itemName[MAX_NAME_LENGTH + 1] = "";
    int quantity;

    // Prompt the user for the item name
    printf("Enter item name to remove:\n>> ");

    fgets(itemName, MAX_NAME_LENGTH, stdin);
    itemName[strcspn(itemName, "\n")] = '\0';

    char tempUserInput[MAX_NAME_LENGTH + 1] = "";
    printf("Enter quantity:\n>> ");
    fgets(tempUserInput, MAX_NAME_LENGTH, stdin);
    quantity = atoi(tempUserInput);

    // CWE-483: Ensure correct block delimitation
    // Code commented out below would result in quantity being set to MAX_QUANTITY no matter what
    /*
    if (quantity > MAX_QUANTITY)
        printf("WARNING: Attempting to remove more items than maximum quantity. Setting quantity to %d.\n", MAX_QUANTITY);
        quantity = MAX_QUANTITY;
    */

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
            printf("WARNING: Removed more items than held. Setting quantity to zero.\n");
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
        printf("Item does not exist in inventory.\n");
    }
}