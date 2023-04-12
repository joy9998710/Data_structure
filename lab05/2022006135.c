#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BST* Tree;
typedef struct BST{
	int value;
	struct BST* left;
	struct BST* right;
}BST;

int findNode(Tree root, int key){
	Tree currNode = root;
	//recursively find the key
	//if currNode == NULL it means that the function did not find the key
	if(currNode == NULL){
		return 0;
	}
	//if currNode->value == key then it means that the function have found the key
	if(currNode->value == key){
		return 1;
	}
	//if key is smaller than currNode's value we find from currNode->left
	else if(currNode->value > key){
		return findNode(currNode->left, key);
	}
	//if key is bigger than currNode's value we find from currNode->right
	else if(currNode->value < key){
		return findNode(currNode->right, key);
	}
}

Tree insertNode(Tree root, int key){
	Tree currNode = root;
	//recursively inserting node
	if(currNode == NULL){
		currNode = (Tree)malloc(sizeof(BST));
		currNode->left = NULL;
		currNode->right = NULL;
		currNode->value = key;
		fprintf(fout, "insert %d\n", key);
	}
	else if(currNode->value < key){
		currNode->right = insertNode(currNode->right, key);
	}
	else if(currNode->value > key){
		currNode->left = insertNode(currNode->left, key);
	}
	else if(currNode->value == key){
		fprintf(fout, "insertion error: %d is already in the tree\n", key);
		return currNode;
	}
	return currNode;
}

Tree deleteNode(Tree root, int key){
	Tree delNode = root;
	//if we can not find the key value just return the original delNode
	if(delNode == NULL){
		return delNode;
	}
	//finding key value recursively
	else if(delNode->value < key){
		delNode->right = deleteNode(delNode->right, key);
	}
	else if(delNode->value > key){
		delNode->left = deleteNode(delNode->left, key);
	}
	//when found there are three cases
	else{
		//Case 1) No child
		if(delNode->left == NULL && delNode->right == NULL){
			Tree tempNode = NULL;
			free(delNode);
			return tempNode;
		}
		//Case 2) 1 child
		//Case 2-1) if only right child exist
		else if(delNode->left == NULL){
			Tree currNode = delNode;
			currNode = currNode->right;
			while(currNode->left != NULL){
				currNode = currNode->left;
			}
			//we don't delete delNode, we delete currNode so just replace the value
			delNode->value = currNode->value;
			delNode->right = deleteNode(delNode->right, currNode->value);
		}
		//Case 2-2) if only left child exist
		else if(delNode->right == NULL){
			Tree currNode = delNode;
			currNode = currNode->left;
			while(currNode->right != NULL){
				currNode = currNode->right;
			}
			//we don't delete delNode, we delete currNode so just replace the value
			delNode->value = currNode->value;
			delNode->left = deleteNode(delNode->left, currNode->value);
		}
		//Case 3) if both children exist
		else{
			Tree currNode = delNode;
			currNode = currNode->right;
			while(currNode->left != NULL){
				currNode = currNode->left;
			}
			//we don't delete delNode, we delete currNode so just replace the value
			delNode->value = currNode->value;
			delNode->right = deleteNode(delNode->right, currNode->value);
		}
	}
	return delNode;
}


void printInorder(Tree root){
	Tree currNode = root;
	//if currNode == NULL then stop traversal
	if(currNode){
		printInorder(currNode->left);
		fprintf(fout, "%d ", currNode->value);
		printInorder(currNode->right);
	}
}

void deleteTree(Tree root){
	//recursively delete whole tree
	Tree delNode = root;
	if(delNode){
		deleteTree(delNode->left);
		deleteTree(delNode->right);
		free(delNode);
	}
}

void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char cv;
	int key;

	Tree root = NULL;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		if(feof(fin))
			break;
		switch(cv){
			case 'i':
				fscanf(fin, "%d", &key);
				root = insertNode(root, key);
				break;
			case 'f':
				fscanf(fin,"%d",&key);
				if(findNode(root, key))
					fprintf(fout, "%d is in the tree\n", key);
				else
					fprintf(fout, "finding error: %d is not in the tree\n", key);
				break;
			case 'd':
				fscanf(fin, "%d", &key);
				if(findNode(root, key)){
					root = deleteNode(root, key);
					fprintf(fout, "delete %d\n", key);
				}
				else{
					fprintf(fout, "deletion error: %d is not in the tree\n", key);
				}
				break;
			case 'p':
				fscanf(fin, "%c", &cv);
				if(cv == 'i'){
					if(root == NULL)
						fprintf(fout, "tree is empty");
					else
						printInorder(root);
				}
				fprintf(fout, "\n");
				break;
		}
	}
	deleteTree(root);
}