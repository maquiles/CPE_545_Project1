//HASH TABLE OF DOUBLY LINKED LISTS USING CHAINING
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"proj1lib.h"
#include"DLLHashTable.h"

struct HashMap *hashTable = NULL;

void insertToHash(RTOS_TMR *tmr){
  int hashIndex = tmr->RTOSTmrDly % hashTable->RTOSTmrTickCtr;
  //struct HashNode *newNode = createNode(key, data);
  //head of list for bucket with index hashIndex
  if(!hashTable[hashIndex].RTOSTmrListPtr){
    hashTable[hashIndex].RTOSTmrListPtr = tmr;
    hashTable[hashIndex].RTOSTmrListEntries = 1;
    return;
  }
  //adding new node to list
  tmr->RTOSTmrNext = (hashTable[hashIndex].RTOSTmrListPtr);
  (hashTable[hashIndex].RTOSTmrListPtr)->RTOSTmrPrev = tmr;
  //update the head of the list and the number of nodes in current bucket
  hashTable[hashIndex].RTOSTmrListPtr = tmr;
  hashTable[hashIndex].RTOSTmrListEntries++;
  return;
}

void deleteFromHash(RTOS_TMR *tmr){
  //find bucket using hash INDEX
  int hashIndex = tmr->RTOSTmrDly % hashTable->RTOSTmrTickCtr, flag = 0;
  RTOS_TMR *temp, *myNode;
  //get list head from current bucket
  myNode = hashTable[hashIndex].RTOSTmrListPtr;
  if(!myNode){
    printf("given data not found in hash table\n");
    return;
  }
  temp = myNode;
  while(myNode != NULL){
    //delete node with given key
    if(myNode->RTOSTmrDly == tmr->RTOSTmrDly){
      flag = 1;
      if(myNode == hashTable[hashIndex].RTOSTmrListPtr){
        hashTable[hashIndex].RTOSTmrListPtr = myNode->RTOSTmrNext;
        (hashTable[hashIndex].RTOSTmrListPtr)->RTOSTmrPrev = NULL;
      }
      else if(myNode->RTOSTmrNext == NULL){
        temp->RTOSTmrNext = myNode->RTOSTmrNext; //or NULL
      }
      else{
        temp->RTOSTmrNext = myNode->RTOSTmrNext;
        RTOS_TMR *temp2 = myNode->RTOSTmrNext;
        temp2->RTOSTmrPrev = temp;
      }
      hashTable[hashIndex].RTOSTmrListEntries--;
      free(myNode);
    }
    temp = myNode;
    myNode = myNode->RTOSTmrNext;
  }
  if(flag){
    printf("data deletion successful\n");
  }
  else{
    printf("given data was not found in hash tabel\n");
  }
  return;
}

//dont really need search or display functions
