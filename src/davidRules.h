/**
 * Created by David Slay
 * Summary: Header for David's rules.
 */
#ifndef DAVIDRULES_H
#define DAVIDRULES_H
#include <stdbool.h>

#define STR_ARRAY_SIZE 1024

typedef struct Account
{
    char userName[STR_ARRAY_SIZE + 1];
    char password[STR_ARRAY_SIZE + 1];
} Account;

void removeInvalidChars(void);
bool createAccount(Account *accountOut);
bool printAccountInfo(Account *account);
bool stalinSort(int *numArray, int *numElements, long int *beforeSortSumOut, long int *afterSortSumOut);
bool getArrayFromUser(int **array, int numElements);
void freeArrayFromUser(int **array);

#endif