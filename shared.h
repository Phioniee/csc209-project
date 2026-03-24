#ifndef SHARED_H
#define SHARED_H

#define MAX_WORD 50
#define MAX_RESULTS 1000
#define MAX_FILENAME 256

// each word and the number of occurences
typedef struct {
    char word[MAX_WORD];
    int count;
} WordCount;

//result from workeer processing file
typedef struct {
    int size;
    WordCount results[MAX_RESULTS];
} WorkerResult;

typedef struct {
    char filename[MAX_FILENAME];
    int is_done;   // 0 = work, 1 = terminate
} Task;

#endif