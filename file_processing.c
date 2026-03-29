#include <stdio.h>
#include <string.h>
#include "file_processing.h"

void process_file(const char *filename, WordCount *results, int *size) {
    *size = 0;

    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        return; // file couldn't be opened
    }

    char word[MAX_WORD];

    while (fscanf(f, "%49s", word) == 1) {  // read one word at a time
        
        // check if word already exists in results
        int found = 0;
        for (int i = 0; i < *size; i++) {
            if (strcmp(results[i].word, word) == 0) {
                results[i].count++;
                found = 1;
                break;
            }
        }

        // if word is new, add it
        if (!found && *size < MAX_RESULTS) {
            strncpy(results[*size].word, word, MAX_WORD - 1);
            results[*size].word[MAX_WORD - 1] = '\0'; 
            results[*size].count = 1;
            (*size)++;
        }
    }

    fclose(f);
}

// testing individually on pipes
int main() {
    WordCount results[MAX_RESULTS];
    int size = 0;

    process_file("test.txt", results, &size);

    for (int i = 0; i < size; i++) {
        printf("%s: %d\n", results[i].word, results[i].count);
    }
    return 0;
}