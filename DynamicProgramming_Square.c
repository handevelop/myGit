#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define OR ||
#define AND &&
#define PRN(x) printf("%d\n",x) //디버깅용 매크로
int min(int a,int b){
	if(a<b)
		return a;
	else
		return b;
}
int dt[10010][110];
int main(){
	FILE *ifp;
	if((ifp=fopen("input.txt","r"))==NULL)
	{
		printf("파일이 없습니다.");
		return 0;
	}
	int n1,n2;
	fscanf(ifp,"%d",&n1);
	fscanf(ifp,"%d",&n2);
	
	//n1와 n2를 비교해 더 큰 변 찾기
	if(n1<n2)
	{
		int temp=n1;
		n1=n2;
		n2=temp;
	}
	for(int i=1;i<=n1;i++){
		for(int j=1;j<=n2;j++){
			dt[i][j]=1e9;
		}
	}
	for(int i=1;i<=n1;i++){
		for(int j=1;j<=n2;j++){
			if(i==j){
				dt[i][j]=1;
			}else if(3*i>=j*j){
				dt[i][j]=dt[i-j][j]+1;
			}else{
				for(int k=1;k<i;k++){
					dt[i][j]=min(dt[i][j],dt[k][j]+dt[i-k][j]);
				}
				for(int k=1;k<j;k++){
					dt[i][j]=min(dt[i][j],dt[i][k]+dt[i][j-k]);
				}
			}
		}
	}
	printf("%d",dt[n1][n2]);
	return 1;

}



						


					
					
			
		
	




