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
} Node;

typedef struct List {
	Node *head;
	Node *tail;
	size_t size;
} List;

List* initList(void) {
	List *list = (List*)malloc(sizeof(List));
	list->head = list->tail = NULL;
	list->size = 0;
	return list;
}

void pushBack(List *list, char byte) {
	Node *node = (Node*)malloc(sizeof(Node));
	node->byte = byte;
	node->count = 1;
	node->next = NULL;
	if (list->tail) {
		list->tail->next = node;
		list->tail = node;
	} else {
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

void printList(List *list) {
	Node *node = list->head;
	printf("Entries: ");
	while(node) {
		printf("%02X(%zu) ", node->byte, node->count);
		node = node->next;
	}
	printf("\n");
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

	deleteList(&list);

	return 0;
}
