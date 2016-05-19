#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static unsigned long long	fm_count;
static volatile bool		proceed = false;


static void done(int unused)
{
	proceed = false;
	unused = unused;
}

int fm_elim(rows, cols, a, c);
	
unsigned long long tpi12mhu_mki(char* aname, char* cname, int seconds)
{
	FILE*		afile = fopen(aname, "r");
	FILE*		cfile = fopen(cname, "r");

	fm_count = 0;

	if (afile == NULL) {
		fprintf(stderr, "could not open file A\n");
		exit(1);
	}

	if (cfile == NULL) {
		fprintf(stderr, "could not open file c\n");
		exit(1);
	}
	
	int** a;
	int* c;
	char ch;
	int rows, cols;
	int i, j, n;
	rows = fgetc(afile)-'0';
	fgetc(afile);
	cols = fgetc(afile)-'0';
	fgetc(afile);

	a=calloc(rows, sizeof(int*));
	for(i=0;i<rows;i++)
		a[i]=calloc(cols, sizeof(int));

	n=1;
	for(i=0;i<rows;i++){
		for(j=0;j<cols;j++){
			a[i][j]=0;
			ch=fgetc(afile);
			if(ch=='-'){
				n=-1;
				ch=fgetc(afile);
			}
			while(isdigit(ch)){
				a[i][j]*=10;
				a[i][j]+=ch-'0';
				ch=fgetc(afile);
			}
			a[i][j]*=n;
			n=1;
		}
	}

	c = calloc(rows, sizeof(int));

	fgetc(cfile);
	fgetc(cfile);
	for(i=0;i<rows;i++){
		c[i]=0;
		ch=fgetc(cfile);
		if(ch=='-'){
			n=-1;
			ch=fgetc(cfile);
		}
		while(isdigit(ch)){
			c[i]*=10;
			c[i]+=ch-'0';
			ch=fgetc(cfile);
		}
		c[i]*=n;
		n=1;
	}

	fclose(afile);
	fclose(cfile);

	if (seconds == 0) {
		/* Just run once for validation. */
			
		// Uncomment when your function and variables exist...
		// return fm_elim(rows, cols, a, c);
		return 1; // return one, i.e. has a solution for now...
	}

	/* Tell operating system to call function DONE when an ALARM comes. */
	signal(SIGALRM, done);
	alarm(seconds);

	/* Now loop until the alarm comes... */
	proceed = true;
	while (proceed) {
		// Uncomment when your function and variables exist...
		// fm_elim(rows, cols, a, c);

		fm_count++;
	}

	return fm_count;
}

void swap_rows(int **a, int *c, int i, int j)
{
	int* temp = a[i];
	a[i]=a[j];
	a[j]=temp;

	int temp2 = c[i];
	c[i]=c[j];
	c[j]=temp2;


}


int** sort_rows(int** a, int* c, int rows, int cols)
{
	int i, j, k;
	for(i=0;i<rows;i++){
		for(j=0;j>i+1;j++){
			if(a[i][cols-1]<a[j][cols-1]){
				swap_rows(a, c, i, j);
			}
		}
	}

	for(i=0;i<rows;i++){
		if(a[i][cols-1]<=0){
			k=i-1;
			break;
		}
	}
	
	for(i=k;i<rows;i++)
		for(j=k;j>i+1;j++)
			if(a[i][cols-1]>a[j][cols-1])
				a=swap_rows(a, c, i, j);
	
}

int fm_elim(int rows, int cols, int** a, int *c)
{
	int s=rows;
	int r=cols;
	int n1, n2;
	int i, j;
	int b, B;
	int **t, *q;
	t=calloc(s, sizeof(int*));
	q=calloc(s, sizeof(int));
	for(i=0;i<s;i++){
		t[i]=calloc(r, sizeof(int));
		q[i]=c[i];
		for(j=0;j<r;j++){
			t[i][j]=a[i][j];
		}
	}
	while(1){
		sort_rows(t, q, rows, cols);

		for(i=0;i<s;i++){
			if(t[i][r-1]<0){
				n1=i-1;
				break;

			for(i=0;i<s;i++){
				if(t[i][r-1]==0){
					n2=i-1;
					break;
				}
			}

		
			for(i=0;i<s;i++){
				q[i]/=t[i][r-1];
				for(j=0;j<r;j++){
					t[i][j]/=t[i][r-1];
				}
			}

			if(n2>n1){
				int b[n2-n1][r-1];
				for(i=n1+1;i<=n2;i++)
					for(j=0;j<r-1;j++)
						b[i][j]=t[i][j];
			}
			else
				b=-10000;
			if(n1>0){
				int B[n1][r-1];
				for(i=0;i<=n1;i++)
					for(j=0;j<r-1;j++)
						B[i][j]=t[i][j];
			}
			else
				B=10000;

			if(r==1){
				if (b>B)
					return 0;
				for(i=n2+1;i<s;i++)
					if(q[i]<0)
						return 0;
				return 1;
			}

			int sp=s-n2+n1*(n2-n1);
			if(sp<=0)
				return 1;
			s=sp;
			r=r-1;
			int t[sp][r];
			if(n2-n1>n1){
				for(i=0;i<n1;i++)
					for(j=0;j<(r-1);j++)
						t[i][j]=b[i][j]/B[i][j];
				for(i=n1;i<n2-n1;i++)
					for(j=0;j<(r-1);j++)
						t[i][j]=b[i][j];
			}
			else{
				for(i=0;i<n2-n1;i++)
					for(j=0;j<(r-1);j++)
						t[i][j]=b[i][j]/B[i][j];
				for(i=n2-n1;i<n1;i++)
					for(j=0;j<(r-1);j++)
						t[i][j]=b[i][j];
			}
			
		}
	}
}
