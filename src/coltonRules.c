
/*
* Author: Colton Longstreth
*/

#include <stdio.h>
#include <limits.h>

#include "coltonRules.h"

/**
 * @brief a function that starts at an inital value and adds to it based on input given
 * @param int of current value
 * @return int
 */
int addition(int currentTotal)
{
    int numArguments;
    printf("How many times do you want to add to the inital value?\n>> ");
    scanf("%d", &numArguments);

    int addVal;
    for (int i = 0; i < numArguments; i++)
    {
        printf("Enter number to add:\n>> ");
        scanf("%d", &addVal);
        if(currentTotal > INT_MAX - addVal){ //CWE 190 Intger Overflow
            printf("Error: Integer Overflow exiting without last operation.\n");
            return currentTotal;
        }else{
            currentTotal += addVal;
        }
    }
    return currentTotal;
}

/**
 * @brief a function that takes an initial value and subtracts from it based on input given
 * @param int of current value
 * @return int
 */
int subtraction(int currentTotal)
{
    int numArguments;
    printf("How many times do you want to subtract from inital value?\n>> ");
    scanf("%d", &numArguments);

    int subtractVal;
    for (int i = 0; i < numArguments; i++)
    {
        printf("Enter number to subtract:\n>> ");
        scanf("%d", &subtractVal);
        if(currentTotal < INT_MAX + subtractVal){ //CWE 191 Integer Underflow
            printf("Error: Integer Underflow, exiting without last operation.\n");
            return currentTotal;
        }else{
            currentTotal -= subtractVal;
        }
    }
    return currentTotal;
}


/**
 * @brief a function that takes an inital value and multiplies based on input given
 * @param int of current value
 * @return int
 */
int multiplication(int currentTotal)
{
    int numArguments;
    printf("How many times do you want to multiply?\n>> ");
    scanf("%d", &numArguments);

    int multiplyVal;
    for (int i = 0; i < numArguments; i++)
    {
        printf("Enter number to multiply by:\n>> ");
        scanf("%d", &multiplyVal);
        if(currentTotal > INT_MAX / multiplyVal){ //CWE 190 Integer Overflow
            printf("Error: Integer Overflow exiting without last operation.\n");
            return currentTotal;
        }else{
            currentTotal *= multiplyVal;
        }
    }
    return currentTotal;
}

/**
 * @brief a function that takes an inital value and divides based on input given
 * @param int of current value
 * @return int
 */
int division(int currentTotal)
{
    int numArguments;
    printf("How many times do you want to divide?\n>> ");
    scanf("%d", &numArguments);

    int divideVal;
    for (int i = 0; i < numArguments; i++)
    {
        printf("Enter number to divide by:\n>> ");
        scanf(" %d", &divideVal);
        // CWE 369: Divide by Zero
        if (divideVal == 0)
        {
            printf("Error: cannot divide by zero\n");
        }
        else
        {
            currentTotal = currentTotal / divideVal;
        }
    }
    return currentTotal;
}

int multiplyByPowerOfTwo(int currentTotal){
    int powerOfTwo;
    printf("Enter X to multiply current total by 2^x.\n>> ");
    scanf("%d", &powerOfTwo);

    if(currentTotal > INT_MAX / 2){
        printf("Error: Integer overflow, returning without multiplying by 2^%d.\n", powerOfTwo);
        return currentTotal;
    }else if(powerOfTwo < 0){ // CWE 1335 Incorrect Bitwise shift of Integer
        printf("Error: Incorrect Bitwise Shift returning without multiplying by 2^%d\n",powerOfTwo);
        return currentTotal;
    }else{
        currentTotal = currentTotal << powerOfTwo;
    }
}


