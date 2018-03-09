/*
	Singly linked list
	==================

	Optimized lined list from previous lesson.


*/

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	void *value;
	struct Node *next;
} Node;

typedef struct LinkedList {
	Node *head;
	Node *tail;
	size_t size;
	void* (*copy)(void *);
	void (*ifree)(void *);
} LinkedList;

typedef enum LINKED_LIST_ERRORS {
	MEMORY_EXHAUST,
	LIST_UNDERFLOW,
	BAD_ELEMENT_NUMBER
} LINKED_LIST_ERRORS;

static char *LINKED_LIST_ERRORS_STR[3] = {
	"Memory Exhaust: can't allocate memory",
	"List Underflow: can't pop more elements",
	"Bad Element Number: no such element in list"
};

void throwListError(LINKED_LIST_ERRORS err, int line) {
	printf("LinkedList.c [%d] %s", line, LINKED_LIST_ERRORS_STR[err]);
	exit(err);
}

LinkedList* createLinkedList(void* (*copy)(void*), void (*ifree)(void*)) {
	LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
	list->head = list->tail = NULL;
	list->size = 0;
	list->copy = copy;
	list->ifree = ifree;
	return list;
}

void deleteLinkedList(LinkedList **list) {
	Node *node = NULL;
	while ((*list)->head->next) {
		node = (*list)->head;
		(*list)->head = (*list)->head->next;
		(*list)->ifree(node->value);
		free(node);
	}
	(*list)->ifree((*list)->head->value);
	free((*list)->head);
	free(*list);
	*list = NULL;
}

void pushFront(LinkedList *list, void *value) {

	/*
		Push value to the front of the linked list.
		Consider front is head.
	*/

	Node *node = (Node *)malloc(sizeof(Node));
	if (node == NULL) {
		throwListError(MEMORY_EXHAUST, __LINE__);
	}
	node->next = list->head;
	node->value = list->copy(value);
	list->head = node;
	if (list->tail == NULL) {
		list->tail = node;
	}
	list->size++;
}

void pushBack(LinkedList *list, void *value) {
	Node *node = (Node *)malloc(sizeof(Node));
	if (node == NULL) {
		throwListError(MEMORY_EXHAUST, __LINE__);
	}
	node->next = NULL;
	node->value = list->copy(value);
	if (list->tail) {
		list->tail->next = node;
	} else {  // list->tail == NULL and list->head == NULL
		list->head = node;
	}
	list->tail = node;
	list->size++;
}

void* popFront(LinkedList *list) {
	Node *node = NULL;
	void *value = NULL;
	if (list->head == NULL) {
		throwListError(LIST_UNDERFLOW, __LINE__);
	}
	node = list->head;
	value = node->value;
	list->head = list->head->next;
	if (list->tail == node) {  // tail points to last node (node to delete)
		list->tail = NULL;
	}
	free(node);
	list->size--;
	return value;
}

Node* getNth(LinkedList *list, size_t index) {
	Node *node = NULL;
	size_t counter = 0;
	node = list->head;
	while((counter++) < index && node != NULL) {
		if (node == NULL) {
			throwListError(BAD_ELEMENT_NUMBER, __LINE__);
		}
		node = node->next;
	}
	return node;
}

void* popBack(LinkedList *list) {
	Node *node = NULL;
	void  *value = NULL;
	if (list->tail == NULL) {
		throwListError(LIST_UNDERFLOW, __LINE__);
	}
	node = list->head;
	if (node->next == NULL) {
		value = node->value;
		list->head = list->tail = NULL;
		free(node);
		list->size--;
		return value;
	}
	value = list->tail->value;
	while(node->next->next) {
		node = node->next;
	}
	// `node` indicates to the last past end node.
	free(list->tail);
	node->next = NULL;
	list->tail = node;
	list->size--;
	return value;
}

int* copyInt(int *value) {
	int *newValue = (int *)malloc(sizeof(int));
	*newValue = *value;
	return newValue;
}

void freeInt(int *value) {
	free(value);
}

void printList(LinkedList *list, void (*func)(void *)) {
	Node *node = list->head;
	while (node) {
		func(node->value);
		node = node->next;
	}
	printf("\n");
}

void printInt(void *value) {
	printf("%d ", *((int *)value));
}

int main(void) {
	LinkedList *list = createLinkedList(
			(void* (*)(void*)) copyInt,
			(void (*)(void*)) freeInt
	);
	int a, b, c;
	int *value;

	a = 10;
	b = 20;
	c = 30;

	printf("pushFront a=%d\n", a);
	pushFront(list, &a);

	printf("pushFront b=%d\n", b);
	pushFront(list, &b);

	printf("pushFront c=%d\n", c);
	pushFront(list, &c);

	printf("printList: ");
	printList(list, printInt);

	value = popBack(list);
	printf("popBack value=%d\n", *value);
	printf("printList: ");
	printList(list, printInt);
	freeInt(value);

	value = popFront(list);
	printf("popFront value=%d\n", *value);
	printf("printList: ");
	printList(list, printInt);
	freeInt(value);

	printf("try to change local variables...\n");
	b = 0;  // Does not take affect now.
	        // This variables are copied inside linked list.

	printf("printList: ");
	printList(list, printInt);
	deleteLinkedList(&list);
	return 0;
}
