/*
 * Single linked list.
 * ===================
 *
 * Implementation of the single linked list with sort.
 * This this simplified implementation of the single linked list.
 * The inner value of the node is `int`, not the pointer.
 *
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
} List;

List* List_init() {
    List *list = (List *)malloc(sizeof(List));
    list->head = NULL;
    return list;
}

void delList(List **list) {
    Node *node = NULL;
    if ((*list)->head) {
        while((*list)->head->next) {
            node = (*list)->head;
            (*list)->head = (*list)->head->next;
            free(node);
        }
        free((*list)->head);
    }
    free(*list);
    *list = NULL;
}

void push(List *list, int value) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->value = value;
    node->next = list->head;
    list->head = node;
}

int pop(List *list) {
    Node *node = NULL;
    int value;
    if (list->head == NULL) {
        printf("pop: Can't pop, list is empty.\n");
        exit(-1);
    }
    node = list->head;
    value = node->value;
    list->head = list->head->next;
    free(node);
    return value;
}

void merge(Node *a, Node *b, Node **c) {
    Node tmp;
    *c = NULL;
    if (a == NULL) {
        *c = b;
        return;
    }
    if (b == NULL) {
        *c = a;
        return;
    }
    if (a->value < b->value) {
        *c = a;
        a = a->next;
    } else {
        *c = b;
        b = b->next;
    }
    tmp.next = *c;
    while (a && b) {
        if (a->value < b->value) {
            (*c)->next = a;
            a = a->next;
        } else {
            (*c)->next = b;
            b = b->next;
        }
        (*c) = (*c)->next;
    }
    if (a) {
        while (a) {
            (*c)->next = a;
            (*c) = (*c)->next;
            a = a->next;
        }
    }
    if (b) {
        while (b) {
            (*c)->next = b;
            (*c) = (*c)->next;
            b = b->next;
        }
    }
    *c = tmp.next;
}

int main(int argc, char **argv) {
    int i;
    List *list = List_init();

    push(list, 3);
    push(list, 2);
    push(list, 1);

    for (i=1; i<=3; i++) {
        printf("%d ", pop(list));
    }
    printf("\n");

    delList(&list);
    return 0;
}
