#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ISFULL 1
#define ISEMPTY 2
#define DIVIDEZERO 3

FILE *fin;
FILE *fout;

typedef struct Stack{
	int *key;
	int top;
	int max_stack_size;
}Stack;

Stack* CreateStack(int max);
void Push(Stack* S, int X);
int Pop(Stack* S);
int Top(Stack* S);
void DeleteStack(Stack* S);
int IsEmpty(Stack* S);
int IsFull(Stack* S);

void main(int argc, char *argv[]){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");

	Stack* stack;
	char input_str[101];
	int max=20, i=0,a,b,result, error_flag=0;

	fgets(input_str,101,fin);
	stack = CreateStack(max);
	
	fprintf(fout, "top numbers : ");
	while(input_str[i]!='#'){

        if(input_str[i] == '+'){
            //if + is the input then pop two times and push the add result
            int a, b, res;
            //checking whether the stack is empty or not
            if(IsEmpty(stack)){
                error_flag = ISEMPTY;
            }
            else{
                a = Pop(stack);
                b = Pop(stack);
                res = a+b;
            }
            //checking whether the stack is full or not
            if(IsFull(stack)){
                error_flag = ISFULL;
            }
            else{
                Push(stack,res);
            }
        }
        else if(input_str[i] == '-'){
            //if - is the input then pop two times and push the subtraction result
            int a, b, res;
            //checking wheter the stack is empty or not
            if(IsEmpty(stack)){
                error_flag = ISEMPTY;
            }
            else{
                a = Pop(stack);
                b = Pop(stack);
                res = b-a;
            }
            //checking wheter the stack is full or tno
            if(IsFull(stack)){
                error_flag = ISFULL;
            }
            else{
                Push(stack, res);
            }
        }
        else if(input_str[i] == '*'){
            //if * is the input then pop two times and push the multiplied result
            int a, b, res;
            //checking whether the stack is empty or not 
            if(IsEmpty(stack)){
                error_flag = ISEMPTY;
            }
            else{
                a = Pop(stack);
                b = Pop(stack);
                res = b*a;
            }
            //checking whether the stack is full or not
            if(IsFull(stack)){
                error_flag = ISFULL;
            }
            else{
                Push(stack,res);
            }
        }
        else if(input_str[i] == '/'){
            //if / is the input then pop two times and push the divided result
            int a, b, res;
            //checking wheter the stack is empty or not
            if(IsEmpty(stack)){
                error_flag = ISEMPTY;
            }
            else{
                a = Pop(stack);
                b = Pop(stack);
                res = b/a;
            }
            //checking whether the stack is full or not
            if(IsFull(stack)){
                error_flag = ISFULL;
            }
            else{
                Push(stack,res);
            }
        }
        else if(input_str[i] == '%'){
            //if % is the input then pop two times and push the remainder result
            int a, b, res;
            //checking wheter the stack is empty or full
            if(IsEmpty(stack)){
                error_flag = ISEMPTY;
            }
            else{
                a = Pop(stack);
                b = Pop(stack);
                res = b%a;
            }
            //checking whether the stack is full or not
            if(IsFull(stack)){
                error_flag = ISFULL;
            }
            else{
                Push(stack,res);
            }
        }
        else{
            //if the input is not the operator then push the number with int form
            //checking whether the stack is full or not
            if(IsFull(stack)){
                error_flag = ISFULL;
            }
            else{
                Push(stack,input_str[i] - '0');
            }
        }

		if(error_flag) break;

		int t = Top(stack);
		fprintf(fout, "%d ",t);
		result=t;
		i++;
	}

	if(error_flag == ISFULL){
		fprintf(fout, "\nerror : invalid postfix expression, stack is full!\n");
	}
	else if(error_flag == ISEMPTY){
		fprintf(fout, "\nerror : invalid postfix expression, stack is empty!\n");
	}
	else if(error_flag == DIVIDEZERO){
		fprintf(fout, "\nerror : invalid postfix expression, divide by zero!\n");
	}

	else{
		if(stack->top+1 > 1){
			fprintf(fout, "\nerror : invalid postfix expression, %d elements are left!\n", stack->top+1);
		}
		else{
			fprintf(fout, "\nevaluation result : %d\n",result);
		}
	}
	fclose(fin);
	fclose(fout);
	DeleteStack(stack);
}

Stack* CreateStack(int max){
    //malloc stack
    Stack* S = (Stack*)malloc(sizeof(Stack));
    //malloc int
    S->key = (int*)malloc(max*sizeof(int));
    //set top as -1
    S->top = -1;
    //set the maxxise of the stack
    S->max_stack_size = max;
    return S;
}

void DeleteStack(Stack* S){
    free(S->key);
}

void Push(Stack* S, int X){
    S->key[++S->top] = X;
}

int Pop(Stack* S){
    int tmp = S->key[S->top];
    --S->top;
    return tmp;
}

int Top(Stack* S){
    return S->key[S->top];
}

int IsFull(Stack* S){
    return S->top == S->max_stack_size-1;
}

int IsEmpty(Stack* S){
    return S->top == -1;
}
