#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

struct task {
	int done;
	char description[100];
};

struct linked_list {
	struct task task;
	struct linked_list *next;
};

struct linked_list *add_task(struct linked_list *head, char *description, int done) {
	struct task *new_task = malloc(sizeof(struct task));

	new_task->done = done;
	strcpy(new_task->description, description);

	struct linked_list *new_node = malloc(sizeof(struct linked_list));
	new_node->task = *new_task;
	new_node->next = NULL;

	struct linked_list *current = head;
	if (head == NULL) return new_node;
	while (current != NULL) {
		if (current->next == NULL) {
			current->next = new_node;
			break;
		}
		current = current->next;
	}
	return head;
}

struct linked_list *mark_done(struct linked_list *head, int num) {
	struct linked_list *current = head;
	int count = 1;
	while (current != NULL) {
		if (count == num) {
			current->task.done = 1;
		}
		current = current->next;
		count++;
	}
	return head;
}

struct linked_list *delete_task(struct linked_list *head, int num) {
	struct linked_list *prev = NULL;
	struct linked_list *current = head;
	int count = 1;
	while (current != NULL) {
		if (count == num) {
			if (prev == NULL) {
				head = current->next;
			} else {
				prev->next = current->next;
			}
			free(current);
			return head;
		}
		prev = current;
		current = current->next;
		count++;
	}
	return head;
}

struct linked_list *load_tasks(FILE *f) {
	struct linked_list *head = NULL;
	char line[100];
	while (fgets(line, 100, f) != NULL) {
		char desc[100];
		int done;
		sscanf(line, "%d %[^\n]",&done, desc);
		head = add_task(head, desc, done);
	}
	return head;
}

void save_tasks(struct linked_list *head) {
	FILE *f = fopen("tasks.txt", "w"); 
	struct linked_list *current = head;
	while (current != NULL) {
		fprintf(f, "%d %s\n", current->task.done, current->task.description);
		current = current->next;
	}
}

void print_tasks(struct linked_list *head) {
	struct linked_list *current = head;
	int i = 1;
	printf("\n");
	while (current != NULL) {
		printf("%d. %s [%s]\n", i, current->task.description, current->task.done ? "X" : " ");
		current = current->next;
		i++;
	}
}

int main() {
	FILE *f = fopen("tasks.txt", "a+");
	struct linked_list *head = load_tasks(f);

	int choice;
	int mark_num;
	int delete_num;
	char desc[100];

	while (1) {
		printf("\n1. View tasks\n2. Add task\n3. Mark task as done\n4. Delete task\n5. Quit\n\n");
		scanf("%d", &choice);

		if (choice == 1) {
			print_tasks(head);
		}
		else if (choice == 2) {
			printf("\nEnter task: ");
			scanf(" %[^\n]", desc);
			head = add_task(head, desc, 0);
			save_tasks(head);
		}
		else if (choice == 3) {
			printf("\nSelect task number to mark done: ");
			scanf("%d", &mark_num);
			head = mark_done(head, mark_num);
			save_tasks(head);
		}
		else if (choice == 4) {
			printf("\nSelect task number to delete: ");
			scanf("%d", &delete_num);
			head = delete_task(head, delete_num);
			save_tasks(head);
		}
		else if (choice == 5) {
			break;
		}
	}

	fclose(f);
	return 0;
}

