#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PROCESSES 10
#define MAX_MEMORY 1024 * 1024 // Example maximum memory allocation per process (1MB)
#define MIN_PROCESSES 2

typedef enum {
    READY,
    RUNNING,
    TERMINATED
} ProcessState;

typedef struct {
    int pid;
    ProcessState state;
    char name[20];
    void *allocated_memory;  // Pointer to the allocated memory
    size_t memory_size;      // Size of the allocated memory
} Process;

// Global array to store processes
Process process_table[MAX_PROCESSES];
int next_pid = 1;

// Function to create a new process
int create_process(const char *name) {
    if (next_pid > MAX_PROCESSES) {
        printf("Process limit reached. Cannot create more processes.\n");
        return -1;
    }

    Process *new_process = &process_table[next_pid - 1];
    new_process->pid = next_pid++;
    new_process->state = READY;
    strncpy(new_process->name, name, sizeof(new_process->name) - 1);
    new_process->name[sizeof(new_process->name) - 1] = '\0';
    new_process->allocated_memory = NULL;
    new_process->memory_size = 0;

    printf("Process %d (%s) created and is in READY state.\n", new_process->pid, new_process->name);
    return new_process->pid;
}

// Function to allocate memory for a running process
void allocate_memory(Process *process) {
    size_t memory_size = (rand() % (MAX_MEMORY - 1024 + 1)) + 1024;

    process->allocated_memory = malloc(memory_size);
    if (process->allocated_memory == NULL) {
        printf("Process %d (%s) failed to allocate memory.\n", process->pid, process->name);
        return;
    }

    process->memory_size = memory_size;
    printf("Process %d (%s) allocated %zu bytes of memory.\n", process->pid, process->name, memory_size);
}

// Function to run a process
void run_process(Process *process) {
    if (process->state == TERMINATED) {
        printf("Process %d (%s) is already terminated.\n", process->pid, process->name);
        return;
    }

    process->state = RUNNING;
    printf("Process %d (%s) is now RUNNING.\n", process->pid, process->name);

    allocate_memory(process);
}

// Function to terminate a process and free its allocated memory
void terminate_process(Process *process) {
    if (process->state == TERMINATED) {
        printf("Process %d (%s) is already terminated.\n", process->pid, process->name);
        return;
    }

    process->state = TERMINATED;
    printf("Process %d (%s) is now TERMINATED.\n", process->pid, process->name);

    if (process->allocated_memory != NULL) {
        free(process->allocated_memory);
        process->allocated_memory = NULL;
        process->memory_size = 0;
        printf("Process %d (%s) freed its allocated memory.\n", process->pid, process->name);
    }
}

// Function to simulate context switching between processes
void context_switch() {
    int active_processes = next_pid - 1;
    int current_process_index = 0;

    while (active_processes > 0) {
        Process *current_process = &process_table[current_process_index];

        if (current_process->state != TERMINATED) {
            printf("Context switching to Process %d (%s)\n", current_process->pid, current_process->name);
            run_process(current_process);
            terminate_process(current_process);
            active_processes--;
        }

        current_process_index = (current_process_index + 1) % (next_pid - 1);
    }
}

// Function to display all processes
void display_processes() {
    printf("\nProcess Table:\n");
    printf("PID\tName\t\tState\t\tMemory (bytes)\n");
    printf("-----------------------------------------------------\n");
    for (int i = 0; i < next_pid - 1; i++) {
        printf("%d\t%s\t\t%s\t\t%zu\n", process_table[i].pid, process_table[i].name,
               process_table[i].state == READY ? "READY" :
               process_table[i].state == RUNNING ? "RUNNING" : "TERMINATED",
               process_table[i].memory_size);
    }
}

int main() {
    srand(time(NULL));

    int num_processes = (rand() % (MAX_PROCESSES - MIN_PROCESSES + 1)) + MIN_PROCESSES;

    for (int i = 0; i < num_processes; i++) {
        char process_name[20];
        snprintf(process_name, sizeof(process_name), "Process_%d", i + 1);
        create_process(process_name);
    }

    context_switch();

    display_processes();

    return OK;
}

