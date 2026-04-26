#include <stdio.h>

int main() {
	FILE *f = fopen("output.txt", "w");
	if (f == NULL) {
		printf("Error opening file\n");
		return 1;
	}
	fprintf(f, "Hello from another file\n"); 
	fprintf(f, "This is line 2\n"); 
	fclose(f);

	f = fopen("output.txt", "r");
	if (f == NULL) {
		printf("Error opening file\n");
		return 1;
	}
	char line[100];
	while (fgets(line, 100, f) != NULL) {
		printf("%s", line);
	}
	fclose(f);
}

