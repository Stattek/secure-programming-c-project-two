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

#define STR_ARRAY_SIZE 20

const char *validChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

// a linkedlist of jobs
typedef struct jobList
{
    struct jobList *nextJob;
    char theStr[STR_ARRAY_SIZE + 1]; // +1 to hold null terminating character
} jobList;

bool continueProcessingFlag;
jobList *jobs;
jobList *completedJobs;

// CWE 833: Preventing deadlock by having threads share resources but do not use more than one at a time, which prevents a circular wait.
pthread_mutex_t continueProcessingFlagMutex;
pthread_mutex_t jobListMutex;
pthread_mutex_t completedJobsMutex;

static void addCompletedJob(char *jobStr, int jobStrLen)
{
    jobList *newCompletedJob = malloc(sizeof(jobList));
    if (!newCompletedJob)
    {
        fprintf(stderr, "Error creating completed job for completed job list\n");
        return;
    }

    strncpy(newCompletedJob->theStr, jobStr, jobStrLen);
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

static void *performJob(void *arg)
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
            addCompletedJob(currentJob->theStr, STR_ARRAY_SIZE);

            // cleanup this job
            free(currentJob);
        }
    }
    return NULL;
}

static void addJob(char *jobStr, int jobStrLen)
{
    jobList *newJob = malloc(sizeof(jobList));
    if (!newJob)
    {
        fprintf(stderr, "Error creating new job for job list\n");
    }

    strncpy(newJob->theStr, jobStr, jobStrLen);
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

static void initializeValues(void)
{
    pthread_mutex_init(&jobListMutex, NULL);
    pthread_mutex_init(&continueProcessingFlagMutex, NULL);
    continueProcessingFlag = true;
    jobs = NULL;

    printf("Enter first string to remove invalid characters from\n>> ");
    char userInput[STR_ARRAY_SIZE + 1] = "";
    fgets(userInput, STR_ARRAY_SIZE, stdin);
    addJob(userInput, STR_ARRAY_SIZE);
}

static void setContinueProcessingFlag(bool value)
{
    pthread_mutex_lock(&continueProcessingFlagMutex);
    continueProcessingFlag = value;
    pthread_mutex_unlock(&continueProcessingFlagMutex);
}

static void trimUserInput(char *userStr)
{
    int userStrLen = strlen(userStr);
    // get rid of trailing endline character
    if (userStr[userStrLen - 1] == '\n')
    {
        userStr[userStrLen - 1] = 0;
    }
}

void removeInvalidChars(void)
{
    initializeValues();
    pthread_t thread1;
    pthread_t thread2;

    pthread_create(&thread1, NULL, &performJob, NULL);
    pthread_create(&thread2, NULL, &performJob, NULL);

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
            addJob(userInput, STR_ARRAY_SIZE);
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