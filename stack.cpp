#include "stack.h"

stack theStack[100];
deque<stack> theRealStack;
extern int loc;

int searchRealStack(stack test) {
	int size = theRealStack.size();

	if(!theRealStack.size()) {
		return -1;
	}

	for(int c = 0; c < theRealStack.size(); c++) {
		if((theRealStack[c].tokenWord == test.tokenWord && theRealStack[c].scope == test.scope)) {
			return theRealStack.at(c).lineNum;
		}
	}
	return -1;
}

int find(stack test) {
	int size = theRealStack.size();
	stack temp;

	for(int c = 0; c < size; c++) {
		if((theRealStack[c].tokenWord == test.tokenWord) && (theRealStack[c].scope <= test.scope)) {
			return c;
		}
	}

	printf("Var %s has not been declared in this scope\n", test.tokenWord.c_str());
	exit(EXIT_FAILURE);
}

void pushToRealStack(stack test) {
	int result = searchRealStack(test);

	if(result == -1) {
		theRealStack.push_front(test);
		printf("PUSH\n");
	} else {
		printf("Var %s was previously declared on line #%i\n", test.tokenWord.c_str(), result);
		exit(EXIT_FAILURE);
	}
}

void popFromRealStack(int scope, int &vars) {
	int size = theRealStack.size();
	size--;

	for(size; size >= 0; size--) {
		if(theRealStack[size].scope == scope) {
			printf("POP\n");
			theRealStack.erase((theRealStack.begin() + size));
			vars--;
		}
	}
}

int searchStack(stack test) {
	int pos = loc;
	for(pos; pos >= 0; pos--) {
		if((theStack[pos].tokenWord == test.tokenWord) && (theStack[pos].scope == test.scope)) {
			return theStack[pos].lineNum;
		}
	}
	return 0;
}

int checkExists(stack test) {
	int pos = loc;
	for(pos; pos >= 0; pos--) {
		if((theStack[pos].tokenWord == test.tokenWord) && (theStack[pos].scope <= test.scope)) {
			return pos;
		}
	}
	return -1;
}

int checkScope(stack test) {
	int lo = checkExists(test);
	if(lo >= 0) {
		if(theStack[lo].scope > test.scope) {
			return 0;
		} else {
			return 1;
		}
	}
}

int checkExistsScope(stack test) {
	int pos = loc;
	for(int c = 0; c < pos; c++) {
		if(theStack[c].tokenWord == test.tokenWord) {
			if(theStack[c].scope <= test.scope) {
				return c;
			}
		}
	}
}

void push(stack test) {
	theStack[loc] = test;
	loc++;
}

void pop(int scope, int &vars) {
	int pos = loc;
	stack temp;
	temp.tokenWord = "";
	temp.lineNum = -1;
	temp.scope = -1;

	if(scope > 0) {
		for(pos; pos >= 0; pos--) {
			if(theStack[pos].scope == scope) {
				theStack[pos] = temp;
				printf("POP\n\n");
				loc--;
				vars--;
			}
		}
		printf("\n");
	}
}

void pop(int &vars) {
	stack temp;
	temp.tokenWord = "";
	temp.lineNum = -1;
	temp.scope = -1;

	theStack[loc] = temp;
	printf("POP\n\n");
	loc--;
	vars--;
}

int getPreviousDeclaration(stack test) {
	int pos = loc;

	for(pos; pos >= 0; pos--) {
		if(theStack[pos].tokenWord == test.tokenWord) {
			return theStack[pos].lineNum;
		}
	}

	return -1;
}

void printStack() {
	int pos = loc;
	int c = 0;

	for(pos; pos >= 0; pos--) {
		printf("%i : %s : %i\n", c, theStack[pos].tokenWord.c_str(), pos);
		c++;
	}
}

void reorganizeStack() {
	int pos = loc;
	stack temp;

	temp.tokenWord = "";
	temp.lineNum = -1;
	temp.scope = -1;
	for(pos; pos >= 0; pos--) {
		theStack[pos + 1] = theStack[pos];
		theStack[pos] = temp;
	}
}
