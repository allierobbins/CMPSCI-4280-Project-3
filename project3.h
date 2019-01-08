#ifndef PROJECT3_H
#define PROJECT3_H

#include <iostream>
#include <unistd.h>
#include <fstream>
#include "parser.h"
#include "token.h"
#include "testTree.h"
#include "node.h"
#include "semantics.h"

using namespace std;

/* Prototypes */
int fileToDataArray(string, char *);
int getFileSize(string);

#endif
