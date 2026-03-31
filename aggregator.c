#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "aggregator.h"

/* Merge incoming from worker into final array.
   If a word in incoming is alread in final, add counts.
   If not, add the word and its coount to final (if there is space).

*/
void merge_results(WordCount *final, WordCount *incoming, int *final_size, int incoming_size) {
    for (int i = 0; i < incoming_size; i++) {
        int found = 0;
        for (int j = 0; j < *final_size; j++) {
            if (strcmp(final[j].word, incoming[i].word) == 0) {
                final[j].count += incoming[i].count;
                found = 1;
                break;
            }
        }
        if (!found) {
            if (*final_size < MAX_RESULTS) {
                strcpy(final[*final_size].word, incoming[i].word);
                final[*final_size].count = incoming[i].count;
                (*final_size)++;
            }
        }
    }
}
