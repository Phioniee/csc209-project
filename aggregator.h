#ifndef AGGREGATOR_H
#define AGGREGATOR_H

#include "shared.h"

void merge_results(WordCount *final, WordCount *incoming, int *final_size, int incoming_size);

#endif