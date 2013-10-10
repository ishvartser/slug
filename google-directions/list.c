//
//  list.c
//  Program 4: Google Directions
//  Professor D. Hembold
//  CMPS 101: Algorithms & Abstract Data Types
//  UC Santa Cruz  
//
//  Created by Jason Heron and Igor Shvartser on 5/25/13.
//

#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <assert.h>


typedef struct Node {
    long data;
    struct Node* prev;
    struct Node* next;
} Node;

typedef Node* NodeRef;

typedef struct List {
    int length;
    NodeRef first;
    NodeRef last;
    NodeRef current;
} List;

NodeRef newNode(long v) {
    NodeRef n = malloc(sizeof(Node));
    assert (n != NULL);
    n->data = v;
    n->next = NULL;
    n->prev = NULL;
    return n;
}

void freeNode(NodeRef* pN) {
    if( pN!=NULL && *pN!=NULL ) {
        free(*pN);
        *pN = NULL;
    }
}

ListRef newList(void) {
    ListRef L = malloc(sizeof(List));
    assert (L != NULL);
    L->current = NULL;
    L->first = NULL;
    L->last = NULL;
    L->length = 0;
    return L;
}

/* Free the list and release the resources */
void freeList(ListRef* pL) {
    if (pL != NULL && *pL != NULL) {
        makeEmpty(*pL);
        free(*pL);
        *pL = NULL;
    }
}

/* Return true if the list is empty */
int isEmpty(ListRef L) {
    if (L==NULL) {
        printf( "Error: Called isEmpty() on NULL ListRef\n");
        //exit(EXIT_FAILURE);
    }
    return L->length == 0;
}

/* Return true if the current marker is invalid */
int offEnd(ListRef L) {
    if (L==NULL) {
        printf( "Error: Called offEnd() on NULL ListRef\n");
        //exit(EXIT_FAILURE);
    }
    return L->current == NULL;
}

/* Returns true if the current marker is at the first node */
int atFirst(ListRef L) {
    if (L==NULL) {
        printf( "Error: Called atFirst() on NULL ListRef\n");
        //exit(EXIT_FAILURE);
    } else if (isEmpty(L)) {
        printf( "Error: Called atFirst() on empty ListRef\n");
        //exit(EXIT_FAILURE);
    }
    return (L->current == L->first);
}

/* Returns true if the current marker is at the last node */
int atLast(ListRef L) {
    if (L==NULL) {
        printf( "Error: Called atLast() on NULL ListRef\n");
        //exit(EXIT_FAILURE);
    } else if (isEmpty(L)) {
        printf( "Error: Called atLast() on empty ListRef\n");
        //exit(EXIT_FAILURE);
    }
    return (L->current == L->last);
}

/* Return data of the first node */
long getFirst(ListRef L) {
    if (L==NULL) {
        printf( "Error: Called getFirst() on NULL ListRef\n");
        //exit(EXIT_FAILURE);
    } else if (isEmpty(L)) {
        printf( "Error: Called getFirst() on empty ListRef\n");
        //exit(EXIT_FAILURE);
    }
    return L->first->data;
}

/* Return data of the last node */
long getLast(ListRef L) {
    if (L==NULL) {
        printf( "Error: Called getLast() on NULL ListRef\n");
        //exit(EXIT_FAILURE);
    } else if (isEmpty(L)) {
        printf( "Error: Called getLast() on empty ListRef\n");
        //exit(EXIT_FAILURE);
    }
    return L->last->data;
}

/* Return data of the current node */
long getCurrent(ListRef L) {
    if (L==NULL) {
        printf( "Error: Called getCurrent() on NULL ListRef\n");
        //exit(EXIT_FAILURE);
    } else if (isEmpty(L)) {
        printf( "Error: Called getCurrent() on empty ListRef\n");
        //exit(EXIT_FAILURE);
    } else if (offEnd(L)) {
        printf( "Error: Called getCurrent() on ListRef without current element\n");
        //exit(EXIT_FAILURE);
    }
    return L->current->data;
}

/* return the number of nodes in the list */
int getLength(ListRef L) {
    if (L==NULL) {
        printf( "Error: Called getLength() on NULL ListRef\n");
        //exit(EXIT_FAILURE);
    }
    return (L->length);
}

/* check if the arguments lists are equal.
 * the length is equal and they have same elements in same order
 */
int equals(ListRef A, ListRef B) {
    
    if (A==NULL) {
        printf( "Error: Called equals() with NULL ListRef\n");
        //exit(EXIT_FAILURE);
    } else if (B==NULL) {
        printf( "Error: Called equals() with NULL ListRef\n");
        //exit(EXIT_FAILURE);
    }
    
    if (A->length != B->length) {
        return 0;
    } else {
        NodeRef n = A->first;
        NodeRef m = B->first;
        while (n != NULL) {
            if (n->data != m->data) {
                return 0;
            }
            n = n->next;
            m = m->next;
        }
        return 1;
    }
}

/* empty the list, free up the nodes */
void makeEmpty(ListRef L) {
    if (L == NULL) {
        printf( "Error: Called makeEmpty() on NULL ListRef\n");
        //exit(EXIT_FAILURE);
    }
    while (!isEmpty(L)) {
        deleteFirst(L);
    }
}

/* Set current to the first node */
void moveFirst(ListRef L) {
    if (L == NULL) {
        printf( "Error: Called moveFirst() on NULL ListRef\n");
        //exit(EXIT_FAILURE);
    } else if (isEmpty(L)) {
        //printf( "Error: Called moveFirst() on empty ListRef\n");
        //exit(EXIT_FAILURE);
    }
    L->current = L->first;
}

/* set current to the last node */
void moveLast(ListRef L) {
    if (L == NULL) { printf( "Error: Called moveLast() on NULL ListRef\n");
        //exit(EXIT_FAILURE);
    } else if (isEmpty(L)) {
        printf( "Error: Called moveLast() on empty ListRef\n");
        //exit(EXIT_FAILURE);
    }
    L->current = L->last;
}

/* move current to the prev node */
void movePrev(ListRef L) {
    if (L == NULL) {
        printf( "Error: Called movePrev() on NULL ListRef\n");
        //exit(EXIT_FAILURE);
    } else if (isEmpty(L)) {
        printf( "Error: Called movePrev() on empty ListRef\n");
        //exit(EXIT_FAILURE);
    } else if (offEnd(L)) {
        printf( "Error: Called movePrev() on ListRef without current\n");
        //exit(EXIT_FAILURE);
    } else {
        L->current = L->current->prev;
    }
}

/* Move the current ptr to the next node */
void moveNext(ListRef L) {
    if (L == NULL) {
        printf( "Error: Called moveNext() on NULL ListRef\n");
        //exit(EXIT_FAILURE);
    } else if (isEmpty(L)) {
        printf( "Error: Called moveNext() on empty ListRef\n");
        //exit(EXIT_FAILURE);
    } else if (offEnd(L)) {
        printf( "Error: Called moveNext() on ListRef without current\n");
        //exit(EXIT_FAILURE);
    } else {
        L->current = L->current->next;
    }
}

/* Insert before the current ptr */
void insertBeforeFirst(ListRef L, long data) {
    NodeRef n;
    if (L == NULL) {
        printf( "Error: Called insertBeforeFirst() on NULL ListRef\n");
        //exit(EXIT_FAILURE);
    }
    if (isEmpty(L)) {
        L->first = newNode(data);
        L->last = L->first;
    } else {
        n = newNode(data);
        L->first->prev = n;
        n->next = L->first;
        L->first = n;
    }
    L->length++;
}

/* insert a node after the last node
 */
void insertAfterLast(ListRef L, long data) {
    NodeRef n;
    if (L == NULL) { printf( "Error: Called insertAfterLast() on NULL ListRef\n");
        //exit(EXIT_FAILURE);
    }
    if (isEmpty(L)) {
        L->first = newNode(data);
        L->last = L->first;
    } else {
        n = newNode(data);
        L->last->next = n;
        n->prev = L->last;
        L->last = n;
    }
    L->length++;
}

/*
 * insert before the current node, if defined
 */
void insertBeforeCurrent(ListRef L, long data)
{
    NodeRef n;
    if (L == NULL) {
        printf( "Error: Called insertBeforeCurrent() on NULL ListRef\n");
        //exit(EXIT_FAILURE);
    } else if (isEmpty(L)) {
        printf( "Error: Called insertBeforeCurrent() on empty ListRef\n");
        //exit(EXIT_FAILURE);
    } else if (offEnd(L)) {
        printf("Error: Called insertBeforeCurrent() with offEnd() == true\n");
        //exit(EXIT_FAILURE);
    }
    if (atFirst(L)) {
        insertBeforeFirst(L, data);
    } else {
        n = newNode(data);
        n->next = L->current;
        n->prev = L->current->prev;
        n->prev->next = n;
        n->next->prev = n;
        L->length++;
    }
}

/*
 * insert after the current node, if defined
 */
void insertAfterCurrent(ListRef L, long data)
{
    NodeRef n;
    if (L == NULL) {
        printf( "Error: Called insertAfterCurrent() on NULL ListRef\n");
        //exit(EXIT_FAILURE);
    } else if (isEmpty(L)) {
        printf( "Error: Called insertAfterCurrent() on empty ListRef\n");
        //exit(EXIT_FAILURE);
    }
    
    if (atLast(L)) {
        insertAfterLast(L, data);
    } else {
        n = newNode(data);
        n->next = L->current->next;
        n->prev = L->current;
        L->current->next = n;
        n->next->prev = n;
        L->length++;
    }
}

/*
 * delete the first element
 * if the current marker is at first node, then unset the current marker.
 */
void deleteFirst(ListRef L) {
    NodeRef n;
    
    if (L == NULL) {
        printf( "Error: Called deleteFirst() on NULL ListRef\n");
        //exit(EXIT_FAILURE);
    } else if (isEmpty(L)) {
        printf( "Error: Called deleteFirst() on empty ListRef\n");
        //exit(EXIT_FAILURE);
    }
    if ( L->first == L->current ) {
        L->current = NULL;
    }
    if (L->length == 1) {
        freeNode(&(L->first));
        L->first = NULL;
        L->last = NULL;
    } else {
        n = L->first;
        L->first->next->prev = NULL;
        L->first = L->first->next;
        n->next = NULL;
        freeNode(&n);
        n = NULL;
    }
    L->length--;
}

void deleteLast(ListRef L) {
    NodeRef n;
    
    if (L == NULL) {
        printf( "Error: Called deleteLast() on NULL ListRef\n");
        //exit(EXIT_FAILURE);
    } else if (isEmpty(L)) {
        printf( "Error: Called deleteLast() on empty ListRef\n");
        //exit(EXIT_FAILURE);
    }
    if ( L->first == L->current ) {
        L->current = NULL;
    }
    if (L->length == 1) {
        freeNode(&(L->last));
        L->last = NULL;
        L->first = NULL;
    } else {
        n = L->last;
        L->last = n->prev;
        n->prev->next = NULL;
        n->prev = NULL;
        freeNode(&n);
        n = NULL;
    }
    L->length--;
}

void deleteCurrent(ListRef L) {
    if (L == NULL) {
        printf( "Error: Called deleteCurrent() on NULL ListRef\n");
        //exit(EXIT_FAILURE);
    } else if (isEmpty(L)) {
        printf( "Error: Called deleteCurrent() on empty ListRef\n");
        //exit(EXIT_FAILURE);
    } else if (offEnd(L)) {
        printf( "Error: Called deleteCurrent() on ListRef without current \n");
        //exit(EXIT_FAILURE);
    }
    
    if (L->current == L->first) {
        deleteFirst(L);
    } else if(L->current == L->last) {
        deleteLast(L);
    } else {
        if (L->current->next != NULL) {
            L->current->next->prev = L->current->prev;
        }
        if (L->current->prev != NULL) {
            L->current->prev->next = L->current->next;
        }
        L->current->next = NULL;
        L->current->prev =NULL;
        freeNode(&(L->current));
        L->length--;
    }
    L->current = NULL;
}

void printList(FILE* out, ListRef L) {
    NodeRef n = L->first;
    
    if (L == NULL) {
        printf( "Error: Called printList() on NULL ListRef\n");
        //exit(EXIT_FAILURE);
    }
    
    while (n != NULL) { 
        fprintf(out, "%ld ", n->data);
        n = n->next;
    }
    fprintf(out, "\n");
}

ListRef copyList(ListRef L) {
    ListRef nlist = newList();
    NodeRef n = L->first;
    
    while (n != NULL) {
        insertAfterLast(nlist, n->data);
        n = n->next;
    }
    return (nlist);
}
