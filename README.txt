This is the README file for Allie Robbins' Program Translation Project 3.
  I have chosen to do the 'Local' option for this project.

All files were written in C++.

List of files included in program:
  - scanner.cpp
  - scanner.h
  - main.cpp
  - token.h
  - makefile
  - README.txt
  - parser.cpp
  - parser.h
  - testTree.cpp
  - testTree.h
  - semantics.cpp
  - semantics.h
  - stack.cpp
  - stack.hS

To execute the program:
    Run command "make" to build/compile the programs.
    Run command "./statSem "fileName" to run a program after it is built.

To clean the object files:
    Run command "make clean".

Purpose of program:
    The purpose of this program is to read from an input file and identify tokens.
    Then the program will build a tree from the given tokens.
    Finally, the program will check that the tree follows the provided semantics/grammar

Outline of Project 3 by file:
  main.cpp - reads in the command line arguments
           - opens the filename from argv[1]
           - reads from the file and stores values to an array
           - launches parser()
           - launched testTree()
  parser.cpp - declares the nodes of the tree
             - gets a new TK by calling scanner()
             - following the BNF logic, the tree moves through the logic depending on the given TK
             - If an incorrect grammar is found, the program terminates and prints an error
  scanner.cpp - when called, the scanner gets the next token
              - it returns a TK value and then finishes
  testTree.cpp - function to print the parse tree in preorder fashion
  semantics.cpp - recursively loops through the tree that was built by the parser
                - when it finds the given <> tag for a node, it will check for children
                - as it does this, the semantics loop will also keep a stack for the variables
  stack.cpp - this function keeps the stack of the program
            - it utilizes pop, push, and find features
