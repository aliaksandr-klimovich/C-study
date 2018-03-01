#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	void *value;
	struct Node *next;
} Node;

void push(Node **head, void *value) {
	Node *tmp = (Node*)malloc(sizeof(Node));
	tmp->value = value;
	tmp->next = *head;
	*head = tmp;
}

void* pop(Node **head) {
	void *value;
	Node *prev;
	if ((*head) == NULL) {
		printf("pop: The list is empty.\n");
		return NULL;
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
		printf("deleteList: The list is empty.\n");
		return;
	}
	while((*head)->next) {
		prev = (*head);
		(*head) = (*head)->next;
		free(prev);
	}
	free(*head);
	*head = NULL;
}

Node* getNth(Node *head, int n) {
	int counter = 0;
	while (counter < n && head) {
		head = head->next;
		counter++;
	}
	return head;
}

Node* getLast(Node *head) {
	if (head == NULL) {
		printf("getLast: List is empty.\n");
		return NULL;
	}
	while(head->next) {
		head = head->next;
	}
	return head;
}

void pushBack(Node *head, void *value) {
	Node *last = getLast(head);
	Node *tmp = (Node*)malloc(sizeof(Node));
	tmp->next = NULL;
	tmp->value = value;
	last->next = tmp;
}

Node* getLastButOne(Node *head) {
	if (head == NULL) {
		printf("getLastButOne: List is empty.\n");
		return NULL;
	}
	if (head->next == NULL) {
		printf("getLastButOne: Too few nodes. Expected >=2, got 1.\n");
	}
	while(head->next->next) {
		head = head->next;
	}
	return head;
}

void* popBack(Node **head) {
	void *value;
	Node *pFwd;
	Node *pBwd = NULL;
	if (head == NULL || (*head) == NULL) {
		printf("popBack: List is empty.\n");
		return NULL;
	}
	pFwd = *head;
	while (pFwd->next) {
		pBwd = pFwd;
		pFwd = pFwd->next;
	}
	value = pFwd->value;
	if (pBwd == NULL) {
		free(*head);
		*head = NULL;
	} else {
		pBwd->next = NULL;
		free(pFwd);
	}
	return value;
}

typedef struct {
	int x;
	int y;
} Point;

int main(void) {
	int i;
	int array[] = {55, 44, 33, 22, 11};
	int arrayLength = sizeof(array) / sizeof(array[0]);
	Point *pPoint, point = {.x=10, .y=20};
	Node *head = NULL;

	for (i=0; i<arrayLength; i++)
		push(&head, &array[i]);

	for (i=0; i<arrayLength; i++)
		printf("%d\n", *(int *)(pop(&head)));

	push(&head, &point);
	pPoint = pop(&head);
	printf("(%i, %i)\n", pPoint->x, pPoint->y);

	deleteList(&head);  // The list is empty
	return 0;
}
