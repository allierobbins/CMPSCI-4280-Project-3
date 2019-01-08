#include "parser.h"

/* Variable Declarations */
token *TK;

/* Leading Parser of the program */
Node *parser() {
  printf("\nEntered parser.\n");

  /* Call the 1st token */
  TK = scanner();

	/* Create the ROOT of the tree */
	Node *root = NULL;
  root = program();

  /* Auxilary check for EOFTTK */
  if(TK->tokenID == 1100) {
    printf("Parser completed.\n\n");
		return root;

	} else {
		printf("PARSER ERROR : Missing expected EOF TK\n");
		exit(EXIT_FAILURE);
	}
}

/* <program> → void <vars> <block> */
Node *program() {

	/* Create the next node of the tree */
	Node *newNode = createNode("<program>");

  /* If an IDTK is found and it is "void" */
	if(TK->tokenID == 1001 && (TK->tokenWord.find("void") == 0)) {

    /* Consume the TK */
    TK = scanner();

    /* Declare the children of "program" */
		newNode->child1 = vars();
		newNode->child2 = block();

    /* Return from <program> */
		return newNode;

  } else {
		printf("PARSER ERROR : Missing expected 'void' TK \n");
		exit(EXIT_FAILURE);
	}
}

/* <block> → begin <vars> <stats> end */
Node *block() {

  /* Create the next node of the tree */
	Node *newNode = createNode("<block>");

  /* If an IDTK is found and it is "begin" */
	if(TK->tokenID == 1001 && (TK->tokenWord.find("begin") == 0)) {

    /* Consume the TK */
    TK = scanner();

    /* Declare the children of "block" */
		newNode->child1 = vars();
		newNode->child2 = stats();

    /* Consume the TK */
    TK = scanner();
    /* Return from "block" */
    return newNode;

    /* If an IDTK is found and it is "end" */
		if(TK->tokenID == 1001 && (TK->tokenWord.find("e") == 0)) {

      /* Consume the TK */
      TK = scanner();

      /* Return from "block" */
			return newNode;

		} else {
      printf("Scanner Error : Missing expected 'end' TK \n");
			exit(EXIT_FAILURE);
		}

	} else {
		printf("PARSER ERROR: Missing expected 'begin' TK \n");
		exit(EXIT_FAILURE);
	}
}

/* <vars> → empty | let Identifier = integer <vars> */
Node *vars() {

  /* Create the next node of the tree */
	Node *newNode = createNode("<vars>");

  /* If the next TK is let */
  if(TK->tokenID == 1001 && (TK->tokenWord.find("let") == 0)) {

    /* Consume the TK */
    TK = scanner();

    /* If the next TK is an identifier */
    if(TK->tokenID == 1001 && !TK->keyword) {
      newNode->TK.push_back(TK);
      /* Consume the TK */
      TK = scanner();

      newNode->child1 = vars();

      return newNode;
      }
    }

    /* Explicit return - empty */
    return newNode;
  }

/* <expr> → <A> / <expr> | <A> * <expr> | <A> */
Node *expr() {

  /* Create the next node of the tree */
	Node *newNode = createNode("<expr>");

  /* Declare the child <A> */
	newNode->child1 = A();

  /* If the next TK is a / */
	if(TK->tokenID == 1026) {
		newNode->TK.push_back(TK);
		/* Consume the TK */
    TK = scanner();

		newNode->child2 = A();
		return newNode;

  /* If the next TK is a * */
} else if(TK->tokenID == 1013) {
    newNode->TK.push_back(TK);
		/* Consume the TK */
    TK = scanner();

		newNode->child2 = A();
		return newNode;
	}

  /* Only A was called and explicit return */
	return newNode;
}

/* <A> → <M> + <A> | <M> - <A> | <M> */
Node *A() {

	/* Create the next node of the tree */
	Node *newNode = createNode("<A>");

	newNode->child1 = M();

  /* If the next TK is a + */
	if(TK->tokenID == 1011) {
		newNode->TK.push_back(TK);
		/* Consume the TK */
    TK = scanner();

		newNode->child2 = A();
		return newNode;

  /* If the next TK is a - */
} else if(TK->tokenID == 1012) {
		newNode->TK.push_back(TK);
		/* Consume the TK */
    TK = scanner();

		newNode->child2 = A();
		return newNode;
	}

  /* Only M was called and explicit return */
	return newNode;
}

/* <M> → - <M> | <R> */
Node *M() {

	/* Create the next node of the tree */
	Node *newNode = createNode("<M>");

  /* If the next TK is a - */
	if(TK->tokenID == 1012) {
		newNode->TK.push_back(TK);
		/* Consume the TK */
    TK = scanner();

    /* Declare the child <M> */
		newNode->child1 = M();

		return newNode;
	}

  /* Else declare the child <R> and continue*/
	newNode->child2 = R();

	return newNode;
}

/* <R> → ( <expr> ) | Identifier | Number */
Node *R() {

  /* Create the next node of the tree */
	Node *newNode = createNode("<R>");

  /* If the token is ( */
	if(TK->tokenID == 1017) {

    /* Consume the TK */
    TK = scanner();

    /* Declare the children of R */
		newNode->child1 = expr();

    /* If the token is ) */
		if(TK->tokenID == 1018) {

      /* Consume the TK and return */
      TK = scanner();
			return newNode;

		} else {
			printf("Scanner Error : Missing expected ) TK \n");
			exit(EXIT_FAILURE);
		}

    /* If the next TK is an ID TK */
	} else if(TK->tokenID == 1001 && !TK->keyword) {
		newNode->TK.push_back(TK);

    /* Consume the TK and return */
    TK = scanner();
		return newNode;

    /* If the next TK is a # TK */
	} else if(TK->tokenID == 1002) {
		newNode->TK.push_back(TK);

    /* Consume the TK and return */
    TK = scanner();
		return newNode;

	} else {
		printf("Scanner Error : Missing expected ID/# TK\n");
		exit(EXIT_FAILURE);
	}
}

/* <stats> → <stat> <mStat> */
Node *stats() {

  /* Create the next node of the tree */
	Node *newNode = createNode("<stats>");

  /* Declare the children of <stats> */
	newNode->child1 = stat();
	newNode->child2 = mStat();

  /* Return from <stats> */
	return newNode;
}

/* <mStat> → empty | <stat> <mStat> */
Node *mStat() {

  /* Create the next node of the tree */
	Node *newNode = createNode("<mStat>");

  /* If the token is one of the First(in/out/block/if/loop/assign) */
	if(TK->tokenID == 1001) {

      if (TK->tokenWord.find("read") == 0 ||
          TK->tokenWord.find("print") == 0 ||
          TK->tokenWord.find("begin") == 0 ||
          TK->tokenWord.find("cond") == 0 ||
          TK->tokenWord.find("iter") == 0 ||
          TK->tokenWord.find("Identifier") == 0) {

    /* Declare the children of <mStat> */
		newNode->child1 = stat();
		newNode->child2 = mStat();

    /* Return from <mStat> */
		return newNode;
	 }

    /* Explicit return (empty) */
  	return newNode;
  }

}

/* <stat> → <in> | <out> | <block> | <if> | <loop> | <assign> */
Node *stat() {

	/* Create the next node of the tree */
	Node *newNode = createNode("<stat>");

  /* If the token is "read" → the child is <in> */
	if(TK->tokenID == 1001 && (TK->tokenWord.find("read") == 0)) {
		newNode->child1 = in();

    /* Return from <stat> */
		return newNode;

  /* If the token is "print" → the child is <out> */
} else if(TK->tokenID == 1001 && (TK->tokenWord.find("print") == 0)) {
		newNode->child1 = out();

    /* Return from <stat> */
		return newNode;

  /* If the token is "begin" → the child is <block> */
} else if(TK->tokenID == 1001 && (TK->tokenWord.find("begin") == 0)) {
		newNode->child1 = block();

    /* Return from <stat> */
		return newNode;

  /* If the token is "cond" → the child is <if> */
} else if(TK->tokenID == 1001 && (TK->tokenWord.find("cond") == 0)) {
		newNode->child1 = ifStatement();

    /* Return from <stat> */
		return newNode;

  /* If the token is "iter" → the child is <loop> */
} else if(TK->tokenID == 1001 && (TK->tokenWord.find("iter") == 0)) {
		newNode->child1 = loop();

    /* Return from <stat> */
		return newNode;

  /* If the token is IDTK → the child is <assign> */
} else if (!TK->keyword) {
		newNode->child1 = assign();

    /* Return from <stat> */
		return newNode;

	} else {
    printf("PARSER ERROR: Missing expected read/print/brgin/cond/iter/Identifier TK \n");
		exit(EXIT_FAILURE);
	}
}

/* <in> → read ( Identifier ) : */
Node *in() {

  /* Create the next node of the tree */
	Node *newNode = createNode("<in>");

    /* If the token is "read" */
	if(TK->tokenID == 1001 && (TK->tokenWord.find("read") == 0)) {

    /* Consume the TK */
    TK = scanner();

    /* If the token is ( */
		if(TK->tokenID == 1017) {

			/* Consume the TK */
      TK = scanner();

      /* If the token is an ID TK */
			if(TK->tokenID == 1001 && !TK->keyword) {

				newNode->TK.push_back(TK);
				/* Consume the TK */
        TK = scanner();

        /* If the token is a ) TK */
				if(TK->tokenID == 1018) {
          /* Consume the TK */
					TK = scanner();

					return newNode;

				} else {
					printf("PARSER ERROR : Missing expected ) TK \n");
					exit(EXIT_FAILURE);
				}

			} else {
				printf("PARSER ERROR : Missing expected ID TK \n");
				exit(EXIT_FAILURE);
			}

		} else {
			printf("PARSER ERROR : Missing expected ( TK \n");
			exit(EXIT_FAILURE);
		}

	} else {
		printf("PARSER ERROR : Missing expected 'read' TK \n");
		exit(EXIT_FAILURE);
	}
}

/* <out> → print ( <expr> ) : */
Node *out() {

  /* Create the next node of the tree */
	Node *newNode = createNode("<out>");

  /* IF the next token is 'print' */
	if(TK->tokenID == 1001 && (TK->tokenWord.find("print") == 0)) {

    /* Consume the TK */
    TK = scanner();

    /* If the next token is ( */
		if(TK->tokenID == 1017) {
			//newNode->TK.push_back(TK);
      /* Consume the TK */
      TK = scanner();

      /* Declare the child <exprs> */
			newNode->child1 = expr();

      /* If the next TK is ) */
			if(TK->tokenID == 1018) {

        /* Consume the TK */
        TK = scanner();

        /* If the next TK is : */
        if(TK->tokenID == 1010) {

          /* Consume the TK */
          TK = scanner();

        } else {
      			printf("PARSER ERROR : Missing expected : TK \n");
      			exit(EXIT_FAILURE);
      		}

      } else {
  			printf("PARSER ERROR : Missing expected ) TK \n");
  			exit(EXIT_FAILURE);
  		}

		} else {
			printf("PARSER ERROR : Missing expected ( TK \n");
			exit(EXIT_FAILURE);
		}

	} else {
		printf("PARSER ERROR : Missing expected 'print' TK \n");
		exit(EXIT_FAILURE);
	}
}

// <if> → cond ( <expr> <RO> <expr> ) <block>
Node *ifStatement() {

  /* Create the next node of the tree */
	Node *newNode = createNode("<if>");

  /* If the next TK is 'cond' */
	if(TK->tokenID == 1001 && (TK->tokenWord.find("cond") == 0)) {

    /* Consume the TK */
    TK = scanner();

    /* If the next TK is ( */
		if(TK->tokenID == 1017) {

      /* Consume the TK */
      TK = scanner();

      /* Declare the children <expr> <RO> <expr> */
			newNode->child1 = expr();
			newNode->child2 = RO();
			newNode->child3 = expr();

      /* If the next TK is ) */
			if(TK->tokenID == 1018) {

        /* Consume the TK */
        TK = scanner();

        /* Declare the child <block> */
				newNode->child4 = block();

        /* Return from <if> */
				return newNode;

			} else {
				printf("PARSER ERROR : Missing expected ) TK \n");
				exit(EXIT_FAILURE);
			}

		} else {
			printf("Scanner Error : Missing expected ( TK \n");
			exit(EXIT_FAILURE);
		}

	} else {
		printf("PARSER ERROR: Missing expected 'cond' TK \n");
		exit(EXIT_FAILURE);
	}
}

/* <loop> → iter ( <expr> <RO> <expr> ) <stat> */
Node *loop() {

  /* Create the next node of the tree */
	Node *newNode = createNode("<loop>");

   /* If the next TK is "iter" */
	if(TK->tokenID == 1001 & (TK->tokenWord.find("iter") == 0)) {
    /* Consume the TK */
    TK = scanner();

    /* If the next TK is ( */
		if(TK->tokenID == 1017) {

      /* Consume the TK */
      TK = scanner();

      /* Declare the children <expr> <RO> <expr> */
			newNode->child1 = expr();
			newNode->child2 = RO();
			newNode->child3 = expr();

      /* If the next TK is ) */
			if(TK->tokenID == 1018) {
        /* Consume the TK */
        TK = scanner();

        /* Declare the child <stat>  */
				newNode->child4 = stat();

        /* Return from <loop> */
				return newNode;

			} else {
				printf("Scanner Error : Missing expected ) TK \n");
				exit(EXIT_FAILURE);
			}

		} else {
			printf("PARSER ERROR : Missing expected ( TK \n");
			exit(EXIT_FAILURE);
		}

	} else {
		printf("PARSER ERROR : Missing expected 'iter' TK \n");
		exit(EXIT_FAILURE);
	}
}

/* <assign> → Identifier = <expr> : */
Node *assign() {

  /* Create the next node of the tree */
	Node *newNode = createNode("<assign>");

  /* If the next TK is an ID TK*/
	if(TK->tokenID == 1001 && TK->keyword == 0) {

    /* Consume the TK */
		newNode->TK.push_back(TK);
    TK = scanner();

    /* If the next TK is = */
		if(TK->tokenID == 1003) {

      /* Consume the TK */
      TK = scanner();

      /* Declare the child <expr> */
      newNode->child1 = expr();

      /* If the next TK is : */
  		if(TK->tokenID == 1010) {

        /* Consume the TK */
        TK = scanner();

        /* Return from <assign> */
        return newNode;

  		} else {
  			printf("PARSER ERROR : Missing expected : TK \n");
  			exit(EXIT_FAILURE);
  		}

		} else {
			printf("PARSER ERROR : Missing expected = TK \n");
			exit(EXIT_FAILURE);
		}

	} else {
		printf("PARSER ERROR : Missing expected ID TK \n");
		exit(EXIT_FAILURE);
	}
}

/* <RO> → < | < = | > | > = | = = | = */
Node *RO() {

  /* Create the next node of the tree */
	Node *newNode = createNode("<RO>");

  /* If the next TK is < */
	if(TK->tokenID == 1005) {
		newNode->TK.push_back(TK);
    /* Consume the TK */
    TK = scanner();

    /* If the next TK is = aka <= */
		if(TK->tokenID == 1003) {
			newNode->TK.push_back(TK);
      /* Consume the TK */
      TK = scanner();

			return newNode;
		}

		return newNode;

  /* If the next TK is > */
} else if(TK->tokenID == 1007) {
		newNode->TK.push_back(TK);
    /* Consume the TK */
    TK = scanner();

    /* If the next TK is = aka >= */
		if(TK->tokenID == 1003) {
			newNode->TK.push_back(TK);
      /* Consume the TK */
      TK = scanner();

			return newNode;
		}

		return newNode;

    /* If the next TK is = */
	} else if(TK->tokenID == 1003) {
		newNode->TK.push_back(TK);
    /* Consume the TK */
    TK = scanner();

    /* If the next TK is = aka == */
    if(TK->tokenID == 1003) {
	    newNode->TK.push_back(TK);
      /* Consume the TK */
      TK = scanner();

  		return newNode;
    }

    return newNode;

	} else {
    printf("PARSER ERROR : Missing expected </< =/>/> =/= =/= TK \n");
		exit(EXIT_FAILURE);
	}
}

/* Function to create the nodes of the tree */
Node *createNode(string name) {
	Node* newNode = new Node();

  /* Default child values to NULL */
	newNode->child1 = NULL;
	newNode->child2 = NULL;
	newNode->child3 = NULL;
	newNode->child4 = NULL;
	newNode->name = name;

	return newNode;
}
