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

    Node* getNth(Node *head, size_t index)
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

    void insert(Node *head, size_t index, void *value)
        Insert a node into a list. The node is inserted after(!) n-th node.
        Node counter begins from head starting from 0.

    void deleteNth(Node **head, size_t index)
        Remove a node at desired index.

    void* popNth(Node **head, size_t index)
        Remove a node at desired index and return its value.

    size_t length(const Node *head)
        Returs the length of the list.

    void** toArray(const Node* head)
        Converts linked list to array.

    void fromArray(Node **head, void **array, size_t size)
        Converts array to linked list.

    void printList(const Node *head)

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

void List_delete(Node **head) {
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

Node* getNth(Node *head, size_t index) {
    size_t counter = 0;
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

void List_pushBack(Node *head, void *value) {
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

void insert(Node *head, size_t index, void *value) {
    Node *prev, *next;
    Node *tmp = (Node*)malloc(sizeof(Node));
    tmp->value = value;

    prev = getNth(head, index);
    if (prev == NULL) {
        printf("insert: Can't insert at index %zu.\n", index);
        return;
    }
    next = prev->next;  // Can be NULL;
    prev->next = tmp;
    tmp->next = next;
}

void deleteNth(Node **head, size_t index) {
    Node *prev, *curr, *next;
    if (index == 0) {
        pop(head);
        return;
    }
    prev = getNth(*head, index-1);
    curr = prev->next;  // Can be NULL.
    if (curr == NULL) {
        printf("deleteNth: Can't delete node at index %zu.\n", index);
        return;
    }
    next = curr->next;
    prev->next = next;
    free(curr);
}

void* popNth(Node **head, size_t index) {
    void *value;
    Node *next, *prev, *curr;
    if (index == 0) {
        value = pop(head);
        return value;
    }
    prev = getNth(*head, index-1);
    curr = prev->next;  // Can be NULL.
    if (curr == NULL) {
        printf("deleteNth: Can't pop node at index %zu.\n", index);
        return NULL;
    }
    next = curr->next;
    prev->next = next;
    value = curr->value;
    free(curr);
    return value;
}

size_t length(const Node *head) {
    size_t counter = 0;
    while(head) {
        head = head->next;
        counter++;
    }
    return counter;
}

void** toArray(const Node* head) {
    void **values;
    unsigned int len = length(head);
    values = (void **)malloc(len * sizeof(void *));
    while(head) {
        values[--len] = head->value;
        head = head->next;
    }
    return values;
}

void fromArray(Node **head, void **array, size_t size) {
    size_t i = size - 1;
    if (array == NULL || size == 0) {
        printf("fromArray: Array is empty.\n");
        return;
    }
    do {
        push(head, array[i]);
    } while(i-- != 0);
}

void List_print(const Node *head) {
    size_t index = 0;
    while(head) {
        printf("Index = %zu. Value = %i.\n", index, *(int *)(head->value));
        head = head->next;
        index++;
    }
    printf("\n");
}

typedef struct Point_t {
    int x;
    int y;
} Point;

int main(void) {
    size_t i;
    int array[] = {55, 44, 33, 22, 11};
    size_t arrayLength = sizeof(array) / sizeof(array[0]);
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

    for (i=0; i<arrayLength; i++)
        push(&head, &array[i]);
    void **pointers = toArray(head);
    for (i=0; i<arrayLength; i++) {
        printf("Pointer: %p. Value: %i.\n", pointers[i], *((int *)(pointers[i])));
    }

    List_delete(&head);

    fromArray(&head, pointers, arrayLength);
    List_print(head);

    return 0;
}
