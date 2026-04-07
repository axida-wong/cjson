#include<stdio.h>
#include "stack.h"

int main(void)
{
	int size = 5;
	Stack s;
	stack_init(&s, size);
	printf("%d, empty now\n", stack_is_empty(&s));
	for(int i = 0; i < 5; i++)
		push(&s, i);
	printf("%d, full now\n", stack_is_full(&s));
	for(int i = 0; i < 5; i++)
		printf("%d\n", pop(&s));
	printf("%d, empty now\n", stack_is_empty(&s));
	
	return 0;
}
