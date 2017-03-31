
#include <iostream> 
#include <iomanip> 
#include <cstring>
#include<algorithm>
#include<cstdio>
#include<stack>
#include<queue>
/*
	Name:SUBSET SUM Problem
	정수 a1,a2,...,an이 주어져 있을 때 몇개의 수를 합쳐 그 합이 k가 되면 Yes를 출력한다.
	Input: int n, arr a, int k
	Output: Yes or No
	Example
	n = 4
	a = {1, 2, 4, 7}
	k = 13
	결과: Yes
	
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
