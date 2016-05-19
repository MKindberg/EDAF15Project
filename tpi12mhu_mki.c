#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>

static unsigned long long	fm_count;
static volatile bool		proceed = false;


static void done(int unused)
{
	proceed = false;
	unused = unused;
}

int fm_elim(int n, int m, int A[n][m], int* c);

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

	char ch;
	int rows, cols;
	int i, j, n;
	rows = fgetc(afile)-'0';
	fgetc(afile);
	cols = fgetc(afile)-'0';
	fgetc(afile);

	int a[rows][cols];
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

	int c[rows];

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

/*for(int i=0;i<rows;i++){
	printf("%d |", c[i]);
	for(int j=0;j<cols;j++){
		printf("%d ", a[i][j]);
	}
	printf("\n");
}*/
	fm_elim(rows, cols, a, c);
	if (seconds == 0) {
		/* Just run once for validation. */
		// Uncomment when your function and variables exist...
		return fm_elim(rows, cols, a, c);
	}

	/* Tell operating system to call function DONE when an ALARM comes. */
	signal(SIGALRM, done);
	alarm(seconds);

	/* Now loop until the alarm comes... */
	proceed = true;
	while (proceed) {
	// Uncomment when your function and variables exist...
	fm_elim(rows, cols, a, c);

	fm_count++;
}

return fm_count;
}

void swap_rows(int cols, double** a, double* c, int i, int j)
{
	double temp;
	for(int k=0;k<cols;k++){
		temp=a[i][k];
		a[i][k]=a[j][k];
		a[j][k]=temp;
	}

	temp = c[i];
	c[i]=c[j];
	c[j]=temp;

}


void sort_rows(int rows, int cols, double** a, double* c)
{
	int i, j, k=-1;
	for(i=0;i<rows;i++){
		for(j=0;j<i+1;j++){
			if(a[i][cols-1]>a[j][cols-1]){
				swap_rows(cols, a, c, i, j);
			}
		}
	}

	for(i=0;i<rows;i++){
		if(a[i][cols-1]<0.00001){
			k=i;
			break;
		}
	}

if(k!=-1)
	for(i=k;i<rows;i++)
		for(j=k;j<i+1;j++)
			if(a[i][cols-1]<a[j][cols-1])
				swap_rows(cols, a, c, i, j);

}


double min(double** t, double* q, int n1, int n2){
	int i;
	double min=1000000;
	for(i=n1;i<n2;i++){
		if(t[i][0]+q[i]<min)
			min=t[i][0]+q[i];
	}

	return min;
}

double max(double** t, double* q, int n1, int n2){
	int i;
	double max=-1000000;
	for(i=n1;i<n2;i++){
		if(t[i][0]+q[i]>max)
			max=t[i][0]+q[i];
	}

	return max;
}

int fm_elim(int n, int m, int A[n][m], int* c)
{

	int n1=0, n2=0, i, j;
	int s = n;
	int r = m;
	double** t = malloc(n*sizeof(double*));
	for(i=0;i<n;i++)
		t[i]=malloc(m*sizeof(double));

	double* q = malloc(n*sizeof(double));
	
	for(i=0;i<n;i++){
		q[i] = (double) c[i];
		for(j=0;j<m;j++)
		t[i][j] = (double) A[i][j];
	}
	/*printf("\n\nq:\n");
	for(i=0;i<s;i++){
		printf("%f \n", q[i]);}
		for(j=0;j<r;j++){
				printf("%f ", t[i][j]);
		}
		printf("\n");
	}
	printf("\n");
sort_rows(s, r, t, q);
printf("\nsorted\n");
for(i=0;i<s;i++){
		//printf("%f ", q[i]);
		for(j=0;j<r;j++){
				printf("%f ", t[i][j]);
		}
		printf("\n");
	}
	printf("\n");*/
int b, B;
while(1){
	n1 = 0;
	n2 = 0;
	for (i=0;i<s;i++){
		if(t[i][r-1]>0.00001)
			n1++;
		else if(t[i][r-1]<0)
			n2++;
	}
	n2+=n1;
	sort_rows(s, r, t, q);

	for(i=0;i<r-1;i++){
		for(j=0;j<n2;j++){
			t[j][i]=t[j][i]/t[j][r-1];
		}
	}
	for(j=0;j<n2;j++){
		q[j]=q[j]/t[j][r-1];
	}

	
		if(n2>n1){
			b=max(t, q, n1+1, n2);
		}
		else{
			b=-1000000;
		}
		if(n1>0){
			B=min(t, q, n1+1, n2);
		}
		else{
			B=1000000;
		}
		
		
	if(r==1){
		if(b>B){
			return 0;
		}
	for(i=n2+1;i<s;i++)
		if(q[i]<0)
			return 0;
		return 1;
	}

	int sp=s-n2+n1*(n2-n1);
	if(sp==0)
		return 1;

	r=r-1;
	double D[sp][r];
	double d[sp];
	int k;
	for(i=0;i<n1;i++){
		for(j=n1;j<n2;j++){
			d[i*(n2-n1)+j-n1]=q[i]-q[j];
			for(k=0;k<r;k++){
				D[i*(n2-n1)+j-n1][k]=t[i][k]-t[j][k];
			}
		}
	}
	for(i=n2;i<s;i++){
		d[n1*(n2-n1)+i-n2]=q[i];
		for(k=0;k<r;k++){
			D[n1*(n2-n1)+i-n2][k]=t[i][k];
		}
	}
	q=realloc(q, s*sizeof(double));
	t=realloc(t, s*sizeof(double*));
	for(i=0;i<s;i++){
		t[i]=realloc(t[i], r*sizeof(double));
		q[i]=d[i];
		for(j=0;j<r;j++){
			t[i][j]=D[i][j];
		}
	}
}
}


/*int main(int argc, char** argv)
{
	tpi12mhu_mki("/h/d8/o/tpi12mki/Desktop/project/input/1/A","/h/d8/o/tpi12mki/Desktop/project/input/1/c",3);

}
*/
