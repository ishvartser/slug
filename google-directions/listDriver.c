//
//  listDriver.c
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

int main(int argc, char* argv[])
{
    ListRef L = newList();
    ListRef M;
    
    printf("(After New List) List is empty? %d \n", isEmpty(L));
    printf("(On Empty List) List is off end? %d \n", offEnd(L));
    
    insertAfterLast(L, 1);
    insertAfterLast(L, 2);
    insertAfterLast(L, 3);
    
    printf("(After insertingAfterLast 1,2,3) List L = ");
    printList(stdout, L);
    printf("\n");
    
    moveFirst(L);
    printf("(after moveFirst) Current= %ld \n", getCurrent(L));
    printf("At first? %d \n", atFirst(L));
    
    moveNext(L);
    printf("(after moveNext) Current= %ld \n", getCurrent(L));
    
    moveLast(L);
    printf("(after moveLast) Current= %ld \n", getCurrent(L));
    printf("At last? %d \n", atLast(L));
    
    movePrev(L);
    
    printf("(after movePrev) First = %ld \n", getFirst(L));
    printf("Current= %ld \n", getCurrent(L));
    printf("Last = %ld \n", getLast(L));
    
    printf("(getLength) Length = %d \n", getLength(L));
    
    M = copyList(L);
    
    printf("(after copyList) List L = ");
    printList(stdout, L);
    printf("\n");
    
    printf("List M = ");
    printList(stdout, M);
    printf("\n");
    
    printf("(Testing equals) Last L = List M? %d \n", equals(L,M));
    
    insertAfterLast(L, 4);
    
    printf("(After insertAfterLast to L) List L = ");
    printList(stdout, L);
    printf("\n");
    
    printf("List M = ");
    printList(stdout, M);
    printf("\n");
    
    makeEmpty(M);
    
    printf("(after makeEmpty(M)) List M = ");
    printList(stdout, M);
    printf("\n");
    
    insertBeforeFirst(L, 6);
    
    printf("(after insertBeforeFirst(L,6) List L = ");
    printList(stdout, L);
    printf("\n");
    
    moveLast(L);
    printf("(after moveLast) Current= %ld \n", getCurrent(L));
    
    insertBeforeCurrent(L,8);
    insertAfterCurrent(L,9);
    printf("(after insertBeforeCurrent 8, insertafterCurrent 9) List L = ");
    printList(stdout, L);
    printf("\n");
    
    deleteCurrent(L);
    
    printf("(after deleteCurrent) List L = ");
    printList(stdout, L);
    printf("\n");
    
    deleteFirst(L);
    
    printf("(after deleteFirst) List L = ");
    printList(stdout, L);
    printf("\n");
    
    deleteLast(L);
    
    printf("(after deleteLast) List L = ");
    printList(stdout, L);
    printf("\n");
    
    freeList(&L);
    freeList(&M);
	printf("freeList works!");
    /*_CrtDumpMemoryLeaks(); //memory leak test */
    
    return (EXIT_SUCCESS);
}
