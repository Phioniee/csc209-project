#include <stdio.h>
#include "output.h"

void print_results(WordCount *results, int size) {
    printf("%-20s | %s\n", "Word", "Count");
    printf("--------------------+----------\n");

    for (int i = 0; i < size; i++) {
        printf("%-20s | %d\n", results[i].word, results[i].count);
    }
}
