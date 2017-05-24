/*
정사각형 만들기두 변의 길이가 모두 양의 정수인 직사각형 모양의 종이가 주어져 있다. 
이 종이를 칼로 여러 번 잘라서 모든 조각이 한 변의 길이가 양의 정수인 정사각형이 되도록 하고자 한다.
칼로 종이를 자르는 규칙은 다음과 같다.
1. 자르는 방향은 수직 또는 수평만 허용된다. 즉, 사선으로는 자를 수 없다.
2. 자르는 도중 칼의 방향을 바꿀 수 없다.
3. 자르는 도중에 칼을 멈출 수 없다. 즉, 일단 어떤 조각을 자르기 시작하면 그 조각이 반드시 둘로 분리될 때 까지 자른다.
4. 잘려진 조각의 각 변의 길이는 양의 정수이어 야 한다.

입출력 예)한 줄에 직사각형 변의 길이를 나타 내는 두 정수 n(1 ≤ n ≤ 10,000)과m (1 ≤ m ≤ 100)이 차례로 주어진다.
입력에서 주어진 변의 길이를 갖는 직사각형 모양의 종이를 제시한 규칙에따라 잘랐을 때 생긴 조각의 최소 개수를 한 줄에 출력한다.
입력은 input.txt, 출력은 화면(printf()) 출력
*/
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



						


					
					
			
		
	




