#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include "shared.h"
#include <sys/types.h>

typedef struct {
        pid_t pid;
        int to_worker[2];       //parent writes and worker reads
        int from_worker[2];     //worker writes and parent reads
} WorkerInformation;

void create_workers(WorkerInformation workers[], int num_workers);
void send_task(int worker_fd, const char *filename);
void send_terminate(int worker_fd);
void receive_results(int worker_fd, WordCount *results, int *size);
void close_parent_pipes(WorkerInformation workers[], int num_workers);
void wait_for_workers(WorkerInformation workers[], int num_workers);

#endif