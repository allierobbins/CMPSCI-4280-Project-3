#include "testTree.h"

void testTree(Node *node, int level) {

  /* If the tree is empty, return */
	if(node == NULL) {
		return;
	}

  /* For the level of the tree, enter the correct # of indents */
	string line;
	token *TK;
	for(int i = 0; i < level; i++) {
		line.append("  ");
	}

  /* Create the line using the node values */
	line.append(node->name.c_str());
	line.append(" ");
	for(int i = 0; i < node->TK.size(); i++) {
		TK = node->TK[i];
		line.append(TK->tokenType);
		line.append("  ");
	}

  /* Print the line value */
	printf("%s\n", line.c_str());


  /* Call the children in order */
	testTree(node->child1, level + 1);
	testTree(node->child2, level + 1);
	testTree(node->child3, level + 1);
	testTree(node->child4, level + 1);
}
