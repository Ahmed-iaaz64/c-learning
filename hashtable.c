#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct entry {
	char key[50];
	char value[50];
};

struct hashtable {
	struct entry array[100];
};

int hash(char *key) {
	int sum = 0;
	for (int i = 0; key[i] != '\0'; i++) {
		sum = sum + key[i];
	}
	return sum % 100;
}

void set(struct hashtable *ht, char *key, char *value) {
	int index = hash(key);

	strcpy(ht->array[index].key, key);
	strcpy(ht->array[index].value, value);
}

char *get(struct hashtable *ht, char *key) {
	int index = hash(key);
	return ht->array[index].value;
}

int main() {
	struct hashtable *ht = malloc(sizeof(struct hashtable));

	set(ht, "name", "Ahmed");
	set(ht, "age", "17");

	printf("%s\n", get(ht, "name"));
	printf("%s\n", get(ht, "age"));
}
