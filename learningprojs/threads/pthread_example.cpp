#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5

// Function that each thread will execute
void *printHello(void *threadId) {
    long tid = (long)threadId;
    printf("Hello from thread %ld!\n", tid);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    for (t = 0; t < NUM_THREADS; t++) {
        printf("Creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, printHello, (void *)t);

        if (rc) {
            printf("Error creating thread; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    // Wait for all threads to finish
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    printf("Main thread exiting.\n");
    pthread_exit(NULL);
}
