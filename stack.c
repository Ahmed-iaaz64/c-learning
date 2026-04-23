#include <stdio.h>
#include <stdlib.h>

struct stack{
	int array[100];
	int top;
};

void print_stack(struct stack *s) {
	for (int i = 0; i <= s->top; i++) {
		printf("%d\n", s->array[i]);
	}
}
void push(struct stack *s, int x) {
	s->top++;
	s->array[s->top] = x;
}

int pop(struct stack *s) {
	int out = s->array[s->top];
	s->array[s->top] = 0;
	s->top--;
	return out;
}

int main() {
	struct stack *s = malloc(sizeof(struct stack));
	s->top = -1;
	push(s, 5);
	push(s, 6);
	push(s, 2);
	push(s, 4);

	print_stack(s);
	int popped = pop(s);

	print_stack(s);
	printf("%d\n", popped);
}
