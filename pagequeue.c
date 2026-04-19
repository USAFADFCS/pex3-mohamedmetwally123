/** pagequeue.c
 * ===========================================================
 * Name: Metwally, Mohamed A 2026
 * Section: CS483 / M4
 * Project: PEX3 - Page Replacement Simulator
 * Purpose: Implementation of the PageQueue ADT — a doubly-linked
 *          list for LRU page replacement.
 *          Head = LRU (eviction end), Tail = MRU end.
 * =========================================================== */
#include <stdio.h>
#include <stdlib.h>

#include "pagequeue.h"

/**
 * @brief Create and initialize a page queue with a given capacity
 */
PageQueue *pqInit(unsigned int maxSize) {

    // malloc a pageQueue
    PageQueue* pageQueue = (PageQueue*)malloc(sizeof(PageQueue));

    // set head and tail to NULL
    pageQueue->head = pageQueue->tail = NULL;

    //Set size to zero
    pageQueue->size = 0;

    //Set maxSize to maxSize
    pageQueue->maxSize =  maxSize;

    // Return the pointer
    return pageQueue;
}

/**
 * @brief Access a page in the queue (simulates a memory reference)
 */
long pqAccess(PageQueue *pq, unsigned long pageNum) {

    
    int found = 0; 
    int depth = -1;

    PqNode* currentNode = pq->tail;

    // Search the queue for pageNum starting from tail to head
    for(int currentPosition = 0; currentNode != NULL && !found; currentPosition++) {
        // pageNum found
        if(currentNode->pageNum == pageNum) {
            depth = currentPosition;
            found = 1;
        }
        else {
            currentNode = currentNode->prev;
        }
    }

    // HIT path (page found at depth d):
    //   - Remove the node from its current position and re-insert
    //     it at the tail (most recently used).
    //   - Return d.
    if(depth >= 0 && currentNode != pq->tail) {
       PqNode* nodeToBeReInserted = currentNode;
       
       // If node is at the head, place next node at the head
       if(currentNode == pq->head) {
            pq->head = nodeToBeReInserted->next;
            pq->head->prev = NULL;
       }
       // If it's in the middle, its prev node should point to its next node, and next node should point to prev. 
       else {
            nodeToBeReInserted->prev->next = nodeToBeReInserted->next;
            nodeToBeReInserted->next->prev = nodeToBeReInserted->prev;
       }


       //Re-insert the node at the tail
        nodeToBeReInserted->next = NULL;
        pq->tail->next = nodeToBeReInserted;
        nodeToBeReInserted->prev = pq->tail;
        pq->tail = nodeToBeReInserted;
    }

    // MISS path (page not found):
    else if(depth == -1) {
        //   - Allocate a new node for pageNum and insert it at the tail.
        PqNode* newNode = (PqNode*)malloc(sizeof(PqNode));
        newNode->pageNum = pageNum;
        newNode->next = NULL;
        //Check if queue is empty
        if(pq->size == 0) {
            pq->head = newNode;
            pq->tail = newNode;
            newNode->prev = NULL;
        }
        //Queue is not empty
        else {
            pq->tail->next = newNode;
            newNode->prev = pq->tail;
            pq->tail = newNode;
        }


        //Increase the size by 1
        pq->size++;

        //   - If size now exceeds maxSize, evict the head node (free it).
        if(pq->size > pq->maxSize) {
            PqNode* nodeToBeDeleted = pq->head;
            pq->head = pq->head->next;
            pq->head->prev = NULL;
            free(nodeToBeDeleted);
            pq->size--;
        }

    }
    return depth;
}

/**
 * @brief Free all nodes in the queue and reset it to empty
 */
void pqFree(PageQueue *pq) {
    PqNode* currentNode = pq->head;
    PqNode* nextNode;

    // TODO: Walk from head to tail, free each node.
    while (currentNode != NULL)
    {
        //Obtain a pointer to the next node
        nextNode = currentNode->next;

        free(currentNode);
        currentNode = nextNode;
    }

    // free the PageQueue struct itself.
    free(pq);


}

/**
 * @brief Print queue contents to stderr for debugging
 */
void pqPrint(PageQueue *pq) {
    // TODO (optional): Print each page number from head to tail,
    //                  marking which is head and which is tail.
    //                  Useful for desk-checking small traces.

    //Mark the head of the queue
    fprintf(stderr, "H\n");
    PqNode* currentNode = pq->head;
    while (currentNode != NULL)
    {
        fprintf(stderr, "%lu\n", currentNode->pageNum);
        currentNode = currentNode->next;
    }
    fprintf(stderr, "T\n");


}
