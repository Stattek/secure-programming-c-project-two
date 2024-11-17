/**
 * Created by Aayaan Shaikh
 * Summary: Header for Aayaan's CWE demonstrating program.
 */

#ifndef AAYAANCWE_H
#define AAYAANCWE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

#define UNIQUE_MAX_WORD_SIZE 100
#define UNIQUE_INITIAL_WORD_COUNT 100
#define UNIQUE_MAX_THREADS 4

typedef struct
{
    char **words;           //  holds unique words
    size_t count;           // count of unique words
    size_t countAll;           // count all words
    size_t capacity;        // capacity of the unique words array
    pthread_mutex_t mutex;  // mutex for thread safety
} UniqueWordData;

typedef struct
{
    const char *filename;     // name of file to process
    UniqueWordData *wordData; // thread's uniqueWordData struct
    size_t startLine;         // start line
    size_t endLine;           // end line
} UniqueThreadData;

static UniqueWordData finalWordData;
static pthread_mutex_t mutex;

void countUniqueWords(const char *filename);

#endif