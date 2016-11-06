//HASH TABLE OF DOUBLY LINKED LISTS USING CHAINING
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"proj1lib.h"
#include"DLLHashTable.h"

struct HashMap *hashTable = NULL;
int eleCount = 0;

struct HashNode{
  int data, key;
  struct HashNode *next, *prev;
};

struct HashMap{
  struct HashNode *head;
  int count;
};

struct HashNode * createNode(int key, int data){
  struct HashNode *newNode;
  newNode = (struct HashNode *)malloc(sizeof(struct HashNode));
  newNode->key = key;
  newNode->data = data;
  newNode->next = NULL;
  newNode->prev = NULL;
  return newNode;
}

void insertToHash(int key, int data){
  int hashIndex = key % eleCount; //NEED TO CHANGE THIS TO BE THE INDEX THAT I WANT
  struct HashNode *newNode = createNode(key, data);
  //head of list for bucket with index hashIndex
  if(!hashTable[hashIndex].head){
    hashTable[hashIndex].head = newNode;
    hashTable[hashIndex].count = 1;
    return;
  }
  //adding new node to list NEED TO UPDATE TO INCLUDE PREV POINTER
  newNode->next = (hashTable[hashIndex].head);
  (hashTable[hashIndex].head)->prev = newNode;
  //update the head of the list and the number of nodes in current bucket
  hashTable[hashIndex].head = newNode;
  hashTable[hashIndex].count++;
  return;
}

void deleteFromHash(int key){
  //find bucket using hash INDEX
  int hashIndex = key % eleCount, flag = 0;
  struct HashNode *temp, *myNode;
  //get list head from current bucket
  myNode = hashTable[hashIndex].head;
  if(!myNode){
    printf("given data not found in hash table\n");
    return;
  }
  temp = myNode;
  while(myNode != NULL){
    //delete node with given key
    if(myNode->key == key){
      flag = 1;
      if(myNode == hashTable[hashIndex].head){
        hashTable[hashIndex].head = myNode->next;
        myNode->next->prev = NULL;
      }
      else if(myNode->next == NULL){
        temp->next = myNode->next;
      }
      else{
        temp->next = myNode->next;
        myNode->next->prev = temp;
      }
      hashTable[hashIndex].count--;
      free(myNode);
    }
    temp = myNode;
    myNode = myNode->next;
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
