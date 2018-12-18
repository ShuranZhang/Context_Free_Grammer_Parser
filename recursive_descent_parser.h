//  CSC 173 Project2
//	University of Rochester, Fall 2018
//	Professor George Ferguson

//  Created by Boyu Zhang on 2018/10/10.
//  Copyright © 2018 Boyu Zhang. All rights reserved.

#ifndef recursive_descent_parser_h
#define recursive_descent_parser_h
#include <stdbool.h>
#include <stdio.h>
typedef struct NODE *TREE;
extern TREE D(void);
extern TREE FT(void);
extern TREE NT(void);
extern TREE TT(void);
extern TREE E(void);
extern TREE F(void);
extern TREE T(void);
extern TREE N(void);
extern TREE makeNode0(char x);
extern TREE makeNode1(char x, TREE t);
extern TREE makeNode2(char x, TREE t1, TREE t2);
extern TREE makeNode3(char x, TREE t1, TREE t2, TREE t3);
extern TREE makeNode4(char x, TREE t1, TREE t2, TREE t3, TREE t4);
extern bool isDigit(char input);
extern bool terminalMatch(char terminal);
extern void printTree(TREE tree, int space);
#endif /* recursive_descent_parser_h */
