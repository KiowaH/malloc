#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define _BSD_SOURCE


struct Node {
    size_t size;
    Node *prev;
    Node *next;
};

//Holds the start of the heap
void* heapStart = sbrk(0);
//keeps track of free list initializes pointer to start of the heap
//holds current allocated block can return to user

Node *fl = NULL;
void *userMem;
int userMemSize;
//Node *FreeList = fl;

Node *tmp;

void* mmalloc(size_t size){
    if(size > 1024){
	fprintf(stdout, "Invalid size please enter a size between 0-1024");
    }
    Node *FreeList = fl;
    //Checking if free list has more nodes to check
    while(FreeList != NULL){
	//Testing for perfect match
        if(size == FreeList->size){
	    printf("Malloc: Testing Perfert match ...\n");
	    printf("Malloc: Requesting %d bytes\n", size);
	    printf("Malloc: Node at %d has enough space\n", FreeList);
	    printf("Malloc: Realeasing %d (size = %d) from free list\n", FreeList, FreeList->size);
	    if(FreeList->next == NULL && FreeList->prev == NULL){
		tmp = FreeList;
		FreeList = NULL;
		fl = FreeList;
		return tmp;
	    }
	    if(FreeList->next != NULL && FreeList->prev != NULL){
		//printf("Next and prev pointers set\n");		
		FreeList->prev->next = FreeList->next;
		FreeList->next->prev = FreeList->prev;
		FreeList->next = NULL;
	        FreeList->prev = NULL;
		tmp = FreeList; 
	        FreeList = NULL;
		return tmp;
	    }
	    //printf("Only one pointer set\n");
	    if(FreeList->next != NULL && FreeList->prev == NULL){
		//printf("Only next pointer set\n");
	        
		FreeList->next->prev = NULL;
		tmp = NULL;
		tmp = FreeList;	
		fl = FreeList->next;
		FreeList = NULL;
		return tmp;	
	    }
	    //printf("only prev pointer set");
	    if(FreeList->prev != NULL && FreeList->next == NULL){
		//printf("Only prev pointer set");
	
		FreeList->prev->next = NULL;
		tmp = NULL;
		tmp = FreeList;
		fl = FreeList->prev;
		FreeList = NULL;
		return tmp;
            } 
	}
	//testing for imperfect match in free list node
	if(size < FreeList->size){
	    printf("Testing imperfect match\n");
	    printf("Malloc: Requesting %d bytes\n", size);
	    printf("Malloc: Node at %d has enough space %d\n", FreeList, FreeList->size);
	    //split
	    userMem = (void *) (FreeList + 12);
	    userMemSize = size + 12;
	    int tempSize = (int) FreeList->size;
	    FreeList = (Node *) (tempSize - userMemSize);
	    int newSize = (int)tempSize - (int)userMemSize;
	    FreeList->size = newSize;
		

	    if(FreeList->next == NULL && FreeList->prev == NULL){
		fl = FreeList;
		return userMem;	
	    }
	    if(FreeList->next != NULL && FreeList->prev != NULL){
		FreeList->prev->next = FreeList->next;
		return userMem;
	    }else{
		if(FreeList->next != NULL && FreeList->prev == NULL){
		    FreeList->next->prev = FreeList;
		    return userMem;
		}
		if(FreeList->prev != NULL && FreeList->next == NULL){
		    FreeList->prev->next = NULL;
		    return userMem;	
		}
	    }

	}
	//if there is no block available and at end of free list
	if(size > FreeList->size && FreeList->next == NULL){
	    Node *flTemp;
	    printf("Malloc: Not enough room in the free list\n"); 
	    printf("Malloc: Requesting %d bytes\n", size);
	    void* temp = sbrk(1024);
	    printf("Malloc: Node at %d has enough space.\n", temp);   
	    userMem = temp + 12 ;
	    userMemSize = size + 12;
	    flTemp = (Node *) (temp + 12 + size);
	    flTemp->size = (1024 - userMemSize);
	    printf("Malloc: Splitting node at %d (%d) into %d (%d) and %d (%d)\n", temp, 1012, userMem, userMemSize, flTemp,flTemp->size);
	    FreeList->next = flTemp;  
	    FreeList->next->prev = FreeList; 
	    
	    break;
	}
	
    	FreeList = FreeList->next;
    }//while
    //base case if free list is empty need to allocate more memory
    if(fl == NULL){
	printf("Malloc: Requesting %d bytes.\n", size);
	//allocates next block of memory returns pointer to start of heap
    	void* hptr = sbrk(1024);
	void* currentPB = sbrk(0);
	int sizeOfHptr = (int)currentPB - (int)hptr;
	fprintf(stdout, "Malloc: Calling sbrk\n");
	userMem = (void *) hptr + 12;
        userMemSize = size + 12;
    	FreeList = (Node *) (hptr + 12 + size);
	FreeList->size = sizeOfHptr - userMemSize;
	printf("Node at %d has enough space %d.\n", hptr, sizeOfHptr);
	printf("Splitting %d (%d) into %d (%d) and %d (%d)\n", hptr, sizeOfHptr - 12, userMem, userMemSize ,FreeList, FreeList->size);
	fl = FreeList;
    }
}//mmalloc




void printFreeList(){
    printf("=== Free List ===\n");
    Node *printFL = fl;
    //node *fl = start;
    while(printFL != NULL){
	printf("Address: (%d) Next, Prev, Size(%d, %d, %d)\n", printFL, printFL->next, printFL->prev, printFL->size);
	
	printFL = printFL->next;	
    }
    printf("\n=== End ===\n");
}

void* mfree(void *){

}

int main(){
mmalloc(900);
printFreeList();
mmalloc(500);
printFreeList();
mmalloc(900);
printFreeList();
mmalloc(512);
printFreeList();
mmalloc(700);
printFreeList();
mmalloc(10);
printFreeList();
}
