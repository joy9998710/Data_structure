#include<stdio.h>
#include<stdlib.h>
#define MIN -987654321

FILE *fin;
FILE *fout;

typedef struct BNode* BNodePtr;

struct BNode{
    int order;
    int size;           /* number of children */
    BNodePtr *child;    /* children pointers */
    int *key;           /* keys */
    int is_leaf;
}BNode;

BNodePtr CreateTree(int order);

void Insert(BNodePtr *root, int key);
BNodePtr Insert_sub(BNodePtr parent, BNodePtr node, int key, int pos);
BNodePtr split_node(BNodePtr parent, BNodePtr node, int pos);
BNodePtr Createnode(int order);

int Find(BNodePtr root, int key);

void PrintTree(BNodePtr root);

void DeleteTree(BNodePtr root);
void free_node(BNodePtr node);



int main(int argc, char* argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    int order;
    fscanf(fin, "%d", &order);
    BNodePtr root = CreateTree(order);

    char cv;
    int key;
    while(!feof(fin)){
        fscanf(fin, "%c", &cv);
        switch(cv){
            case 'i':
                fscanf(fin, "%d", &key);
                if(Find(root, key))
                    fprintf(fout, "insert error : key %d is already in the tree!\n", key);
                else
                    Insert(&root, key);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if(Find(root, key))
                    fprintf(fout, "key %d found\n", key);
                else
                    fprintf(fout, "finding error : key %d is not in the tree!\n", key);
                break;
            case 'p':
                if (root->size == 1)
                    fprintf(fout, "print error : tree is empty!");
                else
                    PrintTree(root);
                fprintf(fout, "\n");
                break;
        }
    }
   
    DeleteTree(root);
    fclose(fin);
    fclose(fout);

    return 0;
}

/* 새로운 트리를 생성하는 함수 */
BNodePtr CreateTree(int order){
    BNodePtr BTree = (BNodePtr)malloc(sizeof(BNode));
    //initialize size with 1
    BTree->size = 1;
    BTree->order = order;
    BTree->key = (int*)malloc(sizeof(int)*order);
    //처음에 생성된 노드는 루트인 동시에 leaf 노드이다
    BTree->is_leaf = 1;
    //생성하는 children의 개수를 order로 초기화 한다
    //split 할 때 포인터도 같이 옮겨줘야 하므로 order + 1로 크기를 잡는다
    BTree->child = (BNodePtr*)malloc(sizeof(BNodePtr)*(order+1));
    for(int i = 0; i <= order; i++){
        BTree->child[i] = NULL;
    }
    return BTree;
}

/* insert key */
void Insert(BNodePtr *root, int key){ // 루트를 함수 내에서 변경시키기 위해 이중 포인터로 선언
    *root = Insert_sub(*root, *root, key, 0); // 리턴값이 있는 재귀함수 활용, 처음에 root 노드는 현재 노드이자 parent이므로 같이 parent, node 똑같이 매개변수 전달
}

//p_pos를 parent가 node를 가리키는 포인터 인덱스로 하자
//이 함수는 실질적인 insert함수이다
BNodePtr Insert_sub(BNodePtr parent, BNodePtr node, int key, int p_pos){
    //현재 노드가 리프노드가 아니면 계속해서 key값이 들어갈 곳을 탐색
    if(node->is_leaf == 0){
        int flag = 0;
        //어디로 가야하는 지를 찾는다
        for(int i = 0; i < node->size - 1; i++){
            if(node->key[i] > key){
                flag = 1;
                node = Insert_sub(node, node->child[i], key, i);
                break;
            }
        }
        if(flag == 0){
            node = Insert_sub(node, node->child[node->size - 1], key, node->size -1);
        }
    }
    //현재 노드가 리프 노드일 때 실제 삽입이 일어남
    else{
        int pos = 0;
        int flag = 0;
        //insert할 위치를 찾는다
        for(;pos < node->size -1; pos++){
            if(node->key[pos] > key){
                break;
            }
        }
        //pos 뒤쪽으로 다 밀기
        for(int i = node->size -2; i >= pos; i--){
            node->key[i+1] = node->key[i];
            node->child[i+1] = node->child[i];
        }
        node->key[pos] = key;
        node->size++;
    }
    //노드가 overflow가 났으면 split node 해주기
    if(node->size == node->order+1){
        return split_node(parent, node, (node->order-1)/2);
    }
    return parent;
}

//pos는 승진해야하는 중간 값을 의미
BNodePtr split_node(BNodePtr parent, BNodePtr node, int pos){
    //overflow가 난 것이 루트인경우
    if(parent == node){
        //새로운 우측 트리
        BNodePtr newNode = Createnode(node->order);
        //새로운 루트
        BNodePtr newRoot = Createnode(node->order);
        int midkey = node->key[pos];
        int j = 0;
        //새로운 노드에 포인터랑 키 주기
        for(int i = pos+1; i < node->size -1; i++){
            newNode->key[j] = node->key[i];
            newNode->child[j] = node->child[i];
            j++;
        }
        newNode->child[j] = node->child[node->size - 1];
        //원래 노드를 왼쪽 노드로써 재활용 하기위해 다른 노드로 넘어간 데이터 값을 모두 지운다
        for(int i = pos; i < node->size -2; i++){
            node->key[i] = MIN;
            node->child[i+1] = NULL;
        }
        node->key[node->size-2] = MIN;
        //홀수 차/짝수 차 size가 달라짐
        if(node->order%2 == 0){
            node->size = pos + 1;
            newNode->size = pos + 2;
        }
        else{
            node->size = pos + 1;
            newNode->size = pos + 1;
        }
        //리프노드 설정하기
        if(node->is_leaf){
            node->is_leaf = 1;
            newNode->is_leaf = 1;
            newRoot->is_leaf = 0;
        }
        else{
            node->is_leaf = 0;
            newNode->is_leaf = 0;
            newRoot->is_leaf = 0;
        }
        //새로운 루트에 데이터 삽입 및 노드 연결
        newRoot->key[0] = midkey;
        newRoot->child[0] = node;
        newRoot->child[1] = newNode;
        newRoot->size = 2;
        return newRoot;
    }
    //새로운 노드(우측)
    BNodePtr newNode = Createnode(parent->order);
    //중간값
    int midkey = node->key[pos];
    int flag = 0;
    //parent의 몇 번째 child인지를 나타내는 변수
    int ptrpos = 0;
    //parent에서 node로 이어지는 포인터 찾기
    //위 for문에서 못 찾으면 마지막 원소보다 midkey값이 큰 것이므로 ptrpos = order - 1;
    for(;ptrpos < parent->size - 1; ptrpos++){
        if(parent->key[ptrpos] > midkey){
            flag = 1;
            break;
        }
    }
    //새로운 노드에 원소 밀어주기
    int j = 0;
    for(int i = pos+1; i < node->size - 1; i++){
        newNode->key[j] = node->key[i];
        newNode->child[j] = node->child[i];
        j++;
    }
    newNode->child[j] = node->child[node->size - 1];
    //원래의 노드를 재활용 하기 위해 원래 노드에서 다른 노드로 넘어간 데이터들 초기화
    for(int i = pos; i < node->size-2; i++){
        node->key[i] = MIN; 
        node->child[i+1] = NULL;
    }
    node->key[node->size-2] = MIN;
    //홀수 차/짝수 차 size가 달라짐
    if(parent->order%2 == 0){
        node->size = pos + 1;
        newNode->size = pos + 2;
    }
    else{
        node->size = pos + 1;
        newNode->size = pos + 1;
    }
    //리프노드 설정하기
    if(node->is_leaf){
        node->is_leaf = 1;
        newNode->is_leaf = 1;
    }
    else{
        node->is_leaf = 0;
        newNode->is_leaf = 0;
    }
    //parent 포인터 밀어주기 및 key값 밀어주기
    for(int i = parent->size - 2; i >= ptrpos; i--){
        parent->key[i+1] = parent->key[i];
        parent->child[i+2] = parent->child[i+1];
    }
    //parent에 데이터 승진시키기
    parent->key[ptrpos] = midkey;
    //새로운 노드를 parent에 포인터로 연결해주기
    parent->child[ptrpos + 1] = newNode;
    parent->size++;
    return parent;
}

/* 노드 하나를 동적할당하는 함수, size, is_leaf는 노드마다 다르므로 초기화하지 않고 공통되는 부분만 초기화 */
BNodePtr Createnode(int order){ 
    BNodePtr newNode = (BNodePtr)malloc(sizeof(BNode));
    newNode->size = 1;
    newNode->order = order;
    //일단은 리프노드가 아니라고 생각
    newNode->is_leaf = 0;
    newNode->key = (int*)malloc(sizeof(int)*order);
    newNode->child = (BNodePtr*)malloc (sizeof(BNodePtr)*(order+1));
    for(int i = 0; i <= order + 1; i++){
        newNode->child[i] = NULL;
    }
    return newNode;
}

/* find node that has key */
int Find(BNodePtr root, int key){
    BNodePtr currNode = root;
    //현재노드가 NULL이면 찾는 값이 없다는 의미이므로 0을 리턴
    if(currNode == NULL){
        return 0;
    }
    //노드를 전체를 돌면서 recursive하게 찾기
    for(int i = 0; i < currNode->size - 1; i++){
        if(key < currNode->key[i]){
            return Find(currNode->child[i], key);
        }
        if(key == currNode->key[i]){
            return 1;
        }
    }
    //Loop를 모두 돌고 나왔을 때 그보다 작은 원소가 없다는 것은 마지막 key값보다 큰 것 이므로 child[size-3]으로 이동
    return Find(currNode->child[currNode->size - 1], key);
}

/* print Tree, inorder traversal */
void PrintTree(BNodePtr root){
    BNodePtr currNode = root;
    if(currNode != NULL){
        //가장 왼쪽으로 이동
        PrintTree(currNode->child[0]);

        for(int i = 0; i < currNode->size - 1; i++){
            fprintf(fout, "%d ", currNode->key[i]);
            PrintTree(currNode->child[i+1]);
        }
    }
}

/* depth first search -> free memory */
void DeleteTree(BNodePtr root){
    BNodePtr currNode = root;
    if(currNode == NULL){
        return;
    }
    for(int i = 0; i < root->size; i++){
        DeleteTree(currNode->child[i]);
    }
    //free 노드
    free(currNode->child);
    free(currNode->key);
    free(currNode);
}

/* 노드의 메모리를 해제해주는 helper function */
void free_node(BNodePtr node){
    free(node->child);
    free(node->key);
    free(node);
}
