#include <stdio.h>
#include <stdlib.h>

FILE *fin;
FILE *fout;

typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef int ElementType;

struct Node{
	ElementType element;
	Position next;
};

List MakeEmpty(List L){
    Position currnode = L;
    if(L == NULL){
        L = (Position)malloc(sizeof(struct Node));
        L->element = -1;
        L->next = NULL;
        return L;
    }

    while(currnode != NULL){
        Position tmp = currnode->next;
        free(currnode);
        currnode = tmp;
    }
    L->element = -1;
    L->next = NULL;
    return L;
}

int IsEmpty(List L){
    if(L->next == NULL){
        return 1;
    }
    return 0;
}

int IsLast(Position P, List L){
    //if P is the last element of the list then the P->next will be NULL
    if(P->next == NULL){
        return 1;
    }
    return 0;
}

void Insert(ElementType X, List L, Position P){
    //malloc a newNode to insert a new node
    Position newNode = (Position)malloc(sizeof(struct Node));
    if(P->next == NULL){
        P->next = newNode;
        newNode->element = X;
        newNode->next = NULL;
        return;
    }
    //setting a temporary Position to point at P->next
    Position tmp = P->next;
    //set P->next to point at newNode
    P->next = newNode;
    //set newNode->next to point at tmp(the original P->next)
    newNode->next = tmp;
    //set the element of newNode
    newNode->element = X;
    return;
}

void PrintList(List L){
    if(IsEmpty(L)){
        fprintf(fout, "empty list!\n");
        return;
    }
    //accessing all the elements in the list
    for(Position currnode = L; currnode != NULL; currnode = currnode-> next){
        if(currnode == L) continue;
        fprintf(fout, "key:%d ", currnode->element);
    }
}

Position FindPrevious(ElementType X, List L){
    for(Position currnode = L; currnode != NULL; currnode = currnode->next){
        if(currnode->next == NULL){
            return currnode;
        }
        if(currnode->next->element == X){
            return currnode;
        }
    }
}

Position Find(ElementType X, List L){
    //searching through all the elements in the list and find the position
    for(Position currnode = L; currnode != NULL; currnode = currnode->next){
        if(currnode->element == X){
            return currnode;
        }
    }
    //if not found, then returning NULL
    return NULL;
}

void Delete(ElementType X, List L){
    Position tmp = Find(X,L);
    //if not exist print out error message
    if(tmp == NULL){
        fprintf(fout, "deletion(%d) failed : element %d is not in the list\n", X, X);
        return;
    }
    //find the previous element of tmp and connect with the next element of tmp
    Position tmp2 = FindPrevious(X,L);
    tmp2->next = tmp->next;
    free(tmp);
    return;
}

void DeleteList(List L){
    MakeEmpty(L);
    free(L);
}

int main(int argc, char **argv){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");
	char x;

	Position header=NULL, tmp=NULL;
	header = MakeEmpty(header);
	while(fscanf(fin,"%c",&x)!=EOF){
		if(x=='i'){
			int a,b;	fscanf(fin,"%d%d",&a,&b);
			tmp = Find(a, header);
			if(tmp!=NULL){
				fprintf(fout, "insertion(%d) failed : the key already exists in the list\n", a);
				continue;
			}
			tmp = Find(b, header);
			if(tmp==NULL){
				fprintf(fout, "insertion(%d) failed : can not find the location to be inserted\n", a);
				continue;
			}
			Insert(a, header, tmp);
		}
		else if(x=='d'){
			int a;	fscanf(fin,"%d",&a);
			Delete(a, header);
		}
		else if(x=='f'){
			int a;	fscanf(fin,"%d",&a);
			tmp = FindPrevious(a, header);
			if(IsLast(tmp, header)) fprintf(fout, "finding(%d) failed : element %d is not in the list\n",a,a);
			else{
				if(tmp->element>0) fprintf(fout, "key of the previous node of %d is %d\n", a, tmp->element);
				else fprintf(fout, "key of the previous node of %d is head\n",a);
			}
		}
		else if(x=='p') PrintList(header);
	}
	DeleteList(header);
	fclose(fin);
	fclose(fout);

	return 0;
}