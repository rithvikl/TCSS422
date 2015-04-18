#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int count = 0;
int turn = 1;

void* run(void* arg);
void pause();

int main(int argc, char* argv[]) {
	pthread_t thread;
	int rv;

	rv = pthread_create(&thread, NULL, run, (void*)2);
	if (rv != 0) {
		printf("ERROR: unable to create new thread.\n");
		return EXIT_FAILURE;
	}
	run((void*)1);

	pthread_join(thread, NULL);
	return EXIT_SUCCESS;
}

void* run(void* arg) {
	while (count < 100) {
		/* Start critical region */
		while (turn != (int)arg);
		count++;
		pause();
		printf("%d -> %d\n", (int)arg, count);
		turn = 3 - (int)arg;
		/* End critical region */
		pause();
	}
	return NULL;
}

void pause() {
	for (int i = 0; i < 20000000; i++);
}
