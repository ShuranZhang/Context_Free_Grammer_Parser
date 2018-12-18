//  CSC 173 Project2
//	University of Rochester, Fall 2018
//	Professor George Ferguson

//  Created by Boyu Zhang on 2018/10/10.
//  Copyright © 2018 Boyu Zhang. All rights reserved.

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "Stack.c"
#include "recursive_descent_parser.h"

// for RD parser
char* currentInput;
// for table-drivem
char* current;
Stack s;

// if the paser did not work
#define FAILED NULL

// node struct
struct NODE {
    char label;
    TREE leftmostChild, rightSibling;
};

// free the whole parse tree
void freeTree(TREE tree){

    if(tree == NULL){
        return;
    }
    freeTree(tree->leftmostChild);
    freeTree(tree->rightSibling);
    free(tree);
}

/*

node

*/
TREE makeNode0(char x){
    TREE root;
    root = (TREE) malloc(sizeof(struct NODE));
    root -> label = x;
    root -> leftmostChild = NULL;
    root -> rightSibling = NULL;
    return root;
}

/*

 x
 |
 t

*/
TREE makeNode1(char x, TREE t){
    TREE root;
    root = makeNode0(x);
    root -> leftmostChild = t;
    return root;
}

/*

 x
 |	 |
 t1	t2

*/
TREE makeNode2(char x, TREE t1, TREE t2){
    TREE root;
    root = makeNode1(x, t1);
    t1 -> rightSibling = t2;
    return root;
}

/*

 x
 |	 |	|
 t1	t2	t3

*/
TREE makeNode3(char x, TREE t1, TREE t2, TREE t3){
    TREE root;
    root = makeNode1(x, t1);
    t1 -> rightSibling = t2;
    t2 -> rightSibling = t3;
    return root;
}


TREE makeNode4(char x, TREE t1, TREE t2, TREE t3, TREE t4){
    TREE root;
    root = makeNode1(x, t1);
    t1 -> rightSibling = t2;
    t2 -> rightSibling = t3;
    t3 -> rightSibling = t4;
    return root;
}

// match the terminal and move forward
bool terminalMatch(char terminal){
    if(*currentInput == terminal){
        currentInput++;
        return true;
    }
    return false;
}

// move forward and comsume on char
char consumeInput(){
    char temp = *currentInput;
    currentInput++;
    return temp;
}

// test if the input char is a digit by ASCII code
// lookhead but DO NOT comsume input
bool isDigit(char input){
    if(input >= 48 && input <= 57){
            return true;
    }
    return false;
}

// construct a tree for digit
// <D> -> 0|1|...|9
/*

<D>
 |
digit

*/
TREE D(){
    if((isDigit(*currentInput))){
        char digit = consumeInput();
    	return makeNode1('D', makeNode0(digit));
    }
    return FAILED;
}

// <NT> -> <N>|e
/*

 <NT>
  |
 <N> or e

*/
TREE NT(){

    if(isDigit(*currentInput)){

        TREE N_tree = N();

        if(N_tree == FAILED){
            return FAILED;
        }else{
            return makeNode1('n', N_tree);
        }

    }else{
        return makeNode1('n', makeNode0('e'));
    }
}

// <N> -> <D><NT>
/*

 <N>
  |		|
 <D>   <NT>

*/
TREE N(){

    TREE D_tree = D();
    TREE NT_tree;

    if(D_tree == FAILED){
        return FAILED;
    }else{

        NT_tree = NT();

        if(NT_tree == FAILED){
            freeTree(D_tree);
            return FAILED;
        }
    }
    return makeNode2('N', D_tree, NT_tree);
}

// <T> -> <F><FT>
/*

 <T>
  |     |  
 <F>   <FT>

*/
TREE T(){

    TREE F_tree = F();
    TREE FT_tree;

    if(F_tree == FAILED){
        return FAILED;
    }else{

        FT_tree = FT();

        if(FT_tree == FAILED){
            freeTree(F_tree );
            return FAILED;
        }
    }

    return makeNode2('T', F_tree , FT_tree);
}

// <FT> -> *<F><FT> | /<F><FT> | e
/*

 <FT>
  |      |      |
  *     <F>   <FT>

*/
TREE FT(){

    TREE F_tree;
    TREE FT_tree;

    if(terminalMatch('*')){

        F_tree = F();

        if(F_tree == FAILED){
            return FAILED;
        }else{

            FT_tree = FT();

            if(FT_tree == FAILED){
                freeTree(F_tree);
                return FAILED;
            }

            return makeNode3('f', makeNode0('*'), F_tree , FT_tree);
        }

    }else if(terminalMatch('/')){

        F_tree = F();

        if(F_tree == FAILED){
            return FAILED;
        }else{

            FT_tree = FT();

            if(FT_tree == FAILED){
                freeTree(F_tree);
                return FAILED;
            }
            return makeNode3('f', makeNode0('/'), F_tree , FT_tree);
        }

    }else{
        return makeNode1('f', makeNode0('e'));

    }
}

// <E> -> <T><TT>
/*

 <E>
  |      |      
 <T>   <TT>

*/
TREE E(){

    TREE T_tree,TT_tree;
    T_tree = T();

    if(T_tree == FAILED){
        return FAILED;
    }else{

        TT_tree = TT();

        if(TT_tree == FAILED){
            freeTree(T_tree);
            return FAILED;
        }

    }
    return makeNode2('E', T_tree , TT_tree );
}

// <TT> -> +<T><TT> | -<T><TT> | e
/*

 <TT>
  |      |      |
  +     <T>    <TT>         

*/
TREE TT(){

    TREE T_tree ,TT_tree;

    if(terminalMatch('+')){

        T_tree = T();

        if(T_tree == FAILED){
            return FAILED;
        }else{

            TT_tree = TT();

            if(TT_tree == FAILED){
                freeTree(T_tree);
                return FAILED;
            }
            return makeNode3('t', makeNode0('+'), T_tree , TT_tree );
        }

    }else if(terminalMatch('-')){

        T_tree =T();

        if(T_tree == FAILED){
            return FAILED;
        }else{

            TT_tree =TT();

            if(TT_tree == FAILED){
                freeTree(T_tree);
                return FAILED;
            }
            return makeNode3('t', makeNode0('-'), T_tree , TT_tree );
        }

    }else{
        return makeNode1('t', makeNode0('e'));
    }
}

// <F> -> <N> | (<E>)
/*

 <F>
  |        |     |      |
 <N>   or  (    <E>     )         

*/
TREE F(){

    TREE N_tree = N();

    if(N_tree == FAILED){

        if(!terminalMatch('(')){
            return FAILED;
        }else{

            TREE E_tree = E();

            if(E_tree == FAILED){
                return FAILED;
            }else{

                if(!terminalMatch(')')){
                    freeTree(E_tree);
                    return FAILED;
                }

                return makeNode3('F', makeNode0('('), E_tree, makeNode0(')'));
            }
        }
        
    }
    return makeNode1('F', N_tree);
}

// print a parse tree in the form Prof. Ferguson required
void printTree(TREE tree, int space){

    if(tree == NULL){
        return;
    }

    for(int i = 0; i < space; i ++){
        printf("%s", " ");
    }

    printf("%c\n", tree->label);

    printTree(tree->leftmostChild, (space + 1));
    printTree(tree->rightSibling, space);

}

// helper method: convert char to int
int toInt(char c){
    return c-'0';
}

// helper method: convert int to char
char toChar(int c){
    return c+'0';
}

// calculate result from a digit tree
Stack c_D(TREE D_tree){

	Stack s = newStack();
	push(s, D_tree->leftmostChild->label);
    return s;
}

// connect two integer to new integer
int connect(int x, int y) {
    int pow = 10;
    while(y >= pow){
        pow *= 10;
    }
    return x * pow + y;        
}

// find the digits of a number
int digit(int num){
    int digit = 1;
    while(num >= 10){
        num = num / 10;
        digit++;
    }
    return digit;
}

// result for NT_tree
Stack c_N(TREE N_tree){

    Stack s = newStack();

    // D stack
    Stack s2 = c_D(N_tree->leftmostChild);

    // N -> D
    if(N_tree->leftmostChild->rightSibling == NULL){

    	transfer(s, s2);
    	freeStack(s2);
        return s;
    }

    // N -> D NT and NT -> e
    if(N_tree->leftmostChild->rightSibling->leftmostChild->label == 'e'){

    	transfer(s, s2);
    	freeStack(s2);
        return s;
    }

    // N -> D NT and NT -> N
    // NT stack
    Stack s1 = c_N(N_tree->leftmostChild->rightSibling->leftmostChild);

    // push to s
    transfer(s, s1);
    transfer(s, s2);
    freeStack(s1);
    freeStack(s2);

    return s;
}

// explicit define function first
Stack c_F(TREE F_tree);

// result of T -> F */ F FT | F
Stack c_FT(TREE F_tree, TREE FT_tree){

	Stack s = newStack();

	// TREE of F
	Stack s2 = c_F(F_tree);

    // T -> F
    if(FT_tree == NULL){
    	
    	transfer(s, s2);
    	freeStack(s2);
        return s;
    }

    // T -> F */ F FT
    if(FT_tree->leftmostChild->label == '*'){

    	Stack s1 = c_FT(FT_tree->leftmostChild->rightSibling, 
            FT_tree->leftmostChild->rightSibling->rightSibling);

    	transfer(s, s1);
    	push(s, '*');
    	freeStack(s1);

    }else if(FT_tree->leftmostChild->label == '/'){

    	Stack s1 = c_FT(FT_tree->leftmostChild->rightSibling, 
            FT_tree->leftmostChild->rightSibling->rightSibling);

    	transfer(s, s1);
    	push(s, '/');
    	freeStack(s1);

    }else{

    	transfer(s, s2);
    	freeStack(s2);
        return s;   
    }

    transfer(s, s2);
    freeStack(s2);
    return s;
}

// result of T
Stack c_T(TREE T_tree){

	Stack new_s = newStack();

	s = c_FT(T_tree->leftmostChild, T_tree->leftmostChild->rightSibling);

    Node cursor = s->first;
    int result = 0;

    // deal with negative value
    int isNg = 0;
    if(cursor->data == '-'){
    	cursor = cursor->next;
    	pop(s);
    	isNg++;
    }

    // calculate the result with priority
    while(cursor != NULL){
        
        if(cursor->data == '*'){

            cursor = cursor->next;
            pop(s);

            // if it is negative input
            if(cursor->data == '-'){
            	cursor = cursor->next;
            	pop(s);
            	isNg++;
            }

            int temp = 0;
            while(toInt(cursor->data) >= 0 && toInt(cursor->data) <= 9){

                temp = connect(temp, toInt(cursor->data));
                cursor = cursor->next;
                pop(s);
                if(cursor == NULL){
                    break;
                }

            }

            // printf("* %d\n", temp);
            result = result * temp;

        }else if(cursor->data == '/'){

            cursor = cursor->next;
            pop(s);

            // if it is negative input
            if(cursor->data == '-'){
            	cursor = cursor->next;
            	pop(s);
            	isNg++;
            }

            int temp = 0;
            while(toInt(cursor->data) >= 0 && toInt(cursor->data) <= 9){

                temp = connect(temp, toInt(cursor->data));
                cursor = cursor->next;
                pop(s);
                if(cursor == NULL){
                    break;
                }

            }

            // printf("/ %d\n", temp);
            result = result / temp;

        }else{

            while(toInt(cursor->data) >= 0 && toInt(cursor->data) <= 9){

            	// printf("data: %c\n", cursor->data);
                result = connect(result, toInt(cursor->data));
                cursor = cursor->next;
                pop(s);
                if(cursor == NULL){
                    break;
                }

            }

            /*
            Node n = s->first;
            while(n != NULL){
            	printf("n: %c\n", n->data);
            	n = n->next;
            }
            */
            // printf("size: %d\n", size(s));
            // printf("c_T initial: %d\n", result);
        }
    }

    // push the result back to the satck
    if(result == 0){
    	push(new_s, toChar(0));
    	return new_s;
    }

    while(result > 0){
    	char c = toChar((result % 10));
    	push(new_s, c);
    	result = result / 10;
    }

    freeStack(s);

    // if is negative
    if(isNg % 2 == 1){
    	push(new_s, '-');
    }
    return new_s;
}

// result of E -> T +- T TT | T
Stack c_TT(TREE T_tree, TREE TT_tree){

    // T stack
	Stack s1 = c_T(T_tree);

	// final stack
	Stack s = newStack();

    // E -> T && TT -> e
    if(TT_tree == NULL){

    	transfer(s, s1);
    	freeStack(s1);
        return s;
    }

    // E -> T +- T TT
    if(TT_tree->leftmostChild->label == '+'){

    	// TT stack
		Stack s2 = c_TT(TT_tree->leftmostChild->rightSibling, 
        TT_tree->leftmostChild->rightSibling->rightSibling);

		transfer(s, s2);
    	push(s, '+');
    	freeStack(s2);

    }else if(TT_tree->leftmostChild->label == '-'){

    	// TT stack
		Stack s2 = c_TT(TT_tree->leftmostChild->rightSibling, 
        TT_tree->leftmostChild->rightSibling->rightSibling);

		transfer(s, s2);
    	push(s, '-');
    	freeStack(s2);

    }else{

    	transfer(s, s1);
    	freeStack(s1);
        return s; 
    }

    transfer(s, s1);
    freeStack(s1);
    return s;
}

// result of E
Stack c_E(TREE E_tree){

	Stack new_s = newStack();
	s = c_TT(E_tree->leftmostChild, E_tree->leftmostChild->rightSibling);

	// deal with negative number
	if(s->first->data == '-'){
		push(s, '0');
	}

    Node cursor = s->first;
    int result = 0;

    // calculate the result
    while(cursor != NULL){
        
        if(cursor->data == '+'){

            cursor = cursor->next;
            pop(s);

            // if it is negative input
            int isNg = 0;
            if(cursor->data == '-'){
            	cursor = cursor->next;
            	pop(s);
            	isNg++;
            }

            int temp = 0;
            while(toInt(cursor->data) >= 0 && toInt(cursor->data) <= 9){

                temp = connect(temp, toInt(cursor->data));
                cursor = cursor->next;
                pop(s);
                if(cursor == NULL){
                    break;
                }

            }

            if(isNg){
            	// printf("- %d\n", temp);
            	result = result - temp;
            }else{
            	// printf("+ %d\n", temp);
            	result = result + temp;
            }

        }else if(cursor->data == '-'){

            cursor = cursor->next;
            pop(s);

            // if it is negative input
            int isNg = 0;
            if(cursor->data == '-'){
            	cursor = cursor->next;
            	pop(s);
            	isNg++;
            }

            int temp = 0;
            while(toInt(cursor->data) >= 0 && toInt(cursor->data) <= 9){

                temp = connect(temp, toInt(cursor->data));
                cursor = cursor->next;
                pop(s);
                if(cursor == NULL){
                    break;
                }

            }

            if(isNg){
            	// printf("+ %d\n", temp);
            	result = result + temp;
            }else{
            	// printf("- %d\n", temp);
            	result = result - temp;
            }

        }else{

            while(toInt(cursor->data) >= 0 && toInt(cursor->data) <= 9){

                result = connect(result, toInt(cursor->data));
                cursor = cursor->next;
                pop(s);
                if(cursor == NULL){
                    break;
                }

            }

            // printf("c_E initial: %d\n", result);
        }
    }

    // push the result back to the satck
    if(result == 0){
    	push(new_s, toChar(0));
        freeStack(s);
    	return new_s;
    }

    if(result < 0){
    	int new_re = (-1) * result;
    	while(new_re > 0){
    		char c = toChar((new_re % 10));
    		push(new_s, c);
    		new_re = new_re / 10;
    	}

    	push(new_s, '-');
    	freeStack(s);
    	return new_s;
    }

    while(result > 0){
    	char c = toChar((result % 10));
    	push(new_s, c);
    	result = result / 10;
    }

    freeStack(s);
    return new_s;
}

// result for F tree
Stack c_F(TREE F_tree){

	Stack s = newStack();

    // F -> N
    if(F_tree->leftmostChild->label != '('){

    	Stack s1 = c_N(F_tree->leftmostChild);
    	transfer(s, s1);
    	freeStack(s1);

        return s;
    }

    // F -> (E)
    Stack s2 = c_E(F_tree->leftmostChild->rightSibling);
    transfer(s, s2);
    freeStack(s2);

    return s;
}

int CalculateTree(Stack s){

	Node cursor = s->first;

	// if it is negative
	if(cursor->data == '-'){

		cursor = cursor->next;
		pop(s);

		int result = 0;

		while(toInt(cursor->data) >= 0 && toInt(cursor->data) <= 9){

        	result = connect(result, toInt(cursor->data));
        	cursor = cursor->next;
        	pop(s);
        	if(cursor == NULL){
            	break;
        	}
    	}

    	return (-1) * result;
	}

	// if is positive
    int result = 0;

	while(toInt(cursor->data) >= 0 && toInt(cursor->data) <= 9){

        result = connect(result, toInt(cursor->data));
        cursor = cursor->next;
        pop(s);
        if(cursor == NULL){
            break;
        }
    }

    // freeStack(s);
    return result;
}

// Table-driven parser
char* productions[23] ={
    "Tt", 	// T TT
    "+Tt",	// + T TT
    "-Tt",	// - T TT
    "e",	// 	e
    "Ff",	// F FT
    "*Ff",	// * F FT
    "/Ff",	// / F FT
    "e",	// e
    "N",	// N
    "(E)",	// ( E )
    "Dn",	// D NT
    "N",	// N
    "e",	// e
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9"
};

int parseTable[8][17] = {
    //                  {0 , 1,  2,  3,  4,  5,  6,  7,  8,  9,  +,  -,  *,  /,  (,  ), /0}
    /*E*/               {0,  0, 0,   0,  0,  0,  0,  0,  0,  0, -1, -1, -1, -1,  0, -1, -1},
    /*T*/               {4,  4, 4,   4,  4,  4,  4,  4,  4,  4,  -1, -1, -1, -1,  4, -1, -1},
    /*TT*/              {-1,-1,-1,  -1, -1, -1, -1, -1, -1, -1,   1,  2, -1, -1, -1, -1,  3},
    /*F*/               {8,  8, 8,   8,  8,  8,  8,  8,  8,  8,  -1, -1, -1, -1,  9, -1, -1},
    /*FT*/              {-1,-1,-1,  -1, -1, -1, -1, -1, -1, -1,  -1, -1,  5,  6, -1, -1,  7},
    /*N*/               {10, 10,10, 10, 10, 10, 10, 10, 10, 10,  -1, -1, -1, -1, -1, -1, -1},
    /*NT*/              {11, 11,11, 11, 11, 11, 11, 11, 11, 11,  12, 12, 12, 12, 12, 12, 12},
    /*D*/               {13, 14,15, 16, 17, 18, 19, 20, 21, 22,  -1, -1, -1, -1, -1, -1, -1},
};

int sytacticcategory_to_int(char c) {
    switch (c) {
        case 'E':
            return 0;
        case 'T': // T
            return 1;
        case 't': // TT
            return 2;
        case 'F': // F
            return 3;
        case 'f': // FT
            return 4;
        case 'N':
            return 5;
        case 'n': // NT
            return 6;
        case 'D': // D
            return 7;
        default:
            return -1;
    }
}

int symbol_to_int(char c) {
    switch (c) {
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        case '+':
            return 10;
        case '-':
            return 11;
        case '*':
            return 12;
        case '/':
            return 13;
        case '(':
            return 14;
        case ')':
            return 15;
        case 'e':
            return 16;
        default :
            return -1;
    }
}

bool isTerminal(char c) {
    switch (c) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '+':
        case '-':
        case '*':
        case '/':
        case '(':
        case ')':
        case 'e':
            return true;
            break;
    }
    return false;
}

// helper method: find the length of char*
int str_length(char* c){
	int length = 0;
	while(*c != '\0'){
		length++;
		c++;
	}
	return length;
}

TREE find_production(){

    char c = pop(s);
    // printf("pop: %c\n", c);

    // if pop is a syntactic category 
    if(!isTerminal(c)){

    	// find correct production from the table
    	int production;

    	if(*current == 10){

    		// if reach the end of the input
    		production = parseTable[sytacticcategory_to_int(c)][symbol_to_int('e')];

    	}else{

    		// if not end of line
    		production = parseTable[sytacticcategory_to_int(c)][symbol_to_int(*current)];
    	}

        // if no suitable production found
        if(production == -1){
            return FAILED;
        }

        // the result production body
        char* temp = productions[production];
        // length of the production body
        int index = str_length(temp) - 1;

        // push the production to the stack
        // from right to left
        while(index >= 0){

        	// printf("push: %c\n", temp[index]);
            push(s, temp[index]);
            index--;
        }

        // build the tree from top to bottom
        if(str_length(temp) == 1){

        	return makeNode1(c, find_production());

        }else if(str_length(temp) == 2){

        	TREE left = find_production();
            TREE right = find_production();
            return makeNode2(c, left, right);

        }else if(str_length(temp) == 3){

        	TREE left = find_production();
            TREE mid = find_production();
            TREE right = find_production();
            return makeNode3(c, left, mid, right);

        }

    }

    // if pop is a terminal
    if(*current == c){
    	// if the terminals matched
    	// printf("%s\n", "current++");
    	current++;
    }
    return makeNode0(c);
}

// run the table-driven parser
TREE table_driven_parser(){

    push(s, 'E');
    return find_production();
}

int main() {

    printf("%s\n", "CSC173 Project 2");
    printf("%s\n", "BOYU ZHANG (bzhang25), SHURAN ZHANG (szhang73), ZIYI YOU (zyou5)");

    while(1){

        printf("\n%s\n", "Please choose your parser or quit (1 for RD parser, 2 for Table-driven parser, 3 to quit the program): ");

        int choice;
        scanf("%d", &choice);

        if(choice == 1){

            printf("RD parser\n");
            currentInput = (char*)malloc(200*sizeof(char));
            char* temp = currentInput;

            printf("%s\n", "Please enter your expression: ");
            scanf("%s", currentInput);
        
            // run RD paser
            TREE E_tree = E();
    
            if(*currentInput != '\0'){

                // if did not reach the end if the expression
                printf("%s %c\n", "Failed. Did NOT consume all inout with currentInput: ", *currentInput);

            }else if(E_tree == FAILED){

                // not well-formatted
                printf("Failed. Cannot be parsed.\n");

            }else{

                printf("RD parsing done!\n");

                // print the parse tree
                printTree(E_tree, 0);

                // calculate the result
                Stack result_stack = c_E(E_tree);
                printf("Expression result by RD parser: %d\n", CalculateTree(result_stack));
                freeStack(result_stack);
            }

            freeTree(E_tree);

            // UI and memory cleaning
            printf("\n");
            free(temp);

        }else if(choice == 2){

            printf("Table-driven parser\n");
            current = (char*)malloc(200*sizeof(char));
            char* temp = current;

            printf("%s\n", "Please enter your expression: ");
            scanf("%s", current);

            // test table-driven parser
            s = newStack();
            TREE E_tree_table = table_driven_parser();
            freeStack(s);

            if(*current != '\0'){

                // if did not reach the end if the expression
                printf("%s %c\n", "Failed. Did NOT consume all inout with currentInput: ", *current);

            }else if(E_tree_table == FAILED){

                // not well-formatted
                printf("Failed. Cannot be parsed.\n");

            }else{

                printf("Table-driven parsing done!\n");

                // print the parse tree
                printTree(E_tree_table, 0);

                // calculate the result
                Stack result_stack = c_E(E_tree_table);
                printf("Expression result by table-driven parser: %d\n", CalculateTree(result_stack));
                freeStack(result_stack);
            }
            freeTree(E_tree_table);

            // UI and memory cleaning
            printf("\n");
            free(temp);

        }else if (choice == 3){
            printf("%s\n", "Program terminated!");
            break;
        }else{
            printf("%s\n", "Please enter a valid input!");
            break;
        }

    }

}
 




