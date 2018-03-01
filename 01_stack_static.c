#include <stdio.h>
#include <stdlib.h>

#define STACK_MAX_SIZE 20
#define STACK_OVERFLOW  -100
#define STACK_UNDERFLOW -101

typedef int T;

typedef struct {
    T data[STACK_MAX_SIZE];
    size_t size;
} Stack_t;

void push(Stack_t *stack, const T value) {
    if (stack->size >= STACK_MAX_SIZE) {
        exit(STACK_OVERFLOW);
    }
    stack->data[stack->size] = value;
    stack->size++;
}

T pop(Stack_t *stack) {
    if (stack->size == 0) {
        exit(STACK_UNDERFLOW);
    }
    stack->size--;
    return stack->data[stack->size];
}

T peek(const Stack_t *stack) {
    if (stack->size <= 0) {
        exit(STACK_UNDERFLOW);
    }
    return stack->data[stack->size - 1];
}

void printStackValue(const T value) {
    printf("%d", value);
}

void printStack(const Stack_t *stack, void (*printStackValue)(const T)) {
    int i;
    int len = stack->size - 1;
    printf("stack %d > ", stack->size);
    for (i = 0; i < len; i++) {
        printStackValue(stack->data[i]);
        printf(" | ");
    }
    if (stack->size != 0) {
        printStackValue(stack->data[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
	Stack_t stack;
	stack.size = 0;
	size_t i;
	
	push(&stack, 3);
	printStack(&stack, printStackValue);
	
	push(&stack, 5);
	printStack(&stack, printStackValue);
	
	push(&stack, 7);
	printStack(&stack, printStackValue);

	printf("%d\n", pop(&stack));
	printStack(&stack, printStackValue);
	
	printf("%d\n", pop(&stack));
	printStack(&stack, printStackValue);
	
	printf("%d\n", pop(&stack));
	printStack(&stack, printStackValue);
/*
	// Overflow
  	for (i = 0; i < 100; i++) {
          push(&stack, i);
      }


	// Underflow
  	pop(&stack);
*/
	return 0;
}
