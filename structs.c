#include <stdio.h>

struct student {
	char name[50];
	int age;
	char grade;
};

void birthday(struct student *s) {
	s->age = s->age + 1;
}
void print_student(struct student s) {
	printf("Name: %s, Age: %d, Grade: %c\n", s.name, s.age, s.grade);
}
int main() {
	struct student s = { "Ahmed", 17, 'A' };

	print_student(s);

	birthday(&s);

	print_student(s);
}
