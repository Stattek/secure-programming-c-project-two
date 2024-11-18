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
#include "xavierRules.h"
#include "aayaanCWE.h"
#include "waleedRules.h"
#include "coltonRules.h"

#define INPUT_BUFFER_SIZE 1024
#define NUM_SORT_ARRAY_ELEMENTS 8

static void makeChoice(void);
static bool isInteger(const char *str);
static void processUserInput(void);
static void processInput(const char *input);
static void getFileName(char *fileName, int fileNameLen);

static void getFileName(char *fileName, int fileNameLen)
{
    printf("Enter file name:\n>> ");
    fgets(fileName, fileNameLen, stdin);
    trimUserInput(fileName);
}

/**
 * @author David Slay
 * @brief Prompts user to make a choice on what to do.
 */
static void makeChoice(void)
{
    // NOTE: new options are added here
    static const char *options[] = {
        // David
        "Quit",
        "Remove non-characters from input",
        "Validate user input",
        "Perform Stalin/Drop sort",
        // Xavier options
        "Add item to inventory",
        "Remove item from inventory",
        // Aayaan options
        "Evaluate the vocabulary of a file",
        // Waleed options
        "Perform input validation",
        // Colton
        "Use Integer Calculator",
    };

    bool isUserContinuing = true;
    while (isUserContinuing)
    {
        printf("\nChoose an option:\n");
        int optionsLength = sizeof(options) / sizeof(options[0]);
        for (int i = 0; i < optionsLength; i++)
        {
            printf("\t%d. %s\n", i, options[i]);
        }

        bool validInput = false;

        char buf[INPUT_BUFFER_SIZE + 1] = ""; // +1 for null-terminating char
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

        // NOTE: functions are added as cases here
        // CWE-478: Ensuring default case exists.
        switch (userInput)
        {
        case 0:
            isUserContinuing = false;
            break;
        case 1:
        {
            removeInvalidChars();
            break;
        }
        case 2:
        {
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
        }
        case 3:
        {
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

            printf("Result of Stalin sort:\n\t");
            for (int i = 0; i < numElements; i++)
            {
                if (i == 0)
                {
                    printf("[");
                }

                printf("%d", numArray[i]);

                if (i < numElements - 1)
                {
                    printf(", ");
                }
                else
                {
                    printf("]");
                }
            }
            printf("\n\tSum before: %ld\n\tSum after: %ld\n", sumBefore, sumAfter);
            freeArrayFromUser(&numArray);
            break;
        }
        case 4:
        {
            // Add item to inventory
            addItem();
            break;
        }
        case 5:
        {
            // Remove item from inventory
            removeItem();
            break;
        }
        case 6:
        {
            char fileName[INPUT_BUFFER_SIZE] = "";
            getFileName(fileName, INPUT_BUFFER_SIZE);

            countUniqueWords(fileName);
        }
        case 7:
        {
            processUserInput();
            break;
        }
        case 8:
        {

            runIntCalculator();
            break;
        }
        default:
            break;
        }
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

/**
 * @author Waleed Chatta
 * @brief Processes user input demonstrating fixed CWE implementations.
 *
 * This function prompts the user for input and calls `processInput()` to process it.
 * It ensures that the input is handled securely and correctly.
 */
static void processUserInput(void)
{
    char userInput[INPUT_BUFFER_SIZE];

    printf("Enter a number:\n>> ");
    if (fgets(userInput, sizeof(userInput), stdin) != NULL)
    {
        // Remove trailing newline character
        userInput[strcspn(userInput, "\n")] = '\0';

        processInput(userInput);
    }
    else
    {
        fprintf(stderr, "Error reading input.\n");
    }
}

/**
 * @author Waleed Chatta
 * @brief Processes input data with proper validation and handling.
 *
 * This function demonstrates proper handling by fixing several common weaknesses.
 *
 * @param input The input data provided by the user.
 */
static void processInput(const char *input)
{
    // Call the fixed processIntegerInput function (CWE-1287)
    processIntegerInput(input);

    // Call the fixed validateAlphanumericInput function (CWE-115)
    validateAlphanumericInput(input);

    // Call the fixed typeConfusion function (CWE-351)
    typeConfusion();

    // Convert input to integer for further processing
    char *endptr;
    errno = 0;
    long value = strtol(input, &endptr, 10);
    if (errno != 0 || *endptr != '\0')
    {
        // Error already reported in processIntegerInput
        return;
    }

    // Call the fixed checkValue function (CWE-480)
    checkValue((int)value);

    // Call the fixed accessArray function (CWE-431)
    accessArray((int)value);

    // Call the fixed handleError function (CWE-430)
    handleError((int)value);
}

int main(void)
{
    makeChoice();
    return 0;
}
