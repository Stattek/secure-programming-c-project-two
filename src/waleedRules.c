#include "waleedRules.h"

/**
 * @brief Processes input without proper type validation.
 *
 * This function demonstrates CWE-1287: Improper Validation of Specified Type of Input
 * by not validating the type of input data before processing it.
 *
 * @param input The input data to process.
 */
void processInput_CWE1287(const char *input)
{
    // Expected input is an integer in string form, but no validation is performed.
    int value = atoi(input);
    printf("Processed value: %d\n", value);
}

/**
 * @brief Demonstrates incorrect operator usage.
 *
 * This function shows CWE-480: Use of Incorrect Operator by using an incorrect operator
 * in a conditional statement.
 *
 * @param value The integer value to check.
 */
void checkValue_CWE480(int value)
{
    // Incorrectly using '=' instead of '==' in the condition.
    if (value = 0)
    {
        printf("Value is zero.\n");
    }
    else
    {
        printf("Value is non-zero.\n");
    }
}

/**
 * @brief Demonstrates missing handler for error conditions.
 *
 * This function exhibits CWE-431: Missing Handler by not handling potential errors
 * or exceptions that may occur.
 */
void accessArray_CWE431(void)
{
    int numbers[5];
    // Accessing array without bounds checking and no error handling.
    numbers[10] = 42; // Out-of-bounds access.
    printf("Assigned value to array.\n");
}

/**
 * @brief Uses inappropriate handler for an error condition.
 *
 * This function demonstrates CWE-430: Deployment of Wrong Handler by deploying the wrong
 * handler when an error occurs.
 *
 * @param value The value to check for error condition.
 */
void handleError_CWE430(int value)
{
    if (value < 0)
    {
        // Inappropriately using exit() to handle error.
        exit(1);
    }
    printf("Value is non-negative.\n");
}

/**
 * @brief Demonstrates insufficient type distinction.
 *
 * This function shows CWE-351: Insufficient Type Distinction by treating different data types
 * as if they were the same.
 */
void typeConfusion_CWE351(void)
{
    int intValue = 42;
    void *data = &intValue;
    // Incorrectly casting to a different type without checking.
    float *floatData = (float *)data;
    printf("Float value: %f\n", *floatData);
}

/**
 * @brief Processes input with misinterpretation.
 *
 * This function demonstrates CWE-115: Misinterpretation of Input by misinterpreting
 * the input data encoding.
 *
 * @param input The input string to process.
 */
void processInput_CWE115(const char *input)
{
    char buffer[100];
    // Assuming input is UTF-8 without verification.
    strcpy(buffer, input);
    printf("Processed input: %s\n", buffer);
}
