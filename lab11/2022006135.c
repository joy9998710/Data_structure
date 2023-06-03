#include<stdio.h>
#include<stdlib.h>

FILE* fin;
FILE* fout;

typedef struct Array Array;

struct Array{
    int size;
    int* values;
};

Array* CreateArray(int size);
void QuickSort(Array* array, int left, int right, int depth);
int Partition(Array* array, int left, int right, int depth);
void PrintArray(Array* array, int left, int right);
void DeleteArray(Array* array);
void swap(int* a, int* b);

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

    int size, i;
    Array* array;

    fscanf(fin, "%d", &size);
    array = CreateArray(size);
    for(i = 0; i < size; i++){
        fscanf(fin, "%d", &array->values[i]);
    }

    QuickSort(array, 0, size - 1, 0);

    fprintf(fout, "\nsorting result :\n");
    PrintArray(array, 0, size - 1);

    DeleteArray(array);


	return 0;
}

Array* CreateArray(int size){
    Array* arr = (Array*)malloc(sizeof(Array));
    arr->size = size;
    arr->values = (int*)malloc(sizeof(int)*size);
    return arr;
}

void QuickSort(Array* array, int left, int right, int depth){
    if(left < right){
        int pivotIdx = Partition(array, left, right, depth);
        QuickSort(array, left, pivotIdx - 1, depth + 1);
        QuickSort(array, pivotIdx+1, right, depth + 1);
    }
}

//pivot의 최후 인덱스를 반환
int Partition(Array* array, int left, int right, int depth){
    //마지막 인덱스에 위치한 value가 pivot
    int pivot = array->values[right];
    fprintf(fout, "left : %d, right : %d, pivot : %d\n", left, right, pivot);
    PrintArray(array,left,right);

    //i는 왼쪽부터 차례로 오면서 비교
    //j는 오른쪽부터 차례로 오면서 비교
    int i = left;
    int j = right - 1;

    //서로 인덱스가 엇갈리기 전까지 loop
    while(i <= j){
        //작은 것 발견할때 까지 i 증가
        while(i <= j && array->values[i] <= pivot){
            i++;
        }
        //큰 것 발견 할 때 까지 j 감소
        while(i <= j && array->values[j] >= pivot){
            j--;
        }
        //엇갈린 위치가 아니면 swap
        if(i < j){
            swap(&array->values[i], &array->values[j]);
            PrintArray(array, left, right);
        }
    }
    //마지막에 pivot과 swap
    swap(&array->values[i], &array->values[right]);
    if(j != right - 1){
        PrintArray(array,left,right);
    }
    return i;
}

void PrintArray(Array* array, int left, int right){
    for(int i = 0; i < left; i++){
        fprintf(fout, "%-3s", "-");
    }
    for(int i = left; i <= right; i++){
        fprintf(fout, "%-3d", array->values[i]);
    }
    for(int i = right + 1; i < array->size; i++){
        fprintf(fout, "%-3s", "-");
    }
    fprintf(fout, "\n");
}

void DeleteArray(Array* array){
    free(array->values);
    free(array);
}

//swap 함수 : 값을 직접 바꾸어야 하므로 포인터로 접근
void swap(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}