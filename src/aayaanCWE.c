#include "aayaanCWE.h"

// CWE-1116: Inaccurate Comments - The comments on functions accurately describe the parameters and purpose of the function.
// If these comments were inaccurate then developers would rely on faulty assumptions while programming.
/**
 * @author Aayaan Shaikh
 * @brief function that dynamically adds words if they are unique to the UniqueWordData Structure
 * @version 0.1
 * @date 2024-11-16
 * @param wordData Struct to hold data on unique words
 * @param word word to be added to the array if it is unique 
 * @return void*
 */
static void addUniqueWord(UniqueWordData *wordData, const char *word)
{
    // check if the word is unique
    for (size_t i = 0; i < wordData->count; i++)
    {
        // CWE-481: Assigning instead of Comparing - Here we compare making sure not to assign
        // We make sure we never assign instead of comparing by placing the constant on the left, if we used = we would get a compiler error instead of logic error. 
        if (0 == strcmp(wordData->words[i], word))
        {
            wordData->countAll++; //update total count

            return; // word is already in array
        }
    }

    // if not, add it to the array
    if (wordData->count >= wordData->capacity)
    {
        // expand the array if necessary
        wordData->capacity *= 2;
        wordData->words = realloc(wordData->words, wordData->capacity * sizeof(char *));
        if (!wordData->words)
        {
            perror("Failed to reallocate memory");
            exit(EXIT_FAILURE);
        }
    }

    wordData->words[wordData->count] = strdup(word); // save unique word
    wordData->count++;                               // increment count
    wordData->countAll++;
}

// CWE-1116: Inaccurate Comments - correct comments for function behavior and parameters
/**
 * @author Aayaan Shaikh
 * @brief function that processes lines from the file, parsing words passing them to addUniqueWord 
 * @version 0.1
 * @date 2024-11-16
 * @param wordData Struct to hold data on unique words
 * @param word word to be added to the array if it is unique 
 * @return pointer to UniqueThreadData struct
 */
static void *processFile(void *arg)
{
    UniqueThreadData *data = (UniqueThreadData *)arg;
    FILE *file = fopen(data->filename, "r");
    if (!file)
    {
        perror("Error: Cannot open file");
        return NULL;
    }

    char buffer[256]; // buffer to hold lines 
    size_t currentLine = 0;
    char *saveptr = NULL; // for thread safe strtok_r

    // go to start line
    while (currentLine < data->startLine && fgets(buffer, sizeof(buffer), file) != NULL)
    {
        currentLine++;
    }

    // process lines 
    while (currentLine < data->endLine && fgets(buffer, sizeof(buffer), file) != NULL)
    {
        // word seperators
        char *token = strtok_r(buffer, " \n\t\r.,;!?:\"'()[]{}<>", &saveptr);
        while (token != NULL)
        {
            
            // add words if they are unique
            // CWE-1116: Inaccurate Comments- "add words if they are unique"
            // if comment was incorrect then developer would extraneously check for uniqueness before adding to array
            // CWE-366: Race Condition within a Thread - critical section is locked to ensure thread safety
            // if critical section was unlocked here the race condition would result in different random outputs.
            pthread_mutex_lock(&mutex);
            addUniqueWord(&finalWordData, token);
            pthread_mutex_unlock(&mutex);

            token = strtok_r(NULL, " \n\t\r.,;!?:\"'()[]{}<>", &saveptr);
        }
        currentLine++;
    }

    fclose(file);
    return NULL;
}

/**
 * @author Aayaan Shaikh
 * @brief function that prints the number of unique words in a file
 * @version 0.1
 * @date 2024-11-16
 * @param char filename, the file to be processed
 * @return void*
 */
void countUniqueWords(const char *filename)
{
    // open the file to count lines
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error: Cannot open file");
        return;
    }

    // count the total lines
    size_t totalLines = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        totalLines++;
    }
    fclose(file);

    // initialize thread data
    // CWE-909: Missing Initialization of Resource - all resources are initialized before being used
    // iterating a value before initializing it would result in unperdicatble behavior like random junk values in the field.
    finalWordData.words = malloc(UNIQUE_INITIAL_WORD_COUNT * sizeof(char *));
    finalWordData.count = 0;
    finalWordData.countAll = 0;
    finalWordData.capacity = UNIQUE_INITIAL_WORD_COUNT;
    pthread_mutex_init(&mutex, NULL);

    // thread creation
    pthread_t threads[UNIQUE_MAX_THREADS];
    UniqueThreadData threadData[UNIQUE_MAX_THREADS];

    size_t linesPerThread = totalLines / UNIQUE_MAX_THREADS;
    for (size_t i = 0; i < UNIQUE_MAX_THREADS; i++)
    {
        threadData[i].filename = filename;
        threadData[i].startLine = i * linesPerThread;
        // CWE-481: Assigning instead of Comparing - here we make sure to compare instead of assigning
        threadData[i].endLine = (i == UNIQUE_MAX_THREADS - 1) ? totalLines : (i + 1) * linesPerThread;

        pthread_create(&threads[i], NULL, processFile, &threadData[i]);
    }

    // wait for threads to finish
    for (size_t i = 0; i < UNIQUE_MAX_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

   pthread_mutex_destroy(&mutex);

    // calculate vocabulary statistics
    // CWE-783: Operator Precedence Logic Error - divide before multiplying by 100 for percentage
    // Use parenthesis for proper explicit order of operations in expression
    float percentUnique = ((float)finalWordData.count / finalWordData.countAll) * 100.0;
    const char *vocabAdj = (percentUnique < 3.0) ? "sparse" :
                           (percentUnique < 5.0) ? "standard" : "verbose";

    printf("Number of unique words: %zu\n", finalWordData.count);
    printf("Based on the percentage of unique words, this work's vocabulary is %s!\n", vocabAdj);

    // cleanup
    // CWE-415: Double Free - avoided any extra or double free()
    // Correct iteration and careful code review to avoid double free and corrupting the memory structure.
    for (size_t i = 0; i < finalWordData.count; i++)
    {
        free(finalWordData.words[i]);
    }
    free(finalWordData.words);
}
