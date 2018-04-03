#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DEBUG

/*
 * File definitions
 * ================
 *
 * Functions to work with file.
 *
 */

char *FILE_NAME = "test_file_to_compress.db";
FILE *fp;

typedef enum Error_t {
    LIST_OVERFLOW = 1,
    LIST_UNDERFLOW,
    STACK_OVERFLOW,
    STACK_UNDERFLOW,
} ERROR;

static char *ERRORS[] = {
    "List overflow: memory exhaust, can't allocate memory.",
    "List underflow: can't get not existing element, "
        "possible the list is empty.",
    "Stack overflow: memory exhaust, can't allocate memory.",
    "Stack underflow: can't get not existing element, "
        "possible the list is empty.",
};

void throwError(ERROR e, int line) {
    printf("[%d] %s", line, ERRORS[e]);
    exit(e);
}

/*
 * Stack
 * =====
 *
 * Contains coded character.
 *
 */

#define STACK_INIT_SIZE 10
#define STACK_INCREMENT 10

typedef struct Stack_t {
    char *bits;
    size_t size;  // Total size allocated for this array.
    size_t length;  // Length of the array. Can be less then size.
} Stack;

Stack* Stack_init(void) {
#ifdef DEBUG
    printf("[DEBUG][FUNC_CALL] Stack_init\n");
#endif
    Stack *s = (Stack*)malloc(sizeof(Stack));
    if (s == NULL) {
        throwError(STACK_OVERFLOW, __LINE__);
    }
    s->size = STACK_INIT_SIZE;
    s->bits = malloc(s->size * sizeof(char));
    if (s->bits == NULL) {
        throwError(STACK_OVERFLOW, __LINE__);
    }
    s->length = 0;
    return s;
}

void Stack_delete(Stack **s) {
#ifdef DEBUG
    printf("[DEBUG][FUNC_CALL] Stack_delete, stack=%p\n", s);
#endif
    free((*s)->bits);
    free(*s);
    *s = NULL;
}

void Stack_resize(Stack *s) {
#ifdef DEBUG
    printf("[DEBUG][FUNC_CALL] Stack_resize, stack=%p\n", s);
#endif
    s->size += STACK_INCREMENT;
#ifdef DEBUG
    printf("[DEBUG] New size: %zu\n", s->size);
#endif
    s->bits = realloc(s->bits, s->size * sizeof(char));
    if (s->bits == NULL) {
        throwError(STACK_OVERFLOW, __LINE__);
    }
}

void Stack_push(Stack *s, char bit) {
#ifdef DEBUG
    printf("[DEBUG][FUNC_CALL] Stack_push, stack=%p, bit=%c\n", s, bit);
#endif
    if (s->length >= s->size) {
        Stack_resize(s);
    }
    s->bits[s->length] = bit;
    s->length++;
}

char Stack_pop(Stack *s) {
#ifdef DEBUG
    printf("[DEBUG][FUNC_CALL] Stack_pop, stack=%p\n", s);
#endif
    if (s->length == 0 || s->length > s->size) {
        throwError(STACK_UNDERFLOW, __LINE__);
    }
    s->length--;
    return s->bits[s->length];
}

char Stack_peek(Stack *s, size_t i) {
#ifdef DEBUG
    printf("[DEBUG][FUNC_CALL] Stack_peek, stack=%p, i=%zu\n", s, i);
#endif
    if (i < s->length) {
        return s->bits[i];
    }
    return s->bits[s->length - 1];  // ?
    // Sorry, but nothing comes in my head...
}

void Stack_implode(Stack *s) {
#ifdef DEBUG
    printf("[DEBUG][FUNC_CALL] Stack_implode, stack=%p\n", s);
#endif
    s->size = s->length;
    s->bits = realloc(s->bits, s->size * sizeof(char));
}

#ifdef DEBUG
void Stack_print(Stack *s) {
    size_t i;
    printf("[DEBUG] Stack contents: ");
    for (i = 0; i < s->length; i++) {
        printf("%c", s->bits[i]);
    }
    printf("\n");
}
#endif

/*
 * Node, List, Tree
 * ================
 *
 * List consist of Nodes, either Tree consist of Nodes.
 * Both List and Tree are the same structure - List.
 *
 */

typedef struct Node_t {
    /*
     * Node of List and (or) Tree.
     */
    char byte;
    size_t count;
    struct Node_t *next;
    struct Node_t *previous;
    struct Node_t *left;
    struct Node_t *right;
} Node;

typedef struct List_t {
    /*
     * List and tree. All in one.
     */
    Node *head;
    Node *tail;
    size_t size;
} List;

List* List_init(void) {
#ifdef DEBUG
    printf("[DEBUG][FUNC_CALL] List_init\n");
#endif
    List *list = (List*)malloc(sizeof(List));
    list->head = list->tail = NULL;
    list->size = 0;
    return list;
}

void List_pushBack(List *list, char byte) {
#ifdef DEBUG
    printf("[DEBUG][FUNC_CALL] List_pushBack, list=%p, byte=%02X\n",
            list, byte);
#endif
    Node *node = (Node*)malloc(sizeof(Node));
#ifdef DEBUG
    printf("[DEBUG] node=%p\n", node);
#endif
    if (node == NULL) {
        throwError(LIST_OVERFLOW, __LINE__);
    }
    node->byte = byte;
    node->count = 1;
    node->next = node->left = node->right = NULL;
    if (list->tail) {
#ifdef DEBUG
        printf("[DEBUG] list->tail=%p\n", list->tail);
#endif
        node->previous = list->tail;
        list->tail->next = node;
        list->tail = node;
    } else {
        node->previous = NULL;
        list->head = list->tail = node;
    }
    list->size++;
}

void List_delete(List **list) {
#ifdef DEBUG
    printf("[DEBUG][FUNC_CALL] List_delete, list=%p\n", *list);
    printf("[DEBUG] list->size=%zu\n", (*list)->size);
#endif
    Node *node = (*list)->head;
    while(node) {
        (*list)->head = (*list)->head->next;
        free(node);
        node = (*list)->head;
    }
    free(*list);
    *list = NULL;
}

#ifdef DEBUG
void List_print(List *list) {
    printf("[DEBUG][FUNC_CALL] List_print, list=%p\n", list);
    printf("[DEBUG] list->head=%p, list->tail=%p, list->size=%zu\n",
            list->head, list->tail, list->size);
    Node *node = list->head;
    printf("[DEBUG] Entries: ");
    while(node) {
        printf("[DEBUG] %02X(%zu) ", node->byte, node->count);
        node = node->next;
    }
    printf("\n");
}
#endif

void List_add(List *list, char byte) {
#ifdef DEBUG
    printf("[DEBUG][FUNC_CALL] List_add, list=%p, byte=%02X\n",
            list, byte);
#endif
    Node *node = list->head;
    if (node == NULL) {
        List_pushBack(list, byte);
        return;
    }
    while (node) {
        if (node->byte == byte) {
            node->count++;
            return;
        }
        node = node->next;
    }
    List_pushBack(list, byte);
}

void List_sort(List *list) {
#ifdef DEBUG
    printf("[DEBUG][FUNC_CALL] List_sort, list=%p\n", list);
#endif
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

void Tree_build(List *list) {
#ifdef DEBUG
    printf("[DEBUG][FUNC_CALL] Tree_build, list=%p\n", list);
#endif
    Node *newNode, *leftNode, *rightNode, *thirdNode, *node;
    if (list->head == NULL) {
        return;
    }
    while (list->size > 1) {
        // Get first two elements.
        leftNode = list->head;
        rightNode = leftNode->next;

        // Get third element.
        thirdNode = rightNode->next;

        // Create new newNode.
        newNode = (Node*)malloc(sizeof(Node));
        if (newNode == NULL) {
            throwError(LIST_OVERFLOW, __LINE__);
        }
#ifdef DEBUG
        printf("[DEBUG] newNode=%p\n", newNode);
#endif
        newNode->count = leftNode->count + rightNode->count;
        newNode->left = leftNode;
        newNode->right = rightNode;

        // Remove connections from left and right nodes.
        leftNode->next = leftNode->previous = NULL;
        rightNode->next = rightNode->previous = NULL;

        // New list head.
        list->head = thirdNode;

        if (thirdNode == NULL) {
            // If there is no third node, then the only one node is the
            // root node - `newNode`.
            list->head = list->tail = newNode;
            newNode->previous = newNode->next = NULL;
        } else {
            // Third node should have NULL as previous node.
            thirdNode->previous = NULL;

            // Insert new node in the list.
            node = list->head;
            while (node && newNode->count > node->count) {
                node = node->next;
            }
            // `node` == NULL or its count more then `newNode`.
            if (node == NULL) {
                // End of the list reached.
                // Push `newNode` back.
                list->tail->next = newNode;
                newNode->previous = list->tail;
                newNode->next = NULL;
                list->tail = newNode;
            } else {
                // `node` has greater or equal counter.
                // Insert before `node`.
                if (node->previous == NULL) {
                    // Push front.
                    newNode->previous = NULL;
                    newNode->next = node;
                    node->previous = newNode;
                    list->head = newNode;
                } else {
                    // `node->previous` is a node.
                    node->previous->next = newNode;
                    newNode->previous = node->previous;
                    node->previous = newNode;
                    newNode->next = node;
                }
            }

        }
        // Decrease list size.
        list->size--;
    }
}

void Tree_iterDelete(Node *node) {
#ifdef DEBUG
    printf("[DEBUG][FUNC_CALL] Tree_iterDelete, node=%p\n", node);
    printf("[DEBUG] node->count=%zu, node->previous=%p, "
            "node->next=%p, node->left=%p, node->right=%p\n",
            node->count, node->previous, node->next,
            node->left, node->right);
    if (node->left == NULL && node->right == NULL) {
        printf("[DEBUG] node->byte=%02X\n", node->byte);
    }
#endif
    if (node->left) {
        Tree_iterDelete(node->left);
        free(node->left);
    }
    if (node->right) {
        Tree_iterDelete(node->right);
        free(node->right);
    }
}

void Tree_delete(List **tree) {
#ifdef DEBUG
    printf("[DEBUG][FUNC_CALL] Tree_delete, tree=%p\n", *tree);
#endif
    Node *root = (*tree)->head;
    Tree_iterDelete(root);
    free(root);
    free(*tree);
    *tree = NULL;
}

#ifdef DEBUG
void Tree_iterPrint(Node *node) {
    printf("[DEBUG][FUNC_CALL] Tree_iterPrint, node=%p\n", node);
    if (node->left) {
        Tree_iterPrint(node->left);
    }
    if (node->right) {
        Tree_iterPrint(node->right);
    }
    if (node->left == NULL && node->right == NULL) {
        printf("[DEBUG] '%c' found\n", node->byte);
    }
}
#endif

void Tree_iterMakeTable(Node *node, Stack *s) {
#ifdef DEBUG
    printf("[DEBUG][FUNC_CALL] Tree_iterMakeTable, "
            "node=%p, stack=%p\n", node, s);
#endif
    if (node->left) {
        Stack_push(s, '0');
        Tree_iterMakeTable(node->left, s);
        Stack_pop(s);
    }
    if (node->right) {
        Stack_push(s, '1');
        Tree_iterMakeTable(node->right, s);
        Stack_pop(s);
    }
    if (node->left == NULL && node->right == NULL) {
#ifdef DEBUG
        printf("[DEBUG] Char '%c':\n", node->byte);
        Stack_print(s);
#endif
    }
}

void Tree_makeTable(Node *node) {
#ifdef DEBUG
    printf("[DEBUG][FUNC_CALL] Tree_makeTable, node=%p\n", node);
#endif
    Stack *s = Stack_init();
    Tree_iterMakeTable(node, s);
    Stack_delete(&s);
}

typedef struct HashTable_t {

} HashTable;

/*
 * Main function
 * =============
 *
 */

int main(int argc, char **argv) {
    List *list = List_init();
    char c;

    fp = fopen(FILE_NAME, "r");
    while ((c = fgetc(fp)) != EOF) {
        List_add(list, c);
    }
    fclose(fp);

#ifdef DEBUG
    List_print(list);
#endif
    List_sort(list);
#ifdef DEBUG
    List_print(list);
#endif

    Tree_build(list);
#ifdef DEBUG
    Tree_iterPrint(list->head);
#endif
    Tree_makeTable(list->head);
    Tree_delete(&list);

    return 0;
}
