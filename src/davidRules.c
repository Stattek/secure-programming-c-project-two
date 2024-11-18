/**
 * Created by David Slay
 * Summary: Implementation for David's rules.
 */
#include "davidRules.h"
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <errno.h>

const char *validChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

// a linkedlist of jobs
typedef struct jobList
{
    struct jobList *nextJob;
    char theStr[STR_ARRAY_SIZE + 1]; // +1 to hold null terminating character CWE 193 Off by One Error
} jobList;

bool continueProcessingFlag;
jobList *jobs;
jobList *completedJobs;

/**
 * CWE 833: Deadlock
 *
 * Preventing deadlock by having threads share resources,
 * but we do not use more than one at a time, which prevents a circular wait.
 * This will ensure that a deadlock is impossible because we prevent one of the
 * four conditions for deadlock (out of the conditions: circular wait, hold and wait,
 * mutual exclusion, and no preemption).
 */
pthread_mutex_t continueProcessingFlagMutex;
pthread_mutex_t jobListMutex;
pthread_mutex_t completedJobsMutex;

/**
 * @author David Slay
 * @brief Adds a completed job to the completedJobs linked list.
 *
 * @param jobStr The validated string.
 */
static void addCompletedJob(char *jobStr)
{
    /**
     * CWE 252: Unchecked Return Value
     *
     * We avoid this by checking the return value from the malloc() function, which has
     * the possibility of returning a null pointer in cases such as when there is no memory
     * that it can allocate for the variable. If we cannot malloc this node in the job list,
     * we just do not add it.
     */
    jobList *newCompletedJob = malloc(sizeof(jobList));
    if (!newCompletedJob)
    {
        fprintf(stderr, "Error creating completed job for completed job list\n");
        return;
    }

    /**
     * CWE 120: Buffer Copy without Checking Size of Input ('Classic Buffer Overflow')
     *
     * Copy the jobStr buffer into theStr, using strncpy to ensure the size of the input
     * does not cause buffer overflow (truncating the string if it is larger than the
     * destination buffer, theStr).
     */
    strncpy(newCompletedJob->theStr, jobStr, STR_ARRAY_SIZE);
    newCompletedJob->nextJob = NULL;

    pthread_mutex_lock(&completedJobsMutex);
    // add the newly completed job at the end of the list
    if (completedJobs)
    {
        jobList *curJob = completedJobs;

        // find the end of the list
        while (curJob->nextJob != NULL)
        {
            curJob = curJob->nextJob;
        }

        curJob->nextJob = newCompletedJob;
    }
    else
    {
        completedJobs = newCompletedJob;
    }
    pthread_mutex_unlock(&completedJobsMutex);
}

/**
 * @author David Slay
 * @brief Function to be performed by threads. Continuously executes jobs
 * by removing all non-characters from strings that are input by the user.
 * This function will stop once the continueProcessingFlag has been set to
 * false.
 *
 * @param arg Thread arguments.
 */
static void *executeJob(void *arg)
{
    while (true)
    {
        // check flag
        bool continueProcessing = true;
        pthread_mutex_lock(&continueProcessingFlagMutex);
        continueProcessing = continueProcessingFlag;
        pthread_mutex_unlock(&continueProcessingFlagMutex);

        if (!continueProcessing)
        {
            // stop processing jobs
            break;
        }

        jobList *currentJob = NULL;

        // lock the mutex so we can remove all invalid characters from the string
        // CWE 833: avoiding deadlock by using one mutex lock at a time
        pthread_mutex_lock(&jobListMutex);
        // take the job from the front of the list
        if (jobs)
        {
            currentJob = jobs;
            jobs = jobs->nextJob;
        }
        pthread_mutex_unlock(&jobListMutex);

        if (currentJob)
        {
            // remove invalid characters
            int curJobStrLength = strlen(currentJob->theStr);
            int curNumValidChars = strspn(currentJob->theStr, validChars);
            while (curNumValidChars != curJobStrLength)
            {
                // remove the next character from the string and shift everything left
                int length = STR_ARRAY_SIZE - 1;
                for (int i = curNumValidChars; i < STR_ARRAY_SIZE - 1; i++)
                {
                    currentJob->theStr[i] = currentJob->theStr[i + 1];
                }
                curJobStrLength = strlen(currentJob->theStr);
                curNumValidChars = strspn(currentJob->theStr, validChars);
            }

            // create a new completed job
            addCompletedJob(currentJob->theStr);

            // cleanup this job
            free(currentJob);
        }
    }
    return NULL;
}

/**
 * @author David Slay
 * @brief Adds a string from user input to the job list to be
 * validated and have non-characters removed.
 *
 * @param jobStr The string to modify.
 */
static void addJob(char *jobStr)
{
    /**
     * CWE 252: Unchecked Return Value
     *
     * We avoid this by checking the return value from the malloc() function, which has
     * the possibility of returning a null pointer in cases such as when there is no memory
     * that it can allocate for the variable. If we cannot malloc this node in the job list,
     * we just do not add it.
     */
    jobList *newJob = malloc(sizeof(jobList));
    if (!newJob)
    {
        fprintf(stderr, "Error creating new job for job list\n");
    }

    /**
     * CWE 120: Buffer Copy without Checking Size of Input ('Classic Buffer Overflow')
     *
     * Copy the jobStr buffer into theStr, using strncpy to ensure the size of the input
     * does not cause buffer overflow (truncating the string if it is larger than the
     * destination buffer, theStr).
     */
    strncpy(newJob->theStr, jobStr, STR_ARRAY_SIZE);
    newJob->nextJob = NULL;

    pthread_mutex_lock(&jobListMutex);
    // add the new job at the end of the list
    if (jobs)
    {
        jobList *curJob = jobs;

        // find the end of the list
        while (curJob->nextJob != NULL)
        {
            curJob = curJob->nextJob;
        }

        curJob->nextJob = newJob;
    }
    else
    {
        jobs = newJob;
    }
    pthread_mutex_unlock(&jobListMutex);
}

/**
 * @author David Slay
 * @brief Initializes mutexes and global data to be used by threads.
 * Takes first job from the user.
 */
static void initializeValues(void)
{
    pthread_mutex_init(&jobListMutex, NULL);
    pthread_mutex_init(&continueProcessingFlagMutex, NULL);
    continueProcessingFlag = true;
    jobs = NULL;

    printf("Enter first string to remove invalid characters from\n>> ");
    char userInput[STR_ARRAY_SIZE + 1] = "";
    fgets(userInput, STR_ARRAY_SIZE, stdin);
    addJob(userInput);
}

/**
 * @author David Slay
 * @brief Sets the continueProcessingFlag value.
 *
 * @param value The new value.
 */
static void setContinueProcessingFlag(bool value)
{
    pthread_mutex_lock(&continueProcessingFlagMutex);
    continueProcessingFlag = value;
    pthread_mutex_unlock(&continueProcessingFlagMutex);
}

/**
 * @author David Slay
 * @brief Trims the user input by removing the trailing endline character from
 * the string if it exists.
 *
 * @param userStr The user input string.
 */
void trimUserInput(char *userStr)
{
    int userStrLen = strlen(userStr);
    // get rid of trailing endline character
    if (userStr[userStrLen - 1] == '\n')
    {
        userStr[userStrLen - 1] = 0;
    }
}

/**
 * @author David Slay
 * @brief Function that utilizes multithreading to take in user input
 * and remove all characters from user input. Prints the strings after
 * the user quits. Demonstrates how to avoid deadlock when multithreading,
 * utilizing mutual exclusion, hold and wait,
 */
void removeInvalidChars(void)
{
    initializeValues();
    pthread_t thread1;
    pthread_t thread2;

    pthread_create(&thread1, NULL, &executeJob, NULL);
    pthread_create(&thread2, NULL, &executeJob, NULL);

    bool continueReading = true;
    char userInput[STR_ARRAY_SIZE + 1] = "";
    while (continueReading)
    {
        printf("Enter another string to remove invalid characters from (or 'q' to quit)\n>> ");
        fgets(userInput, STR_ARRAY_SIZE, stdin);
        trimUserInput(userInput);
        if (strcmp("q", userInput) == 0)
        {
            continueReading = false;
        }
        else
        {
            addJob(userInput);
        }
    }

    setContinueProcessingFlag(false);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_lock(&completedJobsMutex);
    int index = 1;
    while (completedJobs)
    {
        // print out the completed job and move to the next
        jobList *curJob = completedJobs;
        printf("Validated string %d: %s\n", index, curJob->theStr);
        completedJobs = completedJobs->nextJob;
        index++;

        // cleanup
        free(curJob);
    }
    pthread_mutex_unlock(&completedJobsMutex);
}

/**
 * @author David Slay
 * @brief Creates an account from user input, utilizing regex
 * to validate that the input username and password are valid.
 *
 * @param accountOut The account object output.
 *
 * @returns True upon error, false upon success.
 */
bool createAccount(Account *accountOut)
{
    if (!accountOut)
    {
        return true; // error
    }

    regex_t regex;
    /**
     * CWE 186: Overly Restrictive Regular Expression
     * Avoids the problem of an overly restrictive expression by allowing the user to input
     * any combination of letters and numbers, with some symbols allowed, as well. This regex
     * matches all values that are expected, as we don't want to allow the user to input any other symbols
     * besides these.
     */
    int regerr = regcomp(&regex, "^([0-9a-zA-Z+!@#$%^&*_-])+$", REG_EXTENDED);
    if (regerr)
    {
        fprintf(stderr, "Could not compile regex\n");
        return true;
    }

    char userName[STR_ARRAY_SIZE + 1] = "";
    bool isValidEmail = false;
    do
    {
        printf("Enter your username:\n>> ");
        fgets(userName, STR_ARRAY_SIZE, stdin);
        trimUserInput(userName);
        int regexResult = regexec(&regex, userName, 0, NULL, 0);
        if (regexResult == 0)
        {
            isValidEmail = true;
        }
        else
        {
            printf("Invalid username\n");
        }
    } while (!isValidEmail);
    strncpy(accountOut->userName, userName, STR_ARRAY_SIZE);

    regerr = regcomp(&regex, "^([0-9a-zA-Z+!@#$%^&*()_{}|:;'\",.<>?=-])+$", REG_EXTENDED);
    if (regerr)
    {
        fprintf(stderr, "Could not compile regex\n");
        return true;
    }

    char password[STR_ARRAY_SIZE + 1] = "";
    bool isValidPassword = false;
    do
    {
        printf("Enter password:\n>> ");
        fgets(password, STR_ARRAY_SIZE, stdin);
        trimUserInput(password);
        int regexResult = regexec(&regex, password, 0, NULL, 0);
        if (regexResult == 0)
        {
            isValidEmail = true;
        }
        else
        {
            printf("Invalid password\n");
        }
    } while (!isValidEmail);
    strncpy(accountOut->password, password, STR_ARRAY_SIZE);

    return false; // success
}

/**
 * @author David Slay
 * @brief Prints the info of an account.
 *
 * @param account The account to print.
 *
 * @returns True on error, false on success.
 */
bool printAccountInfo(Account *account)
{
    if (!account)
    {
        return true; // error
    }

    printf("---Account Info---\n");
    printf("Username: %s\n", account->userName);
    printf("Password: %s\n", account->password);

    return false;
}

/**
 * @author David Slay
 * @brief Does a stalin sort of the array and keeps track of the sum before and after the
 * sort, as it is possible and likely that data is lost.
 *
 * @param numArray The array of numbers to be sorted.
 * @param beforeSortSumOut The output sum before the sort.
 * @param afterSortSumOut The output sum after the sort.
 * @param sumOut The sum output.
 *
 * @returns True on error, false otherwise.
 */
bool stalinSort(int *numArray, int *numElements, long int *beforeSortSumOut, long int *afterSortSumOut)
{
    if (!numArray || !numElements || !beforeSortSumOut || !afterSortSumOut)
    {
        return true; // error
    }
    /**
     * CWE 476: NULL Pointer Dereference
     *
     * We avoid breaking this one by checking first that these pointers are not null.
     * If they are, we return, so it becomes impossible that a null pointer is dereferenced.
     */
    *beforeSortSumOut = 0;
    *afterSortSumOut = 0;

    int lastMaxNum;
    if (*numElements >= 0)
    {
        lastMaxNum = numArray[0];
    }

    for (int i = 0; i < *numElements; i++)
    {
        /**
         * CWE 681: Incorrect Conversion between Numeric Types
         *
         * Avoids incorrect conversion by converting an integer to a `long int`,
         * which can represent just as many or more possible values than an `int` can
         * (depends on implementation).
         */
        *beforeSortSumOut += (long int)numArray[i];
    }

    // stalin sort in ascending order
    int currentIndex = 1;
    while (currentIndex < *numElements)
    {
        if (numArray[currentIndex] < lastMaxNum)
        {
            // delete this value
            for (int j = currentIndex; j < (*numElements - 1); j++)
            {
                numArray[j] = numArray[j + 1];
            }
            (*numElements)--;
        }
        else
        {
            lastMaxNum = numArray[currentIndex];
            currentIndex++;
        }
    }

    for (int i = 0; i < *numElements; i++)
    {
        /**
         * CWE 681: Incorrect Conversion between Numeric Types
         *
         * Avoids incorrect conversion by converting an integer to a `long int`,
         * which can represent just as many or more possible values than an `int` can
         * (depends on implementation).
         */
        *afterSortSumOut += (long int)numArray[i];
    }

    return false; // success
}

/**
 * @author David Slay
 * @brief Creates an array on the heap from user input, provided the number of
 * elements to be in the array. NOTE: The array returned must be freed by the user
 * of this function.
 *
 * @param array The array to be allocated and filled.
 * @param numElements The number of elements to be in the array.
 *
 * @returns True on error, false on success.
 */
bool getArrayFromUser(int **array, int numElements)
{
    // check that array is not null
    if (!array)
    {
        return true; // error
    }
    *array = malloc(sizeof(int) * sizeof(numElements));
    // check that array isn't null after malloc
    if (!(*array))
    {
        fprintf(stderr, "Could not allocate array on heap");
        return true; // error
    }

    char userInputStr[STR_ARRAY_SIZE] = "";
    // add all of the elements
    for (int i = 0; i < numElements; i++)
    {
        printf("Enter a value for the array:\n>> ");
        fgets(userInputStr, STR_ARRAY_SIZE, stdin);
        trimUserInput(userInputStr);
        int enteredNum = 0;

        int scanErr = sscanf(userInputStr, "%d", &enteredNum);
        if (scanErr == EOF)
        {
            fprintf(stderr, "Error reading user input\n");
            return true;
        }
        (*array)[i] = enteredNum;
    }

    return false; // success
}

/**
 * @author David Slay
 * @brief Frees array created by getArrayFromUser()
 *
 * @param array The array to free.
 */
void freeArrayFromUser(int **array)
{
    if (array && *array)
    {
        free(*array);
        *array = NULL;
    }
}