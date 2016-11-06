#ifndef DLLHASHTABLE_
#define DLLHASHTABLE_

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"proj1lib.h"

extern int eleCount;

struct HashNode{
  int data, key;
  struct HashNode *next, *prev;
};

struct HashMap{
  struct HashNode *head;
  int count;
};

struct HashNode * createNode(int key, int data);

void insertToHash(int key, int data);
void deleteFromHash(int key);

//dont really need search or display functions

#endif //DLLHASHTABLE_
