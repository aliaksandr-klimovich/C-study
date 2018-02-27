#include <stdio.h>
#include <stdlib.h>

#define INIT_SIZE 5
#define INCREMENTER 20

#define STACK_OVERFLOW  -100
#define STACK_UNDERFLOW -101
#define OUT_OF_MEMORY   -102

typedef int T;

typedef struct {
	T *data;
	size_t size;
	size_t top;
} Stack_t;

Stack_t* createStack() {
    Stack_t *out = NULL;
    out = malloc(sizeof(Stack_t));
    if (out == NULL) {
        exit(OUT_OF_MEMORY);
    }
    out->size = INIT_SIZE;
    out->data = malloc(out->size * sizeof(T));
    if (out->data == NULL) {
        free(out);
        exit(OUT_OF_MEMORY);
    }
    out->top = 0;
    return out;
}

void deleteStack(Stack_t **stack) {
    free((*stack)->data);
    free(*stack);
    *stack = NULL;
}

void resize(Stack_t *stack) {
    stack->size += INCREMENTER;
    stack->data = realloc(stack->data, stack->size * sizeof(T));
    if (stack->data == NULL) {
        exit(STACK_OVERFLOW);
    }
}

void push(Stack_t *stack, T value) {
    if (stack->top >= stack->size) {
        resize(stack);
    }
    stack->data[stack->top] = value;
    stack->top++;
}

T pop(Stack_t *stack) {
    if (stack->top == 0) {
        exit(STACK_UNDERFLOW);
    }
    stack->top--;
    return stack->data[stack->top];
}

T peek(const Stack_t *stack) {
    if (stack->top <= 0) {
        exit(STACK_UNDERFLOW);
    }
    return stack->data[stack->top - 1];
}

void implode(Stack_t *stack) {
    stack->size = stack->top;
    stack->data = realloc(stack->data, stack->size * sizeof(T));
}

int main(int argc, char *argv[]) {
	int i;
	
	printf("Crate stack.\n");
	Stack_t *s = createStack();
	
	printf("\nFill stack .\n");
	for (i = 0; i < INIT_SIZE; i++) {
        push(s, i);
    }
    printf("Stack size: %d.\n", s->size);
    
    printf("\nPush one element to stack size grow.\n");
    push(s, i);
    printf("Stack size: %d.\n", s->size);
    printf("Top index: %d.\n", s->top);
    
    printf("\nReduce stack to minimum size.\n");
    implode(s);
    printf("Stack size: %d.\n", s->size);
    printf("Top index: %d.\n", s->top);
    
    printf("\nRead data from stack.\n");
    for (i = s->top; i != 0; i--) {
        printf("%d ", peek(s));
        printf("%d ", pop(s));
    }
	
	printf("\n\nDelete stack.\n");
	deleteStack(&s);
	return 0;
}
