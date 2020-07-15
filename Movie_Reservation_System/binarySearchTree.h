//BinarySearchTree.h
#include <stdio.h>
#ifndef _BinarySearchTree_H
#define _BinarySearchTree_H
typedef enum { false, true } bool;
typedef struct _data {
	int cId;
	int seatId;
} Data;
typedef struct _node Node;
typedef struct _node {
	Node* left;
	Data data;
	Node* right;
} Node;
typedef struct _bTree {
	Node *root;
	int length;
} BTree;

int main(int argc, char *argv[]);
BTree Create();
int Insert(BTree *btree, Data object);
int Delete(BTree *btree, int seatId);
void Print(Node* root);
Node* Search(Node* btree, int target, int* flag2);
int compare(int a, int b);
void printLeaf(Node* n);
int countNodes(Node* n);
int SelectMax(int a, int b);
int nodesHeight(Node* n);
void ConvertArray(Node *n, int(*tempSeatArray), int(*index));
void PrintAttribute(BTree *btree);
#endif//_BinarySearchTree_H