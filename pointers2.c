#include <stdio.h>

int main() {
	int array[5] = { 10,20,30,40,50 };
	int *p = array;

	for (int i = 0; i < 5; i++) {
		printf("%d\n", *p);
		printf("Address: %p, Value: %d\n", p, *p);
		p++;
	}

	int x = 5;
	int *px = &x;
	int **pp = &px;

	printf("Double pointers:\nValue of x: %d\nAddress of x: %p\nAddress of px: %p\n", **pp, *pp, pp);
}
