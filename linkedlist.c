#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct linked_list {
	int value;
	struct linked_list *next;
};

void print_list(struct linked_list *head) {
	struct linked_list *current = head;
	while (current != NULL) {
		printf("%d\n", current->value);
		current = current->next;
	}
}

void add_node(struct linked_list *head, int x) {
	struct linked_list *new_node = malloc(sizeof(struct linked_list));
	new_node->value = x;
	new_node->next = NULL;

	struct linked_list *current = head;
	while (current != NULL) {
		if (current->next == NULL) {
			current->next = new_node;
			break;
		}
		current = current->next;
	}
}

int main() {
	struct linked_list *node1 = malloc(sizeof(struct linked_list));
	node1->value = 10;

	struct linked_list *node2 = malloc(sizeof(struct linked_list));
	node2->value = 20;

	node1->next = node2;
	node2->next = NULL;

	print_list(node1);
	add_node(node1, 30);
	print_list(node1);
}
