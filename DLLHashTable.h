#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"proj1lib"

#ifndef DLLHASHTABLE_
#define DLLHASHTBALE_

struct HashMap *hashTable = NULL;
extern int eleCount = 0;

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

void insertToHash(int key, int data);
void deleteFromHash(int key);

//dont really need search or display functions

#endif //DLLHASHTABLE_
