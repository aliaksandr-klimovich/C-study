/* 
	Singly linked list
	==================
	
	Represents a linked Node (node). Each node points to next node. 
	Final node points to NULL.
	
		head -> Node -> Node -> NULL
	
	where head points to the top node.
	
	
	Functions description
	---------------------
	void push(Node **head, void *value)
		Add a node to the head.
		
	void* pop(Node **head)
		Remove node from head and return its value.
		
	void deleteList(Node **head)
		Remove whole list.
	
	Node* getNth(Node *head, unsigned int n)
		Get the n-th node from list.
		Node counter strts from 0 and its first position is head.
		So getNth(&head, 0) returns head node.
		
	Node* getLast(Node *head)
		Returns last node which points to NULL.
		
	void pushBack(Node *head, void *value)
		Add a new value to the end of the list.
	
	void* popBack(Node **head)
		Remove the node from the end and return its value.
	
	Node* getLastButOne(Node *head)
	
	void insert(Node *head, unsigned int n, void *value)
		Insert a node into a list. The node is inserted after(!) n-th node.
		Node counter begins from head starting from 0.
		
	void deleteNth(Node **head, unsigned int index)
		Remove a node at desired index.
*/

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

Node* getNth(Node *head, unsigned int index) {
	unsigned int counter = 0;
	while (counter < index && head) {
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

void insert(Node *head, unsigned int index, void *value) {
	Node *prev, *next;
	Node *tmp = (Node*)malloc(sizeof(Node));
	tmp->value = value;

	prev = getNth(head, index);
	if (prev == NULL) {
		printf("insert: Can't insert at index %u.\n", index);
		return;
	}
	next = prev->next;  // Can be NULL;
	prev->next = tmp;
	tmp->next = next;
}

void deleteNth(Node **head, unsigned int index) {
	if (index == 0) {
		pop(head);
		return;
	}
	Node *prev = getNth(*head, index-1);
	Node *curr = prev->next;  // Can be NULL.
	if (curr == NULL) {
		printf("deleteNth: Can't delete node at index %u.\n", index);
		return;
	}
	Node *next = curr->next;
	prev->next = next;
	free(curr);
}

typedef struct {
	int x;
	int y;
} Point;

int main(void) {
	int i;
	int array[] = {55, 44, 33, 22, 11};
	int arrayLength = sizeof(array) / sizeof(array[0]);
	int minusOne = -1;
	Point *pPoint, point = {.x=10, .y=20};
	Node *head = NULL;

	for (i=0; i<arrayLength; i++)
		push(&head, &array[i]);

	insert(head, 0, &minusOne);
	deleteNth(&head, 2);

	for (i=0; i<arrayLength; i++)
		printf("%d\n", *(int *)(pop(&head)));

	push(&head, &point);
	pPoint = pop(&head);
	printf("(%i, %i)\n", pPoint->x, pPoint->y);

	deleteList(&head);
	return 0;
}
