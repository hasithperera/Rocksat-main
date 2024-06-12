#include <stdio.h>
#include <time.h>

int main() {
    struct timespec start, end;
    long long elapsed_ns;

    // Record the start time
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Perform some operations
	usleep(100);
	usleep(100);
	sleep(1);
    // Record the end time
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculate the elapsed time
    elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);

    printf("Elapsed time: %f ms\n", elapsed_ns/1000000.0);


    return 0;
}
