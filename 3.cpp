/*
#include <iostream> 
#include <iomanip> 
#include <cstring>
#include<algorithm>
#include<cstdio>
#include<stack>
#include<queue>
/*
	Name:SUBSET SUM Problem
	���� a1,a2,...,an�� �־��� ���� �� ��� ���� ���� �� ���� k�� �Ǹ� Yes�� ����Ѵ�.
	Input: int n, arr a, int k
	Output: Yes or No
	Example
	n = 4
	a = {1, 2, 4, 7}
	k = 13
	���: Yes
	
	DATE. 2017.3.28 
	Author by Handevelop.

using namespace std;
int n, k;
int *a;
bool isSolve(int k, int *a, int i, int sum)
{
	if (i == n)
		return sum == k;
	if (isSolve(k, a, i + 1, sum))
		return true;
	if (isSolve(k, a, i + 1, sum + a[i]))
		return true;
	
	return false;
}
int main()
{
	cin >> n;
	a = new int[n];
	for (int i = 0; i < n; i++)
	{
		cin >> a[i];
	}
	cin >> k;
	
	if (isSolve(k, a,0,0))
		cout << "Yes" << endl;
	else
		cout << "No" << endl;
	return 0;
}
*/