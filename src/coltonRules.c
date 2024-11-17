
/*
 * Author: Colton Longstreth
 * Summary: This is the functionality for a simple calculator to show CWEs that deal with Integer Overflow and Underflow along with Bitwise operations
 */

#include <stdio.h>
#include <limits.h>

#include "coltonRules.h"

/**
 * @brief a function that takes 2 integers that will be added to each other if it would not result in an integer overflow
 * @param firstNumber first number that will be added to secondNumber
 * @param secondNumber second number that will be added to firstNumber
 * @return int
 */
int addition(int firstNumber, int secondNumber)
{
        int total;
        if (firstNumber > INT_MAX - secondNumber)
        { // CWE 190 Intger Overflow
            printf("Error: Integer Overflow exiting without operation returning first number.\n");
            return firstNumber;
        }
        else
        {
            total = firstNumber + secondNumber;
        }
    return total;
}

/**
 * @brief a function that takes 2 integers and subtracts the second from the first checking for integer underflow beforehand
 * @param firstnumber first number input, will have second number subtracted from it
 * @param secondNumber second number input, will be subtracted from first number
 * @return int
 */
int subtraction(int firstNumber, int secondNumber)
{
        int total;
        if (firstNumber < INT_MIN + secondNumber)
        { // CWE 191 Integer Underflow
            printf("Error: Integer Underflow, exiting without operation returning first number.\n");
            return firstNumber;
        }
        else
        {
            total = firstNumber - secondNumber;
        }
    return total;
}

/**
 * @brief a function that takes two values and multiplies them checking for overflows before multplication
 * @param firstNumber first integer input that will be multiplied by the secondNumber
 * @param secondNumber second integer input that will be multiplied by firstNumber
 * @return int
 */
int multiplication(int firstNumber, int secondNumber)
{
        int total;
        if (firstNumber > INT_MAX / secondNumber)
        { // CWE 190 Integer Overflow
            printf("Error: Integer Overflow exiting without operation returning first number.\n");
            return firstNumber;
        }
        else
        {
            total = firstNumber * secondNumber;
        }
    return total;
}

/**
 * @brief a function that takes 2 integers and divides the first integer by the second checking for division by zero
 * @param firstNumber the integer that will be divided by the secondNumber
 * @param secondNumber the integer that will divide the firstNumber
 * @return int
 */
int division(int firstNumber, int secondNumber)
{
        int total;
        // CWE 369: Divide by Zero
        if (secondNumber == 0)
        {
            printf("Error: cannot divide by zero, returning first number.\n");
            return firstNumber;
        }
        else
        {
            total = firstNumber / secondNumber;
        }
    return total;
}

/**
 * @brief a function that takes 2 integers, 1 for number to be multiplied and 1 for the power of two to multiply by
 * @param firstNumber the number that will be multiplied by the power of 2
 * @param powerOfTwo the power of two that will be used for the operation
 * @return int
 */
int multiplyByPowerOfTwo(int firstNumber,int powerOfTwo)
{
    if (powerOfTwo >= (sizeof(firstNumber) * 8))
    { // CWE 1335 Incorrect Bitwise Shift of Integer
        printf("Error: Integer overflow, returning first number without multiplying by 2^%d.\n", powerOfTwo);
        return firstNumber;
    }
    else if (powerOfTwo < 0)
    { // CWE 1335 Incorrect Bitwise Shift of Integer
        printf("Error: Incorrect Bitwise Shift returning first number without multiplying by 2^%d\n", powerOfTwo);
        return firstNumber;
    }
    else
    {
        int total = firstNumber << powerOfTwo;
        return total;
    }
}
