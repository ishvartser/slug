//
//  list.h
//  Program 4: Google Directions
//  Professor D. Hembold
//  CMPS 101: Algorithms & Abstract Data Types
//  UC Santa Cruz  
//
//  Created by Jason Heron and Igor Shvartser on 5/25/13.
//

#ifndef Program_4_list_h
#define Program_4_list_h

/*******************Data Structures**********************/
typedef struct List* ListRef;

/*****************Constructors-Destructors***************/
//Creates new list
ListRef newList(void);

//Frees up list for memory
void freeList(ListRef* pL);

/********************Access functions********************/
// returns 1 if list is empty else returns 0.
int isEmpty(ListRef L);

// returns 1 if current == NULL; else returns 0
int offEnd(ListRef L);

// returns 1 if current == first and !offEnd(); else returns 0
int atFirst(ListRef L);

// returns 1 if current == last and !offEnd(); else returns 0
int atLast(ListRef L);

// returns the first element; pre: !isEmpty()
long getFirst(ListRef L);

// returns the last element; pre: !isEmpty()
long getLast(ListRef L);

// return the current element pre: !offEnd()
long getCurrent(ListRef L);

// returns the length of the list
int getLength(ListRef L);

// return true iff the two lists have the same keys in the same order
int equals(ListRef A, ListRef B);

/*****************Manipulation procedures***********/
// make the list empty. Post: isEmpty()
void makeEmpty(ListRef L);

// set current marker to the first element in the list
// Pre: !isEmpty(); Post: !offEnd()
void moveFirst(ListRef L);

// set current marker to the last element in the list
// Pre: !isEmpty(); Post: !offEnd()
void moveLast(ListRef L);

// set current marker one step backward.
// Pre: !offEnd(); Post: offEnd() only if atFirst() was true
void movePrev(ListRef L);

// set current marker one step forward.
// Pre: !offEnd(); Post: offEnd() only if atLast() was true
void moveNext(ListRef L);

// Inserts new element before first
// Post: !isEmpty()
void insertBeforeFirst(ListRef L, long data);

// Inserts new element after last one
// Post: !isEmpty()
void insertAfterLast(ListRef L, long data);

// Inserts new element before current one
// Pre: !offEnd(); Post: !isEmpty(), !offEnd()
void insertBeforeCurrent(ListRef L, long data);

// Inserts new element after current one
// Pre: !offEnd(); Post: !isEmpty(), !offEnd()
void insertAfterCurrent(ListRef L, long data);

// delete the first element. Pre: !isEmpty()
void deleteFirst(ListRef L);

// delete the last element. Pre: !isEmpty()
void deleteLast(ListRef L);

// delete the current element.
// Pre: !isEmpty(), !offEnd(); Post: offEnd()
void deleteCurrent(ListRef L);

/************ Other operations ****************/
//prints out list in one line
void printList(FILE* out, ListRef L);

//returns a new copy of the list
ListRef copyList(ListRef L);

#endif
