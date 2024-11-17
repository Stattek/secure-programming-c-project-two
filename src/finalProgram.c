/**
 * Created by David Slay
 * Summary: Final program main implementation file.
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "davidRules.h"
#include "coltonRules.h"

#define INPUT_BUFFER_SIZE 1024
#define NUM_SORT_ARRAY_ELEMENTS 8

static void makeChoice(void);
static bool isInteger(const char *str);

/**
 * @author David Slay
 * @brief Prompts user to make a choice on what to do.
 */
static void makeChoice(void)
{
    // NOTE: new options are added here
    static const char *options[] = {
        "Remove non-characters from input",
        "Validate user input",
        "Perform Stalin sort",
        "Use Integer Calculator",
    };
    printf("Choose an option:\n");
    int optionsLength = sizeof(options) / sizeof(options[0]);
    for (int i = 0; i < optionsLength; i++)
    {
        printf("\t%d. %s\n", (i + 1), options[i]);
    }

    bool validInput = false;

    char buf[INPUT_BUFFER_SIZE] = ""; // +1 for null-terminating char
    while (!validInput)
    {
        printf(">> ");
        fgets(buf, INPUT_BUFFER_SIZE, stdin);
        int lastChar = strlen(buf) - 1;
        if (buf[lastChar] == '\n')
        {
            // make this the new end of the string if it's a newline char
            buf[lastChar] = 0;
        }

        if (isInteger(buf))
        {
            validInput = true;
        }
        else
        {
            // invalid, clear buffer
            memset(buf, 0, INPUT_BUFFER_SIZE);
        }
    }

    long int userInput = 0;
    errno = 0;
    userInput = strtol(buf, NULL, 10);
    if (errno)
    {
        fprintf(stderr, "Error converting user input to long\n");
    }

    // NOTE: add functions as cases here
    switch (userInput)
    {
    case 1:
        removeInvalidChars();
        break;
    case 2:
        Account account;
        if (createAccount(&account))
        {
            fprintf(stderr, "Error creating account\n");
            break;
        }

        if (printAccountInfo(&account))
        {
            fprintf(stderr, "Error printing account info\n");
            break;
        }

        break;
    case 3:
        int *numArray = NULL;
        int numElements = NUM_SORT_ARRAY_ELEMENTS;
        if (getArrayFromUser(&numArray, numElements))
        {
            fprintf(stderr, "Error getting array from user\n");
            break;
        }

        long int sumBefore = 0;
        long int sumAfter = 0;
        // perform sort
        if (stalinSort(numArray, &numElements, &sumBefore, &sumAfter))
        {
            fprintf(stderr, "Error performing Stalin sort\n");
            break;
        }

        printf("Result of Stalin sort:\n");
        for (int i = 0; i < numElements; i++)
        {
            if (i == 0)
            {
                printf("[%d, ", numArray[i]);
            }
            else if (i != numElements - 1)
            {
                printf("%d, ", numArray[i]);
            }
            else
            {
                printf("%d]", numArray[i]);
            }
        }
        printf("\n\tSum before: %ld\n\tSum after: %ld\n", sumBefore, sumAfter);
        freeArrayFromUser(&numArray);
        break;
    case 4:
        char operation[20];
        char continueAnswer;

        int currentTotal = 0;
        printf("Enter number to start:\n>> ");
        scanf("%d", &currentTotal);
        do
        {
            printf("Enter operation (-,+,*,/,^):\n>> ");
            scanf("%s", operation);
            if (strcmp(operation, "-") == 0)
            {
                currentTotal = subtraction(currentTotal);
            }
            else if (strcmp(operation, "+") == 0)
            {
                currentTotal = addition(currentTotal);
            }
            else if (strcmp(operation, "*") == 0)
            {
                currentTotal = multiplication(currentTotal);
            }
            else if (strcmp(operation, "/") == 0)
            {
                currentTotal = division(currentTotal);
            }else if(strcmp(operation, "^") == 0){
                currentTotal = multiplyByPowerOfTwo(currentTotal);
            }
            else
            {
                printf("Incorrect input try again.\n");
            }
            printf("Current number: %d\n", currentTotal);
            printf("Do you want to continue (Y/N)?:\n>> ");
            scanf(" %c", &continueAnswer);
        } while (continueAnswer != 'N' && continueAnswer != 'n');
        printf("Final value is: %d\n", currentTotal);
        break;
    default:
        break;
    }
}

/**
 * @author David Slay
 * @brief Checks whether the string is an integer or not.
 *
 * @returns true if the string contains only digits, false otherwise.
 */
static bool isInteger(const char *str)
{
    bool output = false; // not an integer
    static const char VALID_CHARS[] = "0123456789";

    int strLength = strlen(str);
    if (strLength > 0 && strspn(str, VALID_CHARS) == strLength)
    {
        output = true;
    }

    return output;
}

int main(void)
{
    makeChoice();
    return 0;
}