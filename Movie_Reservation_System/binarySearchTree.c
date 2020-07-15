#include "binarySearchTree.h"



#if 0
int main(int argc, char *argv[]) {
	
	/*test part*/
	BTree btree;
	btree = Create();
	//test insert
	printf("===========test insert============\n");
	
	Data testData1;
	testData1.cId = 10;
	testData1.seatId = 10;
	Data testData2;
	testData2.cId = 5;
	testData2.seatId = 5;
	Data testData3;
	testData3.cId = 3;
	testData3.seatId = 3;
	Data testData4;
	testData4.cId = 13;
	testData4.seatId = 13;
	Data testData5;
	testData5.cId = 7;
	testData5.seatId = 7;
	Data testData6;
	testData6.cId = 12;
	testData6.seatId = 12;
	Data testData7;
	testData7.cId = 6;
	testData7.seatId = 6;
	Data testData8;
	testData8.cId = 4;
	testData8.seatId = 4;
	Data testData9;
	testData9.cId = 1;
	testData9.seatId = 1;
	Insert(&btree, testData1);
	Insert(&btree, testData2);
	Insert(&btree, testData3);
	Insert(&btree, testData4);
	Insert(&btree, testData5);
	Insert(&btree, testData6);
	Insert(&btree, testData7);
	Insert(&btree, testData8);
	Insert(&btree, testData9);
	Print(btree.root);
	printf("==============================\n");

	int array[30];
	int index = 0;
	ConvertArray(btree.root, &array, &index);
	for (int i = 0; i < 9; i++) {
		printf("%d ", array[i]);
	}
	//test delete
	/*
	printf("===========test insert============\n");
	int testValue;
	testValue = Delete(&btree, 10);
	Print(btree.root);
	printf("==============================\n");
	printf("===========test insert============\n");
	testValue;
	testValue = Delete(&btree, 12);
	Print(btree.root);
	printf("==============================\n");
	printf("===========test insert============\n");
	testValue;
	testValue = Delete(&btree, 5);
	Print(btree.root);
	printf("==============================\n");
	printf("===========test left after delete============\n");
	printLeaf(btree.root);
	*/

	return 0;
}
#endif
/*
create: create binaryTree
*/
BTree Create() {
	BTree btree;
	btree.root = NULL;
	btree.length = 0;

	return btree;
}

/*
Insert: insert data into binary tree
*/
int Insert(BTree* Bdummy, Data object) {
	int flag2 = 2;
	Node* tmp; /* Insert할 노드 주소 */
	Node* searchres; /* Search 함수 결과값 : 있으면 NULL, 없으면 insert할 노드의 부모 노드 주소 반환*/

	tmp = (Node*)malloc(sizeof(Node));
	tmp->data = object;  /* Insert할 노드에 데이터값 삽입 */
	/* 양쪽에 널값으로 초기화*/
	tmp->left = NULL;
	tmp->right = NULL;

	searchres = Search(Bdummy->root, tmp->data.seatId, &flag2);/* Search 함수 결과값 */
	switch (flag2) { /* Search 함수에서 설정된 flag값 : 자리가 없으면 Insert, 있으면 '자리 있음' 출력 */
	case 2:
		Bdummy->root = tmp;
		break;
	case -1:
		searchres->left = tmp;
		break;
	case 0:
		printf("이미 있는 자리입니다.");
		free(tmp);
		return 0;
	case 1:
		searchres->right = tmp;
		break;
	}
	(Bdummy->length)++; // increase number of nodes
	//PrintAttribute(Bdummy);
	return 1; /* Insert success : 1, failed : 0 */
}
Node* Search(Node* btree, int target, int* flag2) {
	int flag;

	if (!btree) /* 노드가 하나도 없을 경우 */
		return NULL;

	while (1) {
		flag = compare(target, btree->data.seatId); /* 찾는값과 노드의 key값 비교 */
		switch (flag) {
		case -1:
			*flag2 = -1;
			if (btree->left == NULL) /* insert 자리가 비었을 경우 반환*/
				return btree;
			btree = btree->left;
			break;
		case 0: /* insert할 노드가 이미 있을 경우 반환*/
			*flag2 = 0;
			return btree;
		case 1:
			*flag2 = 1;
			if (btree->right == NULL) /* insert 자리가 비었을 경우 반환*/
				return btree;
			btree = btree->right;
		}
	}
	return NULL;
}
int compare(int x, int y) {
	return ((x<y) ? -1 : (x == y) ? 0 : 1);
}


/*
Delete: delete data in binary tree
1. 지울 노드(index)와 그 부모(parentIndex)를 찾는다
2. 대체위치(sub)와 그 부모(parentSub)를 초기화 한다
3. 대체위치(sub)와 그 부모(parentSub)를 찾는다
4. 찾을 위치(index)와 대체위치(sub)가 멀리 떨어진 경우
5. 찾을 위치(index)와 대체위치(sub)가 서로 붙어있는 경우
6. 찾을 위치의 부모(parentIndex)의 관계 설정(공통)
*/
int Delete(BTree *btree, int key) {
	Node *it = NULL;
	Node *index = NULL;
	Node *parentIndex = NULL;
	Node *parentSub;
	Node *sub;
	bool isRight;

	//int rootKey; balance 조절 시에 추가할 것
	//rootKey = btree->root->data.cId;
	it = btree->root;

	while (it != NULL && it->data.seatId != key) { // search the node with key
		parentIndex = it;
		if (it->data.seatId > key) {
			it = it->left;
		}
		else {
			it = it->right;
		}
	}
	if (it == NULL)
		return 0;

	index = it;
	parentSub = index;


	if (index->right != NULL) { //delnode 오른쪽에 자식 노드 존재
		sub = index->right;
		isRight = true;
	}
	else if (index->left != NULL) { // delnode 왼쪽에 자식 노드 존재
		sub = index->left;
		isRight = false;
	}
	else { // 자식 노드 0 개
		sub = NULL;
	}

	if (sub != NULL) { // delnode 자식 노드 1 or 2개일 때
		while (isRight == true && sub->left != NULL) { //오른쪽 자식 노드(1)가 왼쪽에 또 자식 노드(2)가 있을 때
			parentSub = sub; //parentSub = 오른쪽 자식노드(1)
			sub = sub->left; //sub = 왼쪽 자식노드(2)
		}
		while (isRight == false && sub->right != NULL) {  //왼쪽 자식 노드(1)가 오른쪽에 또 자식노드(2)가 있을 때
			parentSub = sub; //parentSub = 왼쪽 자식노드(1)
			sub = sub->right; //sub = 오른쪽 자식노드(2)
		}
	}

	if (sub != NULL && parentSub != index) { //오른쪽 자식 노드(1)가 왼쪽에 또 자식 노드(2)가 있을 때 
											 //왼쪽 자식 노드(1)가 오른쪽에 또 자식노드(2)가 있을 때
		if (isRight == true) {
			parentSub->left = sub->right;
		}
		else {
			parentSub->right = sub->left;
		}

		sub->left = index->left;
		sub->right = index->right;
	}

	if (sub != NULL && parentSub == index) { //오른쪽 자식 노드(1)가 왼쪽에 또 자식 노드(2)가 없을 때 
											 //왼쪽 자식 노드(1)가 오른쪽에 또 자식노드(2)가 없을 때  
		if (isRight == true) {
			sub->left = index->left;
		}
		else {
			sub->right = index->right;
		}
	}

	if (parentIndex != NULL) {
		if (parentIndex->right == index) {
			parentIndex->right = sub;
		}
		else {
			parentIndex->left = sub;
		}
	}
	else {
		btree->root = sub;
	}

	if (index != NULL) {
		free(index);
		index = NULL;
	}
	(btree->length)--;
	//PrintAttribute(btree);
	return 1;
}

/*
Print: print data of binary tree
*/

void PrintAttribute(BTree *btree) {
	printf("=============Binary Search Tree atrribute================\n");
	printf("Root \tCID:%d SeatID: %d\n", (btree->root)->data.cId, (btree->root)->data.seatId); //root
	printf("Leaf \n");
	printLeaf(btree->root); // leaf
	printf("Height: %d\n", nodesHeight(btree->root));
	printf("Number of nodes: %d\n", btree->length);
}

void printLeaf(Node* n) {
	if (n != NULL) {
		if (n->left == NULL && n->right == NULL) {
			Print(n);
		}
		else {
			printLeaf(n->left);
			printLeaf(n->right);
		}
	}
}
int countNodes(Node* n) {
	if (n == NULL) return 0;
	return 1 + countNodes(n->left) + countNodes(n->right);
}

int SelectMax(int a, int b) {
	if (a > b) return a;
	else return b;
}

int nodesHeight(Node* n) {
	if (n == NULL) return 0;
	else return SelectMax(nodesHeight(n->left), nodesHeight(n->right)) + 1;
}

void Print(Node* node) {
	/*
	1. 노드를 입력받는다
	2. 널이 아닐때 반복한다
	2.1 프린트함수에 왼쪽노드 넣고 반복
	2.2 프린트
	2.3 프린트함수에 오른쪽노드 넣고 반복
	*/
	if (node != NULL) {
		Print(node->left);
		printf("CID:%d SeatID: %d\n", node->data.cId, node->data.seatId);
		Print(node->right);
	}
}

void ConvertArray(Node *n, int(*tempSeatArray), int *index) {
	if (n != NULL) {
		tempSeatArray[(*index)] = n->data.seatId;
		(*index)++;

		if (n->left != NULL) {
			 ConvertArray(n->left, tempSeatArray, index);
		}
		if (n->right != NULL) {
			 ConvertArray(n->right, tempSeatArray, index);
		}
	}
}
