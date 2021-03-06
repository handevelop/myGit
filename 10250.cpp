#include <iostream> 
#include <iomanip> 
#include <cstring>
#include<algorithm>
#include<cstdio>
#include<stack>
#include<queue>
/*
Name:ACM 호텔
n번째 손님이 머무를 호텔 호수를 출력한다.
Input: int cnt, int h, int w, int n
Output: int result
Example
cnt = 1
h = 6, w = 12, n = 10
결과: 402

DATE. 2017.3.30
Author by Handevelop.
*/
using namespace std;
int cnt;
int h;
int w;
int n;
int main()
{
	cin >> cnt;
	int cnt2 = cnt;
	int *result = new int[cnt];
	int num = 0;
	while (cnt)
	{
		cin >> h >> w >> n;
		result[num]= (((n - 1) % h + 1)*100) +((n - 1) / h + 1); //층*100 + 호수
		num++;
		cnt--;
	}
	num = 0;
	for (int i = 0; i < cnt2; i++)
	{
		cout << result[i] << endl;
	}
	return 0;
}