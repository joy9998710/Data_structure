#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>

FILE *fin;
FILE *fout;

#define FROMPARENT 0
#define FROMTHREAD 1

typedef struct ThreadedTree* ThreadedPtr;
typedef int ElementType;

struct ThreadedTree {
	int left_thread; // flag if ptr is thread
	ThreadedPtr left_child;
	ElementType data;
	ThreadedPtr right_child;
	int right_thread; // flag if ptr is thread
}ThreadedTree;

ThreadedPtr CreateTree(){
	ThreadedPtr tree = NULL;
	tree = (ThreadedPtr)malloc(sizeof(ThreadedTree));
	if(tree==NULL){
		fprintf(fout, "Out of Space!\n");
		return NULL;
	}
	tree->data = -1;
	tree->left_thread = 1;
	tree->left_child = tree;
	tree->right_thread = 1;
	tree->right_child = tree;
	return tree;
}

int Insert(ThreadedPtr root, int root_idx, ElementType data, int idx) {
	//as the parent of index idx == idx/2 find all of the parents of index idx
	ThreadedPtr newNode = NULL;
	newNode = (ThreadedPtr)malloc(sizeof(ThreadedTree));
	
	if(newNode == NULL){
		fprintf(fout, "Out of Space!\n");
		return 0;
	}
	if(idx == 1){
		root->right_child = newNode;
		root->right_thread = 0;
		newNode->left_thread = 1;
		newNode->left_child = root;
		newNode->right_thread = 1;
		newNode->right_child = root;
		newNode->data = data;
		return newNode->data;
	}

	//cnt stands for the number of calculation to make idx to 1
	int cnt = 0;
	//find array stands for memorizing the calculation
	int find[100];
	
	for(int i = 0; i < 100; i++){
		find[i] = 1;
	}

	while(idx/2){
		find[cnt] = idx;
		idx /= 2;
		cnt++;
	}

	//starting from root
	ThreadedPtr currNode = root;
	//moving currNode to the parent of idx
	int curridx = root_idx;
	int i = 0;

	//using index i's child is 2*i and 2*i + 1
	while(curridx != find[0]) {
		if(i != cnt){
			if(find[cnt-i] == 2*curridx){
				currNode = currNode->left_child;
			}
			else if(find[cnt-i] == 2*curridx + 1){
				currNode = currNode->right_child;
			}
		}
		curridx = find[cnt-i];
		i++;
	}

	//curridx is now the idx itself so we have to check whether it is odd or even number
	//if it is even number then the newNode will be placed as left_child
	if(curridx%2 == 0){
		//change left_thread of currNode as false
		currNode->left_thread = 0;
		//set newNode's left_child (with thread)
		newNode->left_child = currNode->left_child;
		newNode->left_thread = 1;
		//insert newNode as the left_child of currNode
		currNode->left_child = newNode;
		//set newNode's right_child(with thread)
		newNode->right_child = currNode;
		newNode->right_thread = 1;
		//set newNode's data
		newNode->data = data;
	}
	else if(curridx%2 == 1){
		//change right thread of currNode as false
		currNode->right_thread = 0;
		//set newNode's right_child
		newNode->right_child = currNode->right_child;
		//set newNode's right_thread as true
		newNode->right_thread = 1;
		//insert newNode as the right_child of currNode
		currNode->right_child = newNode;
		//set newNode's left_child (with thread)
		newNode->left_child = currNode;
		newNode->left_thread = 1;
		//set newNode's data
		newNode->data = data;
	}
	return newNode->data;
}

void printInorder(ThreadedPtr root){
	fprintf(fout, "inorder traversal : ");
	ThreadedPtr currNode = root->right_child;

	//Move to the very left node to start inorder traversal
	while(!currNode->left_thread){
		currNode = currNode->left_child;
	}

	while(1){
		while(currNode->right_thread){
			fprintf(fout, "%d ", currNode->data);
			currNode = currNode->right_child;		
			//if the currNode gets to root then we should break this loop
			if(currNode == root){
				return;
			}
		}

		fprintf(fout, "%d ", currNode->data);

		currNode = currNode->right_child;

		while(!currNode->left_thread){
			currNode = currNode->left_child;
		}	
	
	}
}

//free all memory that's been allocated
void DeleteTree(ThreadedPtr root){
	//Using inorder traversal
	ThreadedPtr currNode = root->right_child;
	ThreadedPtr delNode = NULL;
	//move currNode to the very left 
	while(!currNode->left_thread){
		currNode = currNode->left_child;
	}
	while(1){
		while(currNode->right_thread){
			//moving currNode to right child of currNode and delete
			delNode = currNode;
			currNode = currNode->right_child;
			free(delNode);
			if(currNode == root){
				free(currNode);
				return;
			}
		}

		delNode = currNode;
		currNode = currNode->right_child;
		free(delNode);

		while(!currNode->left_thread){
			currNode = currNode->left_child;
		}
	}
}

int main(int argc, char *argv[]){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");

	ThreadedPtr root = CreateTree();

	int NUM_NODES;
	fscanf(fin, "%d", &NUM_NODES);

	int root_idx=0, idx=0;

	while(++idx <= NUM_NODES){
		ElementType data;
		fscanf(fin, "%d", &data);
		if(Insert(root, root_idx, data, idx) == 0){
			return 0;
		}
	}
	printInorder(root);

	DeleteTree(root);
	
	
	fclose(fin);
	fclose(fout);

	return 0;
}
