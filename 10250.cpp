#include <iostream> 
#include <iomanip> 
#include <cstring>
#include<algorithm>
#include<cstdio>
#include<stack>
#include<queue>
/*
Name:ACM ȣ��
n��° �մ��� �ӹ��� ȣ�� ȣ���� ����Ѵ�.
Input: int cnt, int h, int w, int n
Output: int result
Example
cnt = 1
h = 6, w = 12, n = 10
���: 402

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
		result[num]= (((n - 1) % h + 1)*100) +((n - 1) / h + 1); //��*100 + ȣ��
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