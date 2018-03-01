#include <stdio.h>
#include <stdlib.h>

typedef int T;

typedef struct Node {
	T value;
	struct Node *next;
} Node;

void push(Node **head, T value) {
	Node *tmp = (Node*)malloc(sizeof(Node));
	tmp->value = value;
	tmp->next = *head;
	*head = tmp;
}

T pop(Node **head) {
	T value;
	Node *prev;
	if ((*head) == NULL) {
		printf("The list is empty.\n");
		return;
	}
	prev = (*head);
	value = prev->value;
	(*head) = (*head)->next;
	free(prev);
	return value;
}

void deleteList(Node **head) {
	Node *prev;
	if ((*head) == NULL) {
		printf("The list is empty.\n");
		return;
	}
	while((*head)->next) {
		prev = (*head);
		(*head) = (*head)->next;
		free(prev);
	}
	free(*head);
}

int main(void) {
	int i;
	Node *head = NULL;
	for (i=0; i<2; i++) {
		push(&head, i);
	}
	for (i=0; i<2; i++) {
		printf("%d\n", pop(&head));
	}
	deleteList(&head);  // The list is empty
	return 0;
}

