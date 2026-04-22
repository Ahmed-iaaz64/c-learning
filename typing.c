#include <stdio.h>
#include <time.h>

int main() {
	char input[200];
	struct timespec start_t, end_t;
	double diff_t;

	printf("Type this: The quick brown fox jumps over the lazy dog\n");
	printf("> ");

	clock_gettime(CLOCK_MONOTONIC, &start_t);
	fgets(input, 200, stdin);
	clock_gettime(CLOCK_MONOTONIC, &end_t);

	diff_t = (end_t.tv_sec - start_t.tv_sec) +
		 (end_t.tv_nsec - start_t.tv_nsec) / 1e9;

	printf("You typed: %s\n", input);
	printf("You typed in %.2f seconds\n", diff_t);

	int words = 1;
	for (int i = 0; input[i] != '\0'; i++) {
		if (input[i] == ' ') {
			words++;
		}
	}
	float wpm = (words / diff_t) * 60;
	printf("WPM: %.2f\n", wpm);


	return 0;
}
