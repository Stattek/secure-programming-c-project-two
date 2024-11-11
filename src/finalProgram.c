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

#define INPUT_BUFFER_SIZE 1024

static void makeChoice(void);
static bool isInteger(const char *str);

/**
 * @author David Slay
 * @brief Prompts user to make a choice on what to do.
 */
static void makeChoice(void)
{
    static const char *options[] = {
        "Remove invalid characters from strings",
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

    switch (userInput)
    {
    case 1:
        removeInvalidChars();
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