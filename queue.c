#include <stdio.h>
#include <stdlib.h>

struct queue {
	int array[100];
	int front;
	int back;
};

void print_queue(struct queue *q) {
	for (int i = q->front + 1; i <= q->back; i++) {
		printf("%d\n", q->array[i]); 
	}
}

void enqueue(struct queue *q, int x) {
	q->back++;
	q->array[q->back] = x;
}

int dequeue(struct queue *q) {
	q->front++;
	return q->array[q->front];
}

int main() {
	struct queue *q = malloc(sizeof(struct queue));
	q->front = -1;
	q->back = -1;

	enqueue(q, 1);
	enqueue(q, 2);
	enqueue(q, 3);
	enqueue(q, 4);
	print_queue(q);

	dequeue(q);
	print_queue(q);
}
