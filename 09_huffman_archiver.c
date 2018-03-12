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
 * Entry definitions
 * =================
 *
 * Entries are used to store bytes and the frequencies as they appear
 * in the input file.
 *
 */

typedef struct Entry {
	char byte;
	size_t count;
	struct Entry *next;
} Entry;

void pushEntry(Entry **head, char byte) {
	Entry *e = (Entry*)malloc(sizeof(Entry));
	e->byte = byte;
	e->count = 1;
	e->next = *head;
	*head = e;
}

void deleteAllEntries(Entry **head) {
	Entry *e = *head;
	while(*head) {
		e = *head;
		*head = (*head)->next;
		free(e);
	}
}

void addEntry(Entry **head, char byte) {
	Entry *e = *head;
	if (e == NULL) {
		pushEntry(head, byte);
		return;
	}
	while (e) {
		if (e->byte == byte) {
			e->count++;
			return;
		} else {
			e = e->next;
		}
	}
	pushEntry(head, byte);
}

void printEntries(Entry *head) {
	printf("Entries: ");
	while(head) {
		printf("%02X(%zu) ", head->byte, head->count);
		head = head->next;
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
	Entry *head = NULL;
	char c;

	fp = fopen(FILE_NAME, "r");
	while ((c = fgetc(fp)) != EOF) {
		addEntry(&head, c);
	}
	printEntries(head);



	deleteAllEntries(&head);
	fclose(fp);
	return 0;
}

