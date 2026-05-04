#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		return 1;
	}

	FILE *f = fopen(argv[1], "r");
	if (f == NULL) {
		printf("Error: Could Not Open File.\n");
		return 1;
	}

	int program_size = 30000;
	unsigned char program[program_size];
	int len = 0;
	int ch;
	int tape[program_size];
	memset(tape, 0, sizeof(tape));
	int idx = 0;

	while ((ch = fgetc(f)) != EOF) {
		program[len++] = ch;
	}
	program[len] = '\0';

	int ip = 0;
	while (ip < len) {
		ch = (char)program[ip];
		if (ch == '>' && idx < program_size - 1) {
			idx+=1;
		} 
		else if (ch == '<' && idx > 0) {
			idx-=1;
		}
		else if (ch == '+') {
			tape[idx] += 1;
		}
		else if (ch == '-') {
			tape[idx] -= 1;
		}
		else if (ch == '.') {
			printf("%c", tape[idx]);
			fflush(stdout);
		}
		else if (ch == '[') {
			if (tape[idx] == 0) {
				int depth = 1;
				while (depth > 0) {
					ip++;
					if (program[ip] == '[') depth++;
					if (program[ip] == ']') depth--;
				}
			}
		}
		else if (ch == ']') {
			if (tape[idx] != 0) {
				int depth = 1;
				while (depth > 0) {
					ip--;
					if (program[ip] == ']') depth++;
					if (program[ip] == '[') depth--;
				}
			}
		}
		ip++;
		// printf("%d %d\n", idx, tape[idx]);
	}

	fclose(f);
	return 0;
}
