#include <stdio.h>
#include <stdlib.h>

struct binarytree {
	int value;
	struct binarytree *right;
	struct binarytree *left;
};

struct binarytree *insert(struct binarytree *root, int x) {
	if (root == NULL) {
		struct binarytree *new_node = malloc(sizeof(struct binarytree));
		new_node->value = x;
		new_node->right = NULL;
		new_node->left = NULL;
		return new_node;
	}
	if (root->value > x) {
		root->left = insert(root->left, x);
	} else if (root->value <= x) {
		root->right = insert(root->right, x);
	}
	return root;
}

int search(struct binarytree *root, int x) {
	if (root == NULL) return 0;
	if (root->value == x) return 1;
	if (x < root->value) return search(root->left, x);
	return search(root->right, x);
}

int main() {
	struct binarytree *bt = NULL; 

	bt = insert(bt, 4);
	bt = insert(bt, 2);
	bt = insert(bt, 4);
	printf("%d\n", bt->value);
	printf("%d\n", bt->left->value);
	printf("%d\n", bt->right->value);
	printf("%d\n", search(bt, 2));
	printf("%d\n", search(bt, 3));
}
