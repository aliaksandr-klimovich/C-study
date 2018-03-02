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

LinkedList* createLinkedList() {
	LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
	list->head = list->tail = NULL;
	list->size = 0;
	return list;
}

void deleteLinkedList(LinkedList **list) {
	Node *node = NULL;
	while ((*list)->head->next) {
		node = (*list)->head;
		(*list)->head = (*list)->head->next;
		free(node);
	}
	free((*list)->head);
	free(*list);
	*list = NULL;
}

void pushFront(LinkedList *list, void *value) {
	Node *node = (Node *)malloc(sizeof(Node));
	if (node == NULL) {
		throwListError(MEMORY_EXHAUST, __LINE__);
	}
	node->next = list->head;
	node->value = value;
	list->head = node;
	if (list->tail == NULL) {
		list->tail = node;
	}
	list->size++;
}

int main(void) {
	printf("Run!\n");
	return 0;
}
