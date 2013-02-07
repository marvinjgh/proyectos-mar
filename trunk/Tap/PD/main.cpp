#include <cstdio>
using namespace std;

#define MIN(A,B) A<B?A:B
#define INT_MAX 2147483647
#define FOR(q,n,s) for(int q = s; q < n; q++)

void suma(int *R,int *S,int n){
	FOR(i,n,0){
		S[i]=R[i]+R[(i+1)%n];
	}
}

int turco(int *S,int *B, int n, int p){
	int min = INT_MAX;
	int **m;
	m= new int*[p];
	FOR(i,p,0)
		m[i]=new int[n];

	FOR(k,n,0){
		FOR(i,p,0){
			FOR(j,n-1,i<<1){
				if(i==0){
					if(j == (i<<1)){
						m[i][j] = B[i]*(S[(j+k)%n]);
					}else{
						m[i][j] = MIN(m[i][j-1], B[i]*S[(j+k)%n]);
					}
					continue;
				}
				if(j==(i<<1)){
					m[i][j]=B[i]*S[(j+k)%n]+m[i-1][j-2];
				}else{
					m[i][j]=MIN(m[i][j-1],B[i]*S[(j+k)%n]+m[i-1][j-2]);
				}
			}
		}
		if(min > m[p-1][n-2]) 
			min = m[p-1][n-2];
	}

	FOR(i,p,0)
		delete(m[i]);
	delete(m);

	return min;
}

int main()
{
	int *r,*b,*s;
	int n, p;
	scanf("%d %d",&n,&p);
	while(n !=0 && p!=0)
	{
		r = new int[n];
		s = new int[n];
		b = new int[p];

		FOR(i,n,0)
			scanf("%d",&r[i]);

		FOR(i,p,0)
			scanf("%d",&b[i]);

		suma(r,s,n);

		printf("%d\n",-1*turco(s,b,n,p));
		scanf("%d %d",&n,&p);

		delete(r);
		delete(s);
		delete(b);
	}
	return 0;
}