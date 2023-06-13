#include<stdio.h>
#include<stdlib.h>
#include<time.h>

FILE *fin;
FILE *fout;

typedef struct _DisjointSet {
    int size_maze;
    int *ptr_arr;
}DisjointSets;

void init(DisjointSets *sets, DisjointSets *maze_print, int num);
void Union(DisjointSets *sets, int i, int j);
int find(DisjointSets *sets, int i);
void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num);
void printMaze(DisjointSets *sets, int num);
void freeMaze(DisjointSets *sets, DisjointSets *maze_print);

void swap(int *x, int *y) {
    int t = *x;
    *x = *y;
    *y = t;
}

int main(int argc, char* agrv[]) {
    srand((unsigned int)time(NULL));

    int num, i;
    fin = fopen(agrv[1], "r");
    fout = fopen(agrv[2], "w");

    DisjointSets *sets, *maze_print;
    fscanf(fin, "%d", &num);

    sets = (DisjointSets*)malloc(sizeof(DisjointSets));
    maze_print = (DisjointSets*)malloc(sizeof(DisjointSets));

    init(sets, maze_print, num);
    createMaze(sets, maze_print, num);
    printMaze(maze_print, num);

    freeMaze(sets, maze_print);

    fclose(fin);
    fclose(fout);

    return 0;
}

void init(DisjointSets *sets, DisjointSets *maze_print, int num) {
	int i;

	sets->size_maze = num * num;
	sets->ptr_arr = (int*)malloc(sizeof(int) * (sets->size_maze));
	for (i = 0; i < sets->size_maze; i++) sets->ptr_arr[i] = i;

	maze_print->size_maze = num * num * 2;
	maze_print->ptr_arr = (int*)malloc(sizeof(int) * (maze_print->size_maze));
	for (i = 0; i < maze_print->size_maze; i++) maze_print->ptr_arr[i] = 1;
	//maze_print->ptr_arr[(x-1)*2+1 ~ x*2] : two directions(right, down) walls of the number x

	//start and end position have not to have the wall
	maze_print->ptr_arr[maze_print->size_maze - 2] = 0;
}

//작은 값이 부모여야함
void Union(DisjointSets* sets, int i, int j){
    //a, b의 parent 찾기
    int a = find(sets,i);
    int b = find(sets,j);
    //a < b면 b parent a로 설정
    if(a < b){
        sets->ptr_arr[b] = a;
    }
    else{
        sets->ptr_arr[a] = b;
    }
}

int find(DisjointSets* sets, int i){
    //root면 리턴
    if(sets->ptr_arr[i] == i){
        return i;
    }
    //parent를 루트로 바꾸면서 재귀적으로 find
    return sets->ptr_arr[i] = find(sets,sets->ptr_arr[i]);
}

void createMaze(DisjointSets* sets, DisjointSets* maze_print, int num){
    while(1){
        int flag = 1;
        for(int i = 0; i < num*num; i++){
            if(sets->ptr_arr[i] != 0){
                flag = 0;
            }
        }
        if(flag) break;
        int r = rand();
        int a = (int)r % (num*num*2);
        //맨 오른쪽 벽
        if(a%(num*2) == (num - 1) * 2) continue; 
        //맨 아래쪽 벽
        if(a/(num*2) == num - 1 && (a % 2)) continue;
        //오른쪽 벽
        if(a % 2 == 0){
            //사이클 방지를 위해 같은 부모에 속해 있는 지 확인
            if(find(sets,a/2) == find(sets,a/2 + 1)) continue;
            Union(sets, a/2, a/2+1);
            //벽제거
            maze_print->ptr_arr[a] = 0;
        }
        //아래쪽 벽
        else{
            //사이클 방지를 위한 체크
            if(find(sets,a/2) == find(sets, a/2 + num)) continue;
            Union(sets, a/2, a/2 + num);
            //벽제거
            maze_print->ptr_arr[a] = 0;
        }
    }
}


void printMaze(DisjointSets* sets, int num){
    fprintf(fout, "+");
    for(int i = 0; i < num; i++){
        fprintf(fout, "---+");
    }
    for(int i = 0; i < num; i++){
        //입구
        if(i == 0){
            fprintf(fout, "\n ");
        }
        else{
            fprintf(fout, "\n|");
        }
        for(int j = 0; j < num; j++){
            //오른쪽 벽
            if(sets->ptr_arr[num*i*2 + j*2]){
                fprintf(fout, "   |");
            }
            //벽없음
            else{
                fprintf(fout, "    ");
            }
        }
        fprintf(fout,"\n+");
        for(int j = 0; j < num; j++){
            if(sets->ptr_arr[num*i*2+j*2+1]){
                //아래 벽
                fprintf(fout, "---+");
            }
            else{
                fprintf(fout, "   +");
            }
        }
    }
}

void freeMaze(DisjointSets* sets, DisjointSets* maze_print){
	free(sets->ptr_arr);
	free(maze_print->ptr_arr);
	free(sets);
	free(maze_print);
}