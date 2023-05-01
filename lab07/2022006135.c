#include<stdio.h>
#include<stdlib.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct HeapStruct{
	int Capacity;
	int Size;
	int *Elements;
}Heap;

Heap* CreateHeap(int heapSize);
void Insert(Heap *heap, int value);
int Find(Heap *heap, int value);
int DeleteMax(Heap* heap);
void PrintHeap(Heap* heap);
int IsFull(Heap *heap);

int main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	char cv;
	Heap* maxHeap;
	int heapSize, key, max_element;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'n':
				fscanf(fin, "%d", &heapSize);
				maxHeap = CreateHeap(heapSize);
				break;
			case 'i':
				fscanf(fin, "%d", &key);
				Insert(maxHeap, key);
				break;
			case 'd':
				max_element = DeleteMax(maxHeap);
				if(max_element != -INF){
					fprintf(fout, "max element : %d deleted\n", max_element);
				}
				break;
			case 'p':
				PrintHeap(maxHeap);
				break;
			case 'f':
				fscanf(fin, "%d", &key);
				if(Find(maxHeap, key)) fprintf(fout, "%d is in the heap\n", key);
				else fprintf(fout, "finding error : %d is not in the heap\n", key);
				break;
		}
	}

	return 0;
}

Heap* CreateHeap(int heapSize){
    //memory allocation for heap
    Heap* h = (Heap*)malloc(sizeof(Heap));
    //initialize heap's capacity
    h->Capacity = heapSize;
    //initialize the size of heap as -1
    h->Size = -1;
    //memory allocation for the elemnts in heap
    h->Elements = (int*)malloc(sizeof(int)*h->Capacity);
    return h;
}

Heap* percolateUp(Heap* heap, int idx){
    //finding parent's index
    int paridx = (idx-1)/2;
    if(paridx < 0){
        return heap;
    }
    if(heap->Elements[paridx] < heap->Elements[idx]){
        int temp = heap->Elements[idx];
        heap->Elements[idx] = heap->Elements[paridx] ;
        heap->Elements[paridx] = temp;
        return percolateUp(heap, paridx);
    }
}

void Insert(Heap *heap, int value){
    //if the value found then print out error message
    if(Find(heap, value)){
        fprintf(fout, "insert error : %d is already in the heap\n", value);
        return;
    }
    //if the heap is full then print out the error message
    if(IsFull(heap)){
        fprintf(fout, "insert error : heap is full\n");
        return;
    }
    heap->Elements[++heap->Size] = value;
    fprintf(fout, "insert %d\n", value);
    percolateUp(heap, heap->Size);
}

int Find(Heap* heap, int value){
    for(int i = 0; i <= heap->Size; i++){
        if(heap->Elements[i] == value){
            return 1;
        }
    }
    return 0;
}
Heap* percolateDown(Heap* heap, int idx){
    int leftidx = 2*idx + 1;
    int rightidx = 2*idx + 2;
    //if idx get over the size of heap then return
    if(leftidx > heap->Size || rightidx > heap->Size){
        return heap;
    }
    //deciding to go to right or to left
    if(heap->Elements[leftidx] >= heap->Elements[rightidx]){
        if(heap->Elements[idx] > heap->Elements[leftidx]){
            int temp = heap->Elements[idx];
            heap->Elements[idx] = heap->Elements[leftidx];
            heap->Elements[leftidx] = temp;
            return percolateDown(heap, leftidx);
        }
    }
    else if(heap->Elements[rightidx] > heap->Elements[leftidx]){
        int temp = heap->Elements[idx];
        heap->Elements[idx] = heap->Elements[rightidx];
        heap->Elements[rightidx] = temp;
        return percolateDown(heap, rightidx);
    }
    return heap;
}

int DeleteMax(Heap* heap){
    //checking wheter the heap is empty or no
    if(heap->Size == -1){
        fprintf(fout, "delete error : heap is empty\n");
        return -INF;
    }
    int temp = heap->Elements[0];
    heap->Elements[0] = heap->Elements[heap->Size--];
    percolateDown(heap, 0);
    return temp;
}

void PrintHeap(Heap* heap){
    //printing heap with level order
    if(heap->Size == -1){
        fprintf(fout, "print error : heap is empty\n");
        return;
    }
    for(int i = 0; i <= heap->Size; i++){
        fprintf(fout, "%d ", heap->Elements[i]);
    }
    fprintf(fout, "\n");
}

int IsFull(Heap* heap){ 
    //checking wheter the heap is full or not
    if(heap->Capacity -1 == heap->Size){
        return 1;
    }
    return 0;
}
