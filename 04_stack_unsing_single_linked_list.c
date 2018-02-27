#include <stdio.h>
#include <stdlib.h>

#define STACK_OVERFLOW  -100
#define STACK_UNDERFLOW -101

typedef int T;

typedef struct Node_t {
	T value;
	struct Node_t *next;
} Node;

void push(Node **head, T value) {
	Node *newNode = malloc(sizeof(Node));
	if (newNode == NULL) {
		exit(STACK_OVERFLOW);
	}
	newNode->next = *head;
	newNode->value = value;
	*head = newNode;
}

T pop(Node **head) {
	Node *nodeToDelete;
	T value;
	if ((*head) == NULL) {
		exit(STACK_UNDERFLOW);
	}
	nodeToDelete = *head;
	value = nodeToDelete->value;
	*head = (*head)->next;
	free(nodeToDelete);
	return value;
}

Node* pop2(Node **head) {
	Node *nodeToReturn;
	T value;
	if ((*head) == NULL) {
		exit(STACK_UNDERFLOW);
	}
	nodeToReturn = *head;
	*head = (*head)->next;
	return nodeToReturn;
}

T peek(Node *head) {
	if (head == NULL) {
		exit(STACK_UNDERFLOW);
	}
	return head->value;
}

void printStack(const Node *head) {
	printf("stack > ");
	while(head) {
		printf("%d ", head->value);
		head = head->next;
	}
}

size_t getSize(const Node *head) {
	size_t size = 0;
	while(head) {
		size++;
		head = head->next;
	}
	return size;
}

int main(int argc, char *argv[]) {
	int i;
	Node *head = NULL;
	
	printf("Push values to the stack.\n");
	for(i = 10; i != 0; i--) {
		push(&head, i);
	}
	printf("Size of the stack: %d.\n", getSize(head));
	
	printf("\nValues:\n");
	while(head) {
		printf("%d ", peek(head));
		printf("%d ", pop(&head));
	}
	
	printf("\n\nEnd of story... :(\n");
	
	return 0;
}

