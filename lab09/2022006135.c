#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *fin;
FILE *fout;

typedef struct _Queue {
	int* key;
	int first, rear, qsize, max_queue_size;
}Queue;

typedef struct _Graph {
	int size;
	int* node;
	int** matrix;
}Graph;

Graph* CreateGraph(int* nodes, int n);
void InsertEdge(Graph* G, int a, int b);
void PrintGraph(Graph* G);
void DeleteGraph(Graph* G);
void Topsort(Graph* G);
Queue* MakeNewQueue(int X);
int IsEmpty(Queue* Q);
int Dequeue(Queue* Q);
void Enqueue(Queue* Q, int X);
void DeleteQueue(Queue* Q);
void countInput(int* n, char* str);
void parseInput(int* arr, char* str, int n);
void checkIndegree(Graph* G, int* idg);
void sort_node(int* arr, int n);

int main(int argc, char* agrv[]) {
	fin = fopen(agrv[1], "r");
	fout = fopen(agrv[2], "w");

	char numbers[100], inputs[150];
	fgets(numbers, 100, fin);
	int n = 0;
	countInput(&n, numbers);
	int* nodes = (int*)malloc(sizeof(int)*n);
	parseInput(nodes, numbers, n);

	Graph* G = CreateGraph(nodes, n);

	fgets(inputs, 100, fin);
	int len = strlen(inputs), i, j;
	for (i = 0; i < len; i += 4) {
		int a = inputs[i] - '0', b = inputs[i + 2] - '0';
		InsertEdge(G, a, b);
	}

	PrintGraph(G);
	Topsort(G);
	DeleteGraph(G);

	fclose(fin);
	fclose(fout);

	return 0;
}

void countInput(int* n, char* str) {
	int len = strlen(str), i;
	for (i = 0; i < len; i++)
		if (0 <= str[i] - '0' && str[i] - '0' < 10) (*n)++;
}

void parseInput(int* arr, char* str, int n) {
	int len = strlen(str), i;
	int cnt = 0;
	for (i = 0; i < len; i++)
	if (0 <= str[i] - '0' && str[i] - '0' < 10) arr[cnt++] = str[i] - '0';
}

Graph* CreateGraph(int* nodes, int n){
	//그래프 메모리 할당
	Graph* G = (Graph*)malloc(sizeof(Graph));
	//그래프 사이즈 설정
	G->size = n;
	//그래프의 node 배열 메모리 할당
	G->node = (int*)malloc(sizeof(int)*n);
	//nodes에 주어진 노드들을 모두 node에 목사
	for(int i = 0; i < G->size; i++){
		G->node[i] = nodes[i];
	}
	//sort node
	sort_node(G->node, G->size);
	//matrix에 메모리 할당 => 이중배열
	G->matrix = (int**)malloc(sizeof(int*)*n);
	for(int i = 0; i < n; i++){
		G->matrix[i] = (int*)malloc(sizeof(int)*n);
	}
	//matrix의 모든 원소 0으로 초기화
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			G->matrix[i][j] = 0;
		}
	}
	return G;
}

//cycle을 분석하기 위해 DFS활용
void DFS(Graph* G, int v, int* visited, int* cycle){
	//현재 방문한 노드 체크
	visited[v] = 1;

	for(int i = 0; i < G->size; i++){
		//인접한 노드 탐색
		if(G->matrix[v][i] == 1){
			//이미 방문한 것이라면 cycle이 존재하는 것이므로 cycle = 1
			if(visited[i]){
				*cycle = 1;
				return ;
			}
			//방문하지 않은 노드를 방문했다면 계속해서 탐색을 수행
			if(!visited[i]){
				DFS(G, i, visited, cycle);
			}

		}
	}
	//현재 노드의 방문처리를 취소 => 다음 방문을 위한것
	visited[v] = 0;
}

int iscycle(Graph* G){
	//visited를 위한 메모리 할당
	int* visited = (int*)malloc(sizeof(int)*(G->size));
	//모든 노드를 아직 방문하지 않은것으로 처리
	for(int i = 0; i < G->size; i++){
		visited[i] = 0;
	}
	int cycle = 0;

	for(int i = 0; i < G->size; i++){
		if(!visited[i]){
			DFS(G,i, visited, &cycle);
			//cycle이 존재하면 1 리턴
			if(cycle){
				free(visited);
				return 1;
			}
		}
	}
	//모든 탐색이 끝나고도 cycle이 존재하지 않을 것을 확인했으면 return 0
	free(visited);
	return 0;
}

//각각의 node 번호가 존재하고 index는 이와 대응되게 존재함, matrix를 다루기 위해서는 index로 접근해야함 때문에 node번호에 대응되는 index번호를 찾아 대입해야함
void InsertEdge(Graph* G, int a, int b){
	//node에 대응하는 index찾기
	for(int i = 0; i < G->size; i++){
		if(G->node[i] == a){
			a = i;
		}
		if(G->node[i] == b){
			b = i;
		}
	}
	//a에서 b로 이동
	G->matrix[a][b] = 1;
}

void PrintGraph(Graph* G){
	fprintf(fout, "   ");
	//첫번째 줄에는 노드를 모두 출력
	for(int i = 0; i < G->size; i++){
		fprintf(fout, "%d  ", G->node[i]);
	}
	fprintf(fout, "\n");
	//matrix 출력
	for(int i = 0; i < G->size; i++){
		fprintf(fout, "%d  ", G->node[i]);
		for(int j = 0; j < G->size; j++){
			fprintf(fout, "%d  ", G->matrix[i][j]);
		}
		fprintf(fout, "\n");
	}
	fprintf(fout, "\n");
}

void DeleteGraph(Graph* G){
	//free node
	free(G->node);
	//free matrix
	for(int i = 0; i < G->size; i++){
		free(G->matrix[i]);
	}
	free(G->matrix);
	//free Graph
	free(G);
}

void Topsort(Graph* G){
	//cycle이 존재하면 에러 메세지 출력
	if(iscycle(G)){
		fprintf(fout, "sorting error : cycle!\n");
		return;
	}
	//큐 만들기
	Queue* Q = MakeNewQueue(G->size);
	//in-degree 저장할 배열
	int* idg = (int*)malloc(sizeof(int)*(G->size));
	//checkIndgree
	checkIndegree(G, idg);

	for(int i = 0; i < G->size; i++){
		if(idg[i] == 0){
			Enqueue(Q,i);
		}
	}
	while(!IsEmpty(Q)){
		int nodeidx = Dequeue(Q);
		fprintf(fout, "%d ", G->node[nodeidx]);
		for(int i = 0; i < G->size; i++){
			//인접한 지 판단
			if(G->matrix[nodeidx][i] == 1){
				//idg가 0인지 판단
				if(--idg[i] == 0){
					Enqueue(Q, i);
				}
			}
		}
	}
	fprintf(fout, "\n");
}

Queue* MakeNewQueue(int X){
	//Queue 메모리 할당
	Queue* Q = (Queue*)malloc(sizeof(Queue));
	//rear와 first가 꼬여있어야 empty
	Q->first = 0;
	Q->rear = -1;
	//key 메모리 할당
	Q->key = (int*)malloc(sizeof(int)*X);
	//큐 초기화
	for(int i = 0; i < X; i++){
		Q->key[i] = 0;
	}
	//현재 비어있으므로 qsize = 0
	Q->qsize = 0;
	//전체 Queue size는 X
	Q->max_queue_size = X;
	return Q;
}

int IsEmpty(Queue* Q){
	if(Q->qsize == 0){
		return 1;
	}
	return 0;
}

//first 증가
int Dequeue(Queue* Q){
	int temp = Q->key[Q->first];
	Q->first = (Q->first + 1) % (Q->max_queue_size);
	--Q->qsize;
	return temp;
}

void Enqueue(Queue* Q, int X){
	Q->rear = (Q->rear + 1) % (Q->max_queue_size);
	Q->key[Q->rear] = X;
	++Q->qsize;
}

void DeleteQueue(Queue* Q){
	//free key
	free(Q->key);
	//free Queue
	free(Q);
}

void checkIndegree(Graph* G, int* idg){
	for(int i = 0; i < G->size; i++){
		for(int j = 0; j < G->size; j++){
			if(G->matrix[j][i] == 1){
				idg[i]++;
			}
		}
	}
	
}

//node를 정렬하기
void sort_node(int* arr, int n){
	for(int i = 0; i < n-1; i++){
		for(int j = i+1; j < n; j++){
			if(arr[i] > arr[j]){
				int temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}
	}
}


