#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

struct AVLNode;
typedef struct AVLNode *Position;
typedef struct AVLNode *AVLTree;
typedef int ElementType;

typedef struct AVLNode{
	ElementType element;
	AVLTree left, right;
	int height;
}AVLNode;

int max(int a, int b){
    if(a > b){
        return a;
    }
    return b;
}

int Height(Position p){
    if(p == NULL){
        return -1;
    }
    if(p->left == NULL && p->right == NULL){
        p->height = 0;
    }
    else if(p->left == NULL && p->right != NULL){
        p->height = p->right->height + 1;
    }
    else if(p->left != NULL && p->right == NULL){
        p->height = p->left->height + 1;
    }
    else{
        p->height = max(p->left->height, p->right->height) + 1;
    }
    return p->height;
}

//Single Rotation with left = single right rotation
Position SingleRotateWithLeft(Position node){
    //change the arrow between node and node->left
    Position currNode = node->left;
    //LR becomes the left child of node
    node->left = currNode->right;
    currNode->right = node;
    //recursively set the height of nodes
    node->height = max(Height(node->left), Height(node->right)) + 1;
    currNode->height = max(Height(currNode->left), Height(currNode->right)) + 1;
    return currNode;
}

//Single Rotation with right = single left rotation
Position SingleRotateWithRight(Position node){
    Position currNode = node->right;
    //RL becomes the right child of node
    node->right = currNode->left;
    currNode->left = node;
    //recursively set the Height of nodes
    node->height = max(Height(node->left), Height(node->right)) + 1;
    currNode->height = max(Height(currNode->left), Height(currNode->right)) + 1;
    return currNode;
}

//single left rotation of left child and single right rotation of node
Position DoubleRotateWithLeft(Position node){
    Position currNode = node->left;
    //single left rotation of left child and set as node->left
    node->left = SingleRotateWithRight(currNode);
    //single right rotation of root which in this case "node"
    return SingleRotateWithLeft(node);
}

//single right rotation of right child and single left rotation of node
Position DoubleRotateWithRight(Position node){
    Position currNode = node->right;
    //single right rotation of right child and set as node->right
    node->right = SingleRotateWithLeft(currNode);
    //single left rotation of root which in this case "node"
    return SingleRotateWithRight(node);
}

//using to balance the tree
Position Balance(Position currNode){
    int dif = Height(currNode->left) - Height(currNode->right);
    //left side depth is bigger than right side depth
    if(dif >= 2){
        Position L = currNode->left;
        //Case 1) LL
        if(Height(L->left) > Height(L->right)){
            return SingleRotateWithLeft(currNode);
        }
        //Case 2) LR
        else if(Height(L->left) < Height(L->right)){
            return DoubleRotateWithLeft(currNode);
        }
    }
    else if(dif <= -2){
        Position R = currNode->right;
        //Case 3) RR
        if(Height(R->left) < Height(R->right)){
            return SingleRotateWithRight(currNode);
        }
        //Case 4) RL
        else if(Height(R->left) > Height(R->right)){
            return DoubleRotateWithRight(currNode);
        }
    }
    return currNode;
}
//Inserting node in the tree
AVLTree Insert(ElementType X, AVLTree T){
    //if T == NULL then ie means element should be inserted there so make a new element
    if(T == NULL){
        AVLTree newNode = (AVLTree)malloc(sizeof(AVLNode));
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->height = 0;
        newNode->element = X;
        return newNode;
    }
    //if newElement is bigger than T->element then move to the right(recursively)
    if(X > T->element){
       T->right = Insert(X, T->right);
       T->height = max(Height(T->left), Height(T->right)) + 1;
       return Balance(T);
    }
    //if newElement is smaller than T->element then move to the left(recursively)
    else if(X < T->element){
        T->left = Insert(X, T->left);
        T->height = max(Height(T->left), Height(T->right)) + 1;
        return Balance(T);
    }
    else{
        fprintf(fout, "insertion error : %d is already in the tree!\n", X);
    }
    return T;
}

//Deleting node in the tree
AVLTree Delete(ElementType X, AVLTree T){
    AVLTree delNode = T;
    //if we cannot find the key value just return the original delNode
    if(delNode == NULL){
        fprintf(fout, "deletion error : %d is not in the tree!\n", X);
        return delNode;
    }
    //finding key value recursively
    else if(delNode->element < X){
        delNode->right = Delete(X,delNode->right);
        delNode->height = max(Height(delNode->left), Height(delNode->right)) + 1;
        return Balance(delNode);
    }
    else if(delNode->element > X){
        delNode->left = Delete(X,delNode->left);
        delNode->height = max(Height(delNode->left), Height(delNode->right)) + 1;
        return Balance(delNode);
    }
    else{
        //Case 1) No child
        if(delNode->left == NULL && delNode->right == NULL){
            Position tempNode = NULL;
            free(delNode);
            return tempNode;
        }
        //Case 2) 1 child
        //Case 2-1) if only right child exist
        else if(delNode->left == NULL){
            Position currNode = delNode;
            currNode = currNode->right;
            while(currNode->left != NULL){
                currNode = currNode->left;
            }
            //we don't delete delNode we delete currNode so just replace the value
            delNode->element= currNode->element;
            delNode->right = Delete(currNode->element, delNode->right);
        }
        //Case 2-2) if only left child exist
        else if(delNode->right == NULL){
            Position currNode = delNode;
            currNode = currNode->left;
            while(currNode->right != NULL){
                currNode = currNode->right;
            }
            //we don't delete delNode we delete currNdoe so just replace the value
            delNode->element = currNode->element;
            delNode->left = Delete(currNode->element, delNode->left);
        }

        //Case 3) if both children exist
        else{
            Position currNode = delNode;
            currNode = currNode->right;
            while(currNode->left!= NULL){
                currNode = currNode->left;
            }
            //we don't delete delNode, we delete currNode so just replace the value
            delNode->element = currNode->element;
            delNode->right = Delete(currNode->element, delNode->right);
        }
    }
    
    return delNode;
}

//Printing AVLTree with in order traversal
void PrintInorder(AVLTree T){
    if(T){
        PrintInorder(T->left);
        fprintf(fout,"%d(%d) ", T->element, T->height);
        PrintInorder(T->right);
    }
}

//Deleting whole tree
void DeleteTree(AVLTree T){
    AVLTree delNode = T;
    if(delNode){
        DeleteTree(delNode->left);
        DeleteTree(delNode->right);
        free(delNode);
    }
}

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	AVLTree Tree = NULL;
	char cv;
	int key;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d\n", &key);
				Tree = Insert(key, Tree);
				break;
			case 'd':
				fscanf(fin, "%d\n", &key);
				Tree = Delete(key, Tree);
				break;
		}
		PrintInorder(Tree);
		fprintf(fout, "\n");
	}

	DeleteTree(Tree);
	fclose(fin);
	fclose(fout);

	return 0;
}

