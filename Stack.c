//  Stack.c
//  Project2
//  int stack and char stack

//  Created by Boyu Zhang on 10/6/18.
//  Copyright © 2018 Boyu Zhang. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// char version
// stack is LIFO
typedef struct Node* Node;
struct Node {
    char data;
    Node next;
    Node previous;
};

// struct of stack
typedef struct Stack* Stack;
struct Stack {
    Node first;
    Node last;
};

// initialize new stack
Stack newStack() {
    Stack s = (Stack)malloc(sizeof(struct Stack));
    s->first = s->last = NULL;
    return s;
}

// initialize new node
Node newNode(char data) {
    Node n = (Node)malloc(sizeof(struct Node));
    n->data = data;
    n->next = n->previous = NULL;
    return n;
}

// free a stack
void freeStack(Stack s) {

    // free inside first
    Node node = s->first;
    while (node != NULL) {
        Node next = node->next;
        free(node);
        node = next;
    }
    // Free the stack itself
    free(s);
}

// push in a new element at the top
void push(Stack s, char data) {

    // new node
    Node node = newNode(data);
    node->next = s->first;

    if (s->first != NULL) {
        s->first->previous = node;
    }
    s->first = node;
    if (s->last == NULL) {
        s->last = node;
    }
}

// pop the top element
char pop(Stack s) {

        char c = s->first->data;
        Node node = s->first;

        s->first = node->next;
        free(node);
        return c;
}

// size of stack
int size(Stack s){

    Node cursor = s->first;
    int i = 0;
    while(cursor != NULL){
        cursor = cursor->next;
        i++;
    }

    return i;
}

// transfer elements in one stack to another
Stack transfer(Stack re, Stack target){

    Stack temp = newStack();

    Node cursor = target->first;
    while(cursor != NULL){
        push(temp, cursor->data);
        cursor = cursor->next;
    }

    Node cursor2 = temp->first;
    while(cursor2 != NULL){
        push(re, cursor2->data);
        cursor2 = cursor2->next;
    }

    freeStack(temp);
    return re;
}

/*
// helper method: convert char to int
int toInt(char c){
    return c-'0';
}

// helper method: convert int to char
char toChar(int c){
    return c+'0';
}

// connect two integer to new integer
int connect(int x, int y) {
    int pow = 10;
    while(y >= pow){
        pow *= 10;
    }
    return x * pow + y;        
}
*/

/*
int main(){

    
    Stack s  = newStack();
    push(s, '5');
    push(s, '1');
    push(s, '+');
    push(s, '7');
    push(s, '2');

    Stack re = newStack();
    transfer(re, s);

    Node testc = re->first;
    while(testc != NULL){
        printf("%c\n", testc->data);
        testc = testc->next;
    }

    freeStack(s);
    freeStack(re);
    */

    /*
    Node cursor = s->first;
    int result = 0;

    // calculate the result
    while(cursor != NULL){
        
        if(cursor->data == '+'){

            cursor = cursor->next;
            pop(s);

            int temp = 0;
            while(toInt(cursor->data) >= 0 && toInt(cursor->data) <= 9){

                temp = connect(temp, toInt(cursor->data));
                cursor = cursor->next;
                pop(s);
                if(cursor == NULL){
                    break;
                }

            }

            printf("+ %d\n", temp);
            result = result + temp;

        }else if(cursor->data == '-'){

            cursor = cursor->next;
            pop(s);

            int temp = 0;
            while(toInt(cursor->data) >= 0 && toInt(cursor->data) <= 9){

                temp = connect(temp, toInt(cursor->data));
                cursor = cursor->next;
                pop(s);
                if(cursor == NULL){
                    break;
                }

            }

            printf("- %d\n", temp);
            result = result - temp;

        }else{

            while(toInt(cursor->data) >= 0 && toInt(cursor->data) <= 9){

                result = connect(result, toInt(cursor->data));
                cursor = cursor->next;
                pop(s);
                if(cursor == NULL){
                    break;
                }

            }

            printf("initial: %d\n", result);
        }
    }


    while(result > 0){
        char c = toChar((result % 10));
        push(s, c);
        result = result / 10;
    }

    Node n = s->first;
    while(n != NULL){
        printf("n: %c\n", n->data);
        n = n->next;
    }
    
}
*/




