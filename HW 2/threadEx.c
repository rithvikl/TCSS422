#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int count = 0;

void* run(void* arg);
void pause();

int main(int argc, char* argv[]) {
	/* The pthread_t structure stores information about a thread, similar
	   to the Thread class in Java. */
	pthread_t thread;
	int rv;

	/* The pthread_create function creates a new thread, utilizing the
	   pthread_t structure passed as the first argument, and starts the
	   thread executing the function specified as the third argument.
	   The forth argument is the argument passed to the function specified
	   as the third argument (which must be passed as a void*). */
	rv = pthread_create(&thread, NULL, run, (void*)2);
	if (rv != 0) {
		printf("ERROR: unable to create new thread.\n");
		return EXIT_FAILURE;
	}
	run((void*)1);

	/* If the "main" thread finishes first, wait for the second thread
	   to finish before terminating. */
	pthread_join(thread, NULL);
	return EXIT_SUCCESS;
}

/* This function contains the "work" performed by both threads.  The function
   could have any name, but is named "run" to show that it serves the same
   purpose as the "run" method in the corresponding Java example.  The
   argument passed to this function is a simple thread id, e.g. 1 or 2. */
void* run(void* arg) {
	while (count < 100) {
		/* Start critical region */
		count++;
		pause();
		printf("%d -> %d\n", (int)arg, count);
		/* End critical region */
		pause();
	}
	return NULL;
}

void pause() {
	for (int i = 0; i < 20000000; i++);
}
