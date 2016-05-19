#include <stdio.h>
#include <stdlib.h>

void printm(int *A[], int row, int col)
{
	int i, j;
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			printf("%d ", A[i][j]);
		}
		printf("\n");
	}
}

void swap_rows(int **A, int i, int j)
{
	int *temp = A[i];
	A[i]=A[j];
	A[j]=temp;
}

int main(void){
	int **A;
	int i, j;
	int n=3;
	A=calloc(n, sizeof(int*));
	for(i=0;i<n;i++){
		A[i]=calloc(n, sizeof(int));
		for(j=0;j<n;j++){
			A[i][j]=(i+1)*(j+1);
		}
	}
	swap_rows(A, 1, 2);
	printm(A, n, n);
}
