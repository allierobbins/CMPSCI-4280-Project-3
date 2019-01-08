#include "semantics.h"

static int scope = 0;
int loc = 0;
int loopCount = 0;
int variablesOnStack = 0;
int place = 0;

token *temp;

void semantics(Node *root) {
	if(root == NULL) {
		return;
	}

	/* <program> → void <vars> <block> */
	if(root->name == "<program>") {
		printf("PROGRAM\n");
		semantics(root->child1);
		semantics(root->child2);
	}

	/* <block> → begin <vars> <stats> end */
	if(root->name == "<block>") {
		scope++;
		printf("BLOCK\n");
		semantics(root->child1);
		semantics(root->child2);
		popFromRealStack(scope, variablesOnStack);
		scope--;
		return;
	}

	/* <vars> → empty | let Identifier = integer <vars> */
	if(root->name == "<vars>") {
printf("VARS\n");
		if(!root->TK.empty()) {
			temp = root->TK.front();
			root->TK.erase(root->TK.begin());

			stack test;
			test.tokenWord = temp->tokenWord;
			test.scope = scope;
			test.lineNum = temp->line;

			if(temp->tokenID == 1001) {
				pushToRealStack(test);
				semantics(root->child1);
				variablesOnStack++;
			}
		} else {
			return;
		}
	}

	/* <expr> → <A> / <expr> | <A> * <expr> | <A> */
	if(root->name == "<expr>") {
		printf("EXPR\n");

		if(root->child2 != NULL) {
			int local = place;
			place++;

			/* <A> / <expr> */
			if(temp->tokenID == 1126) {
				semantics(root->child1);
				semantics(root->child2);
				return;

				/* <A> * <expr> */
			} else if(temp->tokenID == 1013) {
				semantics(root->child2);
				printf("STORE T%i\n", local);
				semantics(root->child1);
				printf("SUB T%i\n", local);
				return;
			}

		} else {
			semantics(root->child1);
			semantics(root->child1);
	}

	/* <A> → <M> + <A> | <M> - <A> | <M> */
	if(root->name == "<A>") {
		printf("A\n");


		if(!root->TK.empty()) {
			int local = place;
			place++;

			temp = root->TK.front();
			root->TK.erase(root->TK.begin());

			/* <M> + <A> */
			if(temp->tokenID == 1011) {
				semantics(root->child1);
				semantics(root->child2);
				return;

				/* <M> - <A> */
			} else if(temp->tokenID == 1012) {
				semantics(root->child2);
				printf("STORE T%i\n", local);
				semantics(root->child1);
				printf("SUB T%i\n", local);
				return;
			}
		} else {
			semantics(root->child1);
			return;
		}
	}

	/* <M> → - <M> | <R> */
	if(root->name == "<M>") {
		printf("M\n");

		if(root->child1 == NULL) {
			semantics(root->child2);
			return;
		} else {
			semantics(root->child1);
			printf("MULT -1\n");
			return;
		}
	}

	/* <R> → ( <expr> ) | Identifier | Number */
	if(root->name == "<R>") {
		printf("R\n");

		if(!root->TK.empty()) {
			temp = root->TK.front();
			root->TK.erase(root->TK.begin());
			stack STK;

			STK.tokenWord = temp->tokenWord;
			STK.lineNum = temp->line;
			STK.scope = scope;

			if(temp->tokenID == 1001) {
				printf("STACKR %i\n", find(STK));
				return;
			} else if(temp->tokenID == 1002) {
				printf("LOAD %s\n", STK.tokenWord.c_str());
				return;
			}
		} else {
			semantics(root->child1);
			return;
		}
	}

	/* <stats> → <stat> <mStat> */
	if(root->name == "<stats>") {
		printf("STATS\n");

		semantics(root->child1);
		semantics(root->child2);
		return;
	}

	/* <mStat> → empty | <stat> <mStat> */
	if(root->name == "<mStat>") {
		printf("MSTATS\n");

		if(root->child1 != NULL) {
			semantics(root->child1);
			semantics(root->child2);
			return;
		}
	}

	/* <stat> → <in> | <out> | <block> | <if> | <loop> | <assign> */
	if(root->name == "<stat>") {
		printf("STATS\n");

		semantics(root->child1);
		return;
	}

	/* <in> → read ( Identifier ) : */
	if(root->name == "<in>") {
		printf("IN\n");

		if(!root->TK.empty()) {
			temp = root->TK.front();
			root->TK.erase(root->TK.begin());
			stack STK;

			STK.tokenWord = temp->tokenWord;
			STK.lineNum = temp->line;
			STK.scope = scope;

			int local = place;
			place++;

			printf("READ T%i\n", local);
			printf("LOAD T%i\n", local);
			printf("STACKW %i\n", find(STK));
			return;
		}

		return;
	}

	/* <out> → print ( <expr> ) : */
	if(root->name == "<out>") {
		printf("OUT\n");

		int local = place;
		place++;

		semantics(root->child1);
		printf("STORE T%i\n", local);
		printf("WRITE T%i\n", local);
		return;
	}

	// <if> → cond ( <expr> <RO> <expr> ) <block>
	if(root->name == "<if>") {
		printf("IF\n");

		int thisLoop = ++loopCount;
		int local = place;
		place++;

		printf("\nloop%i: ", loopCount);
		/* Child 1 is <expr> */
		semantics(root->child1);
		printf("STORE T%d\n", local);
		/* Child 2 is <RO> */
		semantics(root->child2);
		/* Child 3 is <expr> */
		semantics(root->child3);
		printf("SUB T%d\n", local);
		/* Child 4 is <block> */
		semantics(root->child4);
		printf("end%d: NOOP\n\n", thisLoop);

		return;
	}

	/* <loop> → iter ( <expr> <RO> <expr> ) <stat> */
	if(root->name == "<loop>") {
		printf("LOOP\n");

		int thisLoop = ++loopCount;
		int local = place;
		place++;

		printf("\nloop%i: ", loopCount);
		/* Child 1 is <expr> */
		semantics(root->child1);
		printf("STORE T%d\n", local);
		/* Child 2 is <RO> */
		semantics(root->child2);
		/* Child 3 is <expr> */
		semantics(root->child3);
		printf("SUB T%d\n", local);
		/* Child 4 is <block> */
		semantics(root->child4);
		printf("BR loop%d\n", thisLoop);

		printf("end%d: NOOP\n\n", thisLoop);

		return;
	}

	/* <assign> → Identifier = <expr> : */
	if(root->name == "<assign>") {
		printf("ASSIGN\n");

		semantics(root->child1);

		if(!root->TK.empty()) {
			temp = root->TK.front();
			root->TK.erase(root->TK.begin());

			stack STK;
			STK.tokenWord = temp->tokenWord;
			STK.lineNum = temp->line;
			STK.scope = scope;

			printf("STACKW %i\n\n", find(STK));
			return;

		}
	}

	/* <RO> → < | < = | > | > = | = = | = */
	if(root->name == "<RO>") {
		printf("RO\n");

		if(!root->TK.empty()) {
			temp = root->TK.front();
			root->TK.erase(root->TK.begin());

			if(temp->tokenWord == "<<") {
				if(!root->TK.empty()) {
					token *temp2 = root->TK.front();
					root->TK.erase(root->TK.begin());

					if(temp2->tokenWord == "=") {
						printf("BRNEG end%d\n", loopCount);
						return;
					}
				}

				printf("BRZNEG end%d\n", loopCount);
				return;
			}

			if(temp->tokenWord == ">>") {
				if(!root->TK.empty()) {
					token *temp2 = root->TK.front();
					root->TK.erase(root->TK.begin());

					if(temp2->tokenWord == "=") {
						printf("BRPOS end%d\n", loopCount);
						return;
					}
				}

				printf("BRZPOS end%d\n", loopCount);
				return;
			}

			if(temp->tokenWord == "==") {
				printf("BRPOS end%d\n", loopCount);
				printf("BRNEG end%d\n", loopCount);
				return;
			}

			if(temp->tokenWord == "=!") {
				printf("BRZERO end%d\n", loopCount);
				return;
			}
		}
	}
}
}

void check(token *TK) {
	stack STK;
	STK.tokenWord = TK->tokenWord;
	STK.lineNum = TK->line;
	STK.scope = scope;

	int result = searchStack(STK);
	if(result > 0) {
		cout << "Variable " << STK.tokenWord << " has been previously declared on line " << result << "\n";
		exit(EXIT_FAILURE);
	} else {
		push(STK);
	}
}
