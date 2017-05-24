/*
2차원 평면 상에서 N개 직사각형(1<=N<=1,000,000)의 좌하(x1, y1) 우상(x2, y2) 좌표가 실수로 주어진다(0<=x1<x2, 0<=y1<y2). 
좌표평면 상에서 이 직사각형들이 차지하는공간의 면적을 구하라.
- 입출력 예)입력은 input.txt 파일을 읽고, 출력은 화면(printf())에 출력할 것입력의 첫 줄은 직사각형의 개수 N이다.
출력은 소수점 둘째 자리까지 출력한다.
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 1000000

//실수형 배열을 정렬하는 함수(퀵정렬 이용)
void *Sort(double *arr,int start,int end){
	double temp;
	int i=start;
	int j=end;
	double pivot=arr[(start+end)/2];
	while(i<=j){
		while(arr[i]<pivot){
			i++;
		}
		while(arr[j]>pivot)
		{
			j--;
		}
		if(i<=j)
		{
			temp=arr[i];
			arr[i]=arr[j];
			arr[j]=temp;
			i++;
			j--;
		}
	}
	if(start<j)
		Sort(arr,start,j);
	if(i<end)
		Sort(arr,i,end);
}
double *isOverlap(double *Y,double Y1,double Y2){
	int i=0;
	for(i=0;Y[i]!=-1;i+=2){//-1이 아닐때까지.즉, Y좌표가 입력이 되어있는곳만
		if(Y[i]<Y1&&Y[i+1]>Y2)
		{
			return Y;
		}
		else if(Y[i]<Y1&&Y[i+1]>Y1)
		{
			Y[i+1]=Y2;
			return Y;
		}
		else if(Y[i]<Y2&&Y[i+1]>Y2)
		{
			Y[i]=Y1;
			return Y;
		}
	}
	
	Y[i]=Y1;
	Y[i+1]=Y2;//사각형이 겹치지 않으면 새로 추가
	return Y;
}
int main(){
	double **arr;
	double *X;
	double *Y;
	int *visit;
	double Height=0;
	double Area=0;
	FILE *fp;
	if((fp=fopen("input.txt","r"))==NULL){
		printf("파일이 없습니다.\n");
		return -1;
	}
	int i,j,k,n;
	//N 구하기
	fscanf(fp,"%d",&n);
	//printf("%d ",n);
	//N만큼 동적할당
	arr=(double**)malloc(sizeof(double*)*n);
	//2차원 배열 꼴로 동적할당
	for(i=0;i<=n;i++){
		arr[i]=(double*)malloc(sizeof(double)*4);
	}
	//visit배열 동적할당
	visit=(int*)malloc(sizeof(int)*n);
	//X,Y 좌표 넣을 곳 동적할당
	X=(double*)malloc(sizeof(double)*n*2);
	Y=(double*)malloc(sizeof(double)*n*2);
	//배열에 좌표값 입력
	for(i=0;i<n;i++){
		for(j=0;j<4;j++){
			fscanf(fp,"%lf",&arr[i][j]);
		}
	}
	/*for(i=0;i<n;i++){
		for(j=0;j<4;j++){
			printf("%lf ",arr[i][j]);
		}
	}*/
	//겹치는 부분을 제외한 X를 구하기
	int Xcnt=0;
	for(i=0;i<n;i++){
		for(j=0;j<3;j+=2){
			int flag=0;//X에 있는지 확인하기 위한 임시변수
			for(k=0;k<Xcnt;k++)
				if(arr[i][j]==X[k])
					flag=1;
			if(flag==0)
				X[Xcnt++]=arr[i][j];
		}
	}
	//X좌표를 퀵정렬을 이용해 오름차순으로 정렬
	Sort(X,0,Xcnt-1);
	/*for(i=0;i<Xcnt;i++)
		printf("%lf ",X[i]);*/
	for(i=0;i<Xcnt-1;i++){
		for(j=0;j<n;j++)
			visit[j]=0;//겹치는 사각형을 나타내는 visit 배열을 0으로 초기화
		for(j=0;j<n*2;j++)
			Y[j]=-1;//Y좌표 값을 -1로 초기화
		for(j=0;j<n;j++){
			if(arr[j][0]<=X[i]&&arr[j][2]>X[i])//X[i]가 다른사각형의 X좌표 사이에 있을 경우
				visit[j]=1;//해당 사각형의 인덱스를 1로 바꿈
		}
		int cnt=0;
		for(j=0;j<n;j++){
			if(visit[j]==1){//사각형이 포함되어있는 사각형일 경우
				if(cnt==0){//Y 초기 좌표값 넣기 위한 작업
					Y[0]=arr[j][1];//좌측 하단 Y좌표
					Y[1]=arr[j][3];//우측 상당 Y좌표
					cnt++;//초기값으로 오지 않기 위해 ++ 해줌
				}
				else
					isOverlap(Y,arr[j][1],arr[j][3]);//겹치는지 확인
			}
		}
		for(j=0;Y[j]!=-1;j+=2)
			Height+=Y[j+1]-Y[j];
		Area+=Height*(X[i+1]-X[i]);
		Height=0;
	}
	//최종 넓이 계산
	/*for(i=0;i<Xcnt-1;i++){
		for(j=0;Y[j]!=-1;j+=2){
			Height=Y[j+1]-Y[j];
		}
		Area=Area+Height*(X[i+1]-X[i]);
	}*/
	printf("%.2lf\n",Area);
	return 0;
}
		
