#ifndef STACK_H
#define STACK_H

#include <deque>
#include "scanner.h"
#include "node.h"

typedef struct Stack {
	string tokenWord;
	int lineNum;
	int scope;
}stack;

extern stack theStack[];
extern deque<stack> theRealStack;

void pushToRealStack(stack);
void popFromRealStack(int, int &);
int find(stack);
int searchRealStack(stack);

int checkExists(stack);
int checkExistsScope(stack);
int getPreviousDeclaration(stack);
int searchStack(stack);
void push(stack);
void pop(int, int &);
void pop(int &);
void printStack();
void reorganizeStack();

#endif
