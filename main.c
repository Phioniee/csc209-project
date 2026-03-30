#include <stdio.h>
#include <stdlib.h>

#include "shared.h"
#include "process_manager.h"
#include "aggregator.h"
#include "output.h"

int main(int argc, char *argv[]) {

    //program, num_workers, at least 1 file
    if (argc < 3) {
        fprintf(stderr, "Usage: %s num_workers file1 file2 ...\n", argv[0]);
        return 1;
    }

    // number of workers
    int num_workers = atoi(argv[1]);

    if (num_workers < 3) {
        fprintf(stderr, "Number of workers must be at least 3\n");
        return 1;
    }

    int num_files = argc - 2;

    //dynamically allocate workers
    WorkerInformation *workers = malloc(num_workers * sizeof(WorkerInformation));
    if (workers == NULL) {
        perror("malloc workers");
        exit(1);
    }

    // Task tracking array
    int *tasks_per_worker = malloc(num_workers * sizeof(int));
    if (tasks_per_worker == NULL) {
        perror("malloc tasks");
        exit(1);
    }

    for (int i = 0; i < num_workers; i++) {
        tasks_per_worker[i] = 0;
    }

    WordCount final_results[MAX_RESULTS];
    int final_size = 0;

    create_workers(workers, num_workers);

    int next_file = 0;
    int completed = 0;

    // give initial tasks
    for (int i = 0; i < num_workers && next_file < num_files; i++) {
        send_task(workers[i].to_worker[1], argv[next_file + 2]);
        tasks_per_worker[i]++;
        next_file++;
    }

    // dynamic loop
    while (completed < num_files) {

        for(int i = 0; i < num_workers; i++) {

            if(tasks_per_worker[i] > 0) {

                WordCount temp_results[MAX_RESULTS];
                int temp_size = 0;

                receive_results(workers[i].from_worker[0], temp_results, &temp_size);
                merge_results(final_results, temp_results, &final_size, temp_size);

                tasks_per_worker[i]--;
                completed++;

                //assign new task if available
                if (next_file < num_files) {
                    send_task(workers[i].to_worker[1], argv[next_file + 2]);
                    tasks_per_worker[i]++;
                    next_file++;
                }
            }
        }
    }

    //terminate workers
    for (int i = 0; i < num_workers; i++) {
        send_terminate(workers[i].to_worker[1]);
    }

    //Output results
    print_results(final_results, final_size);

    //cleanup
    close_parent_pipes(workers, num_workers);
    wait_for_workers(workers, num_workers);
    free(workers);
    free(tasks_per_worker);

    return 0;
}