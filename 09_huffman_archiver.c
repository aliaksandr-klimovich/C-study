#include <stdio.h>
#include <stdlib.h>

/*
 * File definitions
 * ================
 *
 * Functions to work with file.
 *
 */

char *FILE_NAME = "test_file_to_compress.db";
FILE *fp;

/*
 * Node definitions
 * =================
 *
 * Nodes are used to store bytes and the frequencies as they appear
 * in the input file.
 *
 */

typedef struct Node {
	char byte;
	size_t count;
	struct Node *next;
	struct Node *previous;
	struct Node *left;
	struct Node *right;
} Node;

typedef struct {
	Node *head;
	Node *tail;
	size_t size;
} List;

typedef enum {
	LIST_OVERFLOW = 1,
	LIST_UNDERFLOW
} ERROR;

static char *ERRORS[] = {
	"List overflow: memory exhaust, can't allocate memory.",
	"List underflow: can't get not existing element, possible the list is empty.",
};

void throwError(ERROR e, int line) {
	printf("[%d] %s", line, ERRORS[e]);
	exit(e);
}

List* initList(void) {
	List *list = (List*)malloc(sizeof(List));
	list->head = list->tail = NULL;
	list->size = 0;
	return list;
}

void pushBack(List *list, char byte) {
	Node *node = (Node*)malloc(sizeof(Node));
	if (node == NULL) {
		throwError(LIST_OVERFLOW, __LINE__);
	}
	node->byte = byte;
	node->count = 1;
	node->next = node->left = node->right = NULL;
	if (list->tail) {
		node->previous = list->tail;
		list->tail->next = node;
		list->tail = node;
	} else {
		node->previous = NULL;
		list->head = list->tail = node;
	}
	list->size++;
}

void deleteList(List **list) {
	Node *node = (*list)->head;
	while(node) {
		(*list)->head = (*list)->head->next;
		free(node);
		node = (*list)->head;
	}
	free(*list);
	*list = NULL;
}

void printList(List *list) {
	Node *node = list->head;
	printf("Entries: ");
	while(node) {
		printf("%02X(%u) ", node->byte, node->count);
		node = node->next;
	}
	printf("\n");
}

void add(List *list, char byte) {
	Node *node = list->head;
	if (node == NULL) {
		pushBack(list, byte);
		return;
	}
	while (node) {
		if (node->byte == byte) {
			node->count++;
			return;
		}
		node = node->next;
	}
	pushBack(list, byte);
}

void sort(List *list) {
	Node *node, *nextNode;
	size_t i = list->size, j;
	if (list->head == NULL) {
		return;
	}
	// Bubble sort
	while (--i != 0) {
		node = list->head;
		j = 0;
		while ((node && node->next) && j++<i) {
			if (node->count > node->next->count) {
				// Exchange node and next node
				nextNode = node->next;
				if (node->previous) {
					node->previous->next = nextNode;
					nextNode->previous = node->previous;
				} else {
					node->next->previous = NULL;
					list->head = nextNode;
				}
				if (nextNode->next) {
					nextNode->next->previous = node;
					node->next = nextNode->next;
				} else {
					node->next = NULL;
					list->tail = node;
				}
				node->previous = nextNode;
				nextNode->next = node;
				node = nextNode;
			}
			node = node->next;
		}
	}
}


/*
 * Main function
 * =============
 *
 * TODO add description here.
 *
 */

int main(int argc, char **argv) {
	List *list = initList();
	char c;

	fp = fopen(FILE_NAME, "r");
	while ((c = fgetc(fp)) != EOF) {
		add(list, c);
	}
	fclose(fp);

	printList(list);
	sort(list);
	printList(list);

	deleteList(&list);
	return 0;
}
