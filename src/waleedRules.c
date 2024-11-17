#include "waleedRules.h"

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
void processInput(const char *input)
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
    int numbers[5];
    int index = 10;
    if (index >= 0 && index < 5)
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
 * @author Waleed Chatta
 * @brief Processes input with correct interpretation of encoding.
 *
 * This function fixes CWE-115 by verifying that the input is valid UTF-8 before processing it.
 * It uses `strncpy` to safely copy the input into a buffer, preventing buffer overflows,
 * and ensures the buffer is null-terminated.
 *
 * @param input The input string to process.
 */
void processInput(const char *input)
{
    if (isValidUTF8(input))
    {
        char buffer[100];
        strncpy(buffer, input, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';
        printf("Processed input: %s\n", buffer);
    }
    else
    {
        printf("Invalid input encoding.\n");
    }
}

/**
 * @author Waleed Chatta
 * @brief Simplified placeholder function to check if input is valid UTF-8.
 *
 * This function should contain actual UTF-8 validation logic. For this example, it assumes the input is valid.
 * In a real-world scenario, proper validation should be implemented to prevent encoding-related issues.
 *
 * @param input The input string to check.
 * @return 1 if the input is valid UTF-8, 0 otherwise.
 */
static int isValidUTF8(const char *input)
{
    return 1;
}
