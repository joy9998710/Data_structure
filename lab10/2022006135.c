#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct Node {
	int vertex;
	int dist;	//distance
	int prev;
}Node;

typedef struct Graph {
	int size;
	int** vertices;
	Node* nodes;
}Graph;

typedef struct Heap {
	int Capacity;
	int Size;
	Node* Element;
}Heap;

Graph* createGraph(int size);
void deleteGraph(Graph* g);
void printShortestPath(Graph* g);
Heap* createMinHeap(int heapSize);
void deleteMinHeap(Heap* minHeap);
void insertToMinHeap(Heap* minHeap, int vertex, int distance);
Node deleteMin(Heap* minHeap);


void main(int argc, char *argv[]) {
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	Graph* g;
	int size;
	fscanf(fin, "%d\n", &size);
	g = createGraph(size + 1);

	char tmp = 0;
	while (tmp != '\n' && tmp != EOF) {
		int node1, node2, weight;
		fscanf(fin, "%d-%d-%d", &node1, &node2, &weight);
		g->vertices[node1][node2] = weight;
		tmp = fgetc(fin);
	}

	printShortestPath(g);

	deleteGraph(g);
}

Graph* createGraph(int size){
    //그래프 메모리 할당
    Graph* g = (Graph*)malloc(sizeof(Graph));
    /* main 함수를 보면 createGraph 함수에 argument로 들어온 size 값은
    입력받은 size 값 보다 하나 크다 이는 vertices의 index와 node의 vertex간의
    호환성을 위한 것이므로 vertices에서만 size를 활용한다*/
    // 그렇기에 g->size = size -1 로 설정하여 vertices만 size + 1 로 접근하도록 하겠다
    g->size = size - 1;
    //size 크기로 배정
    g->vertices = (int**)malloc(sizeof(int*)*size);
    for(int i = 0; i < size; i++){
        g->vertices[i] = (int*)malloc(sizeof(int)*size);
    }
    //나머지는 g->size만큼 배정
    g->nodes = (Node*)malloc(sizeof(Node)*(g->size));
    for(int i = 0; i < g->size; i++){
        //각 노드의 vertex 설정
        g->nodes[i].vertex = i + 1;
        //이전 노드가 모두 NIL 노드로 설정
        g->nodes[i].prev = -1;
        //노드 까지의 거리를 모두 무한대로 설정
        g->nodes[i].dist = INF;
    }
    return g;
}

//그래프를 free
void deleteGraph(Graph* g){
    //vertices free
    for(int i = 0; i < g->size + 1; i++){
        free(g->vertices[i]);
    }
    free(g->vertices);
    //free nodes
    free(g->nodes);
    //free Graph
    free(g);
}

//경로를 recursive하게 프린트하는 함수
void printPath(Graph* g, Node currNode){
    //currNode가 start노드가 아닐때까지 계속 recursive하게 찾기
    if(currNode.vertex != 1){
        //prev로 가면서 찾기
        printPath(g, g->nodes[currNode.prev-1]);
        //stack 구조 인 것을 이용해 프린트하기
        fprintf(fout, "->%d", currNode.vertex);
    }
    //start 일 때는 그것 만 프린트
    else{
        fprintf(fout, "%d", currNode.vertex);
    }
}

void printShortestPath(Graph* g){
    //모든 노드의 거리는 createGraph 당시 이미 INF로 초기화 되어 있음
    //시작 노드 거리 0으로 초기화
    //방문을 체크하는 노드
    int* visited = (int*)malloc(sizeof(int)*(g->size + 1));
    for(int i = 0; i <= g->size; i++){
        visited[i] = 0;
    }
    g->nodes[0].dist = 0;
    Heap* Q = createMinHeap(g->size);
    insertToMinHeap(Q,g->nodes[0].vertex, g->nodes[0].dist);
    while(Q->Size != 0){
        Node currNode = deleteMin(Q);
        visited[currNode.vertex] = 1;
        for(int i = 0; i < g->size; i++){
            //노드가 인접하면
            if(g->vertices[currNode.vertex][i+1] > 0 && visited[g->nodes[i].vertex] == 0){
                if(currNode.dist + g->vertices[currNode.vertex][i+1] < g->nodes[i].dist){
                    g->nodes[i].dist = currNode.dist + g->vertices[currNode.vertex][i+1];
                    g->nodes[i].prev = currNode.vertex;
                    insertToMinHeap(Q, g->nodes[i].vertex, g->nodes[i].dist);
                }
            }
        }
    }
    for(int i = 0; i < g->size; i++){
        //시작노드의 길이는 0으로 정해져 있으므로 굳이 출력할 필요 x
        if(g->nodes[i].vertex == 1){
            continue;
        }
        //도달하지 못한 노드를 위한 출력
        if(g->nodes[i].dist == INF){
            fprintf(fout, "can not reach to node %d\n", g->nodes[i].vertex);
            continue;
        }
        printPath(g, g->nodes[i]);
        //코스트 출력
        fprintf(fout, " (cost : %d)\n", g->nodes[i].dist);
    }
}

//HeapSize로 주어지는 값이 g->size이므로 배열의 index를 0부터 다루어도 됨
Heap* createMinHeap(int heapSize){
    Heap* minHeap = (Heap*)malloc(sizeof(Heap));
    //힙의 수용력은 heapSize
    minHeap->Capacity = heapSize;
    //힙이 현재는 비어있으므로 size = 0
    minHeap->Size = 0;
    //HeapSize만큼 minHeap 배열 만듦
    minHeap->Element = (Node*)malloc(sizeof(Node)*heapSize);
    //initialize minHeap
    for(int i = 0; i < heapSize; i++){
        minHeap->Element[i].vertex = -1;
        minHeap->Element[i].dist = INF;
    }
    return minHeap;
}

//Heap을 free하는 함수
void deleteMinHeap(Heap* minHeap){
    free(minHeap->Element);
    free(minHeap);
}

//insert하기 위한 percolateUp 함수
Heap* percolateUp(Heap* minHeap, int index){
    //현재 인덱스로부터 parent의 인덱스를 구함
    int parentIdx = (index-1)/2;
    //parent의 인덱스가 음수 => 이미 맨 꼭대기 까지 올라왔다는 뜻
    if(parentIdx < 0){
        return minHeap;
    }
    //parent의 dist값이 현재 노드의 dist 값보다 크면 둘을 바꾼다
    if(minHeap->Element[parentIdx].dist > minHeap->Element[index].dist){
        Node temp = minHeap->Element[parentIdx];
        minHeap->Element[parentIdx] = minHeap->Element[index];
        minHeap->Element[index] = temp;
        //이를 재귀적으로 구현
        return percolateUp(minHeap, parentIdx);
    }
    return minHeap;
}

//맨 마지막에 노드를 추가하고 이를 percolateUp 하기
void insertToMinHeap(Heap* minHeap, int vertex, int distance){
    //배열 마지막에 원소 추가 동시에 size 증가
    minHeap->Element[++minHeap->Size-1].vertex = vertex;
    minHeap->Element[minHeap->Size-1].dist = distance;
    minHeap = percolateUp(minHeap, minHeap->Size - 1);
}

Heap* percolateDown(Heap* minHeap, int index){
    int leftIdx = 2*index + 1;
    int rightIdx = 2*index + 2;
    //leftidx가 heapSize를 넘어버리면 rightIdx도 자동으로 넘음 => return
    if(leftIdx > minHeap->Size - 1){
        return minHeap;
    }
    //leftIdx가 heapSize와 같으면 비교는 leftIdx랑만 해도됨
    if(leftIdx == minHeap->Size - 1){
        if(minHeap->Element[leftIdx].dist < minHeap->Element[index].dist){
            Node temp = minHeap->Element[leftIdx];
            minHeap->Element[leftIdx] = minHeap->Element[index];
            minHeap->Element[index] = temp;
            return minHeap;
        }
    }
    //위 두 경우가 아닌경우가 일반적
    //rightchild의 dist 값이 leftchild의 dist값보다 크고
    if(minHeap->Element[leftIdx].dist < minHeap->Element[rightIdx].dist){
        //현재 노드의 dist값보다 leftchild의 dist 값이 더 작으면 percolateDown 수행
        if(minHeap->Element[leftIdx].dist < minHeap->Element[index].dist){
            Node temp = minHeap->Element[leftIdx];
            minHeap->Element[leftIdx] = minHeap->Element[index];
            minHeap->Element[index] = temp;
            return percolateDown(minHeap, leftIdx);
        }
    }

    //leftchild의 dist값이 rightchild의 dist 값보다 큰 경우
    if(minHeap->Element[rightIdx].dist < minHeap->Element[leftIdx].dist){
        //rightchild의 dist 값이 현재 노드의 dist값보다 작은경우
        if(minHeap->Element[rightIdx].dist < minHeap->Element[index].dist){
            Node temp = minHeap->Element[rightIdx];
            minHeap->Element[rightIdx] = minHeap->Element[index];
            minHeap->Element[index] = temp;
            return percolateDown(minHeap, rightIdx);
        }
    }
    return minHeap;
}

//percolateDown을 통해 deleteMin하기
Node deleteMin(Heap* minHeap){
    //최소 힙의 꼭대기 + size를 하나 제거
    Node temp = minHeap->Element[0];
    minHeap->Element[0] = minHeap->Element[minHeap->Size - 1];
    minHeap->Size--;
    minHeap = percolateDown(minHeap, 0);
    return temp;
}



