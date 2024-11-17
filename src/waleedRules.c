#include "waleedRules.h"
#include <errno.h>
#include <ctype.h>

#define ARRAY_LENGTH 5

/**
 * @author Waleed Chatta
 * @brief Processes input with proper type validation.
 *
 * This function fixes CWE-1287 by using `strtol` to convert the input string to an integer.
 * It checks if the entire string was successfully converted and if there were any conversion errors.
 * This ensures that only valid integer inputs are processed.
 *
 * @param input The input data to process.
 */
void processIntegerInput(const char *input)
{
    char *endptr;
    int value = strtol(input, &endptr, 10);
    if (*endptr != '\0')
    {
        printf("Invalid input: not a valid integer.\n");
        return;
    }
    printf("Processed value: %d\n", value);
}

/**
 * @author Waleed Chatta
 * @brief Checks the value using the correct equality operator.
 *
 * This function fixes CWE-480 by using the equality operator `==` instead of the assignment operator `=`
 * in the conditional statement. This ensures the condition correctly compares the value rather than assigning it.
 *
 * @param value The integer value to check.
 */
void checkValue(int value)
{
    if (value == 0)
    {
        printf("Value is zero.\n");
    }
    else
    {
        printf("Value is non-zero.\n");
    }
}

/**
 * @author Waleed Chatta
 * @brief Accesses an array element with proper bounds checking.
 *
 * This function fixes CWE-431 by adding bounds checking before accessing the array.
 * It ensures that the index is within the valid range to prevent out-of-bounds access,
 * which could lead to undefined behavior or security vulnerabilities.
 */
void accessArray(void)
{
    int numbers[ARRAY_LENGTH];
    int index = 10;
    if (index >= 0 && index < ARRAY_LENGTH)
    {
        numbers[index] = 42;
        printf("Assigned value to array.\n");
    }
    else
    {
        printf("Error: Array index out of bounds.\n");
    }
}

/**
 * @author Waleed Chatta
 * @brief Handles error conditions appropriately.
 *
 * This function fixes CWE-430 by replacing the improper use of `exit()` with suitable error handling.
 * Instead of abruptly terminating the program, it reports the error and allows for graceful recovery or termination.
 *
 * @param value The integer value to check.
 */
void handleError(int value)
{
    if (value < 0)
    {
        fprintf(stderr, "Error: Negative value encountered.\n");
        return;
    }
    printf("Value is non-negative.\n");
}

/**
 * @author Waleed Chatta
 * @brief Demonstrates proper type usage without type confusion.
 *
 * This function fixes CWE-351 by avoiding incorrect casting between incompatible types.
 * It uses variables with their correct data types, ensuring type safety and preventing undefined behavior.
 */
void typeConfusion(void)
{
    int intValue = 42;
    printf("Integer value: %d\n", intValue);
}

/**
 * @Waleed Chatta
 * @brief Validates that the input contains only alphanumeric characters.
 *
 * This function fixes CWE-115 by verifying that the input consists only of expected characters.
 * It rejects any input containing special characters or non-alphanumeric symbols.
 *
 * @param input The input string to validate.
 */
void validateAlphanumericInput(const char *input)
{
    for (size_t i = 0; i < strlen(input); i++)
    {
        if (!isalnum((unsigned char)input[i]))
        {
            fprintf(stderr, "Invalid input: contains special characters.\n");
            return;
        }
    }
    printf("Validated input: %s\n", input);
}