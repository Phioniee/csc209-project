#include "process_manager.h"
#include "worker.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void create_workers(WorkerInformation workers[], int num_workers){
    for (int i = 0; i < num_workers; i++) {

        //create pipes and check for errors
        if (pipe(workers[i].to_worker) == -1) {
            perror("pipe to_worker");
            exit(1);
        }

        if (pipe(workers[i].from_worker) == -1) {
            perror("pipe from_worker");
            exit(1);
        }

        //Fork process
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            exit(1);
        }

        //Child process/worker
        if (pid == 0) {

            close(workers[i].to_worker[1]);   // child doesnt write
            close(workers[i].from_worker[0]); // child doesnt read

            // TODO: kate in workers.c
            worker_loop(
                workers[i].to_worker[0],
                workers[i].from_worker[1]
            );

            //close used ends
            close(workers[i].to_worker[0]);
            close(workers[i].from_worker[1]);

            exit(0);
        } else {    //Parent process

            workers[i].pid = pid;

            //close unused ends
            close(workers[i].to_worker[0]);   // parent doesnt read
            close(workers[i].from_worker[1]); // parent doesnt write
        }
    }
}


void send_task(int worker_fd, const char *filename){
    Task task = {0};

    strncpy(task.filename, filename, MAX_FILENAME - 1);
    task.filename[MAX_FILENAME - 1] = '\0';
    task.is_done = 0;

    if (write(worker_fd, &task, sizeof(Task)) != sizeof(Task)) {
        perror("write task");
        exit(1);
    }
}


void send_terminate(int worker_fd){
    Task task = {0};
    task.is_done = 1;

    if (write(worker_fd, &task, sizeof(Task)) != sizeof(Task)) {
        perror("write task");
        exit(1);
    }
}

void receive_results(int worker_fd, WordCount *results, int *size){
    ssize_t bytes_read;

    //Read size
    bytes_read = read(worker_fd, size, sizeof(int));
    if (bytes_read != sizeof(int)) {
        perror("read size");
        exit(1);
    }

    if (*size < 0 || *size > MAX_RESULTS) {
        fprintf(stderr, "Invalid result size: %d\n", *size);
        exit(1);
    }

    // Read results array
    if (*size > 0) {
        bytes_read = read(worker_fd, results, (*size) * sizeof(WordCount));
        if (bytes_read != (ssize_t)(*size * sizeof(WordCount))) {
            perror("read results");
            exit(1);
        }
    }
}

void close_parent_pipes(WorkerInformation workers[], int num_workers){
    for (int i = 0; i < num_workers; i++) {
        close(workers[i].to_worker[1]);
        close(workers[i].from_worker[0]);
    }
}

void wait_for_workers(WorkerInformation workers[], int num_workers){
    for (int i = 0; i < num_workers; i++) {
        waitpid(workers[i].pid, NULL, 0);
    }
}