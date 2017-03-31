#include <iostream> 
#include <iomanip> 
#include <cstring>
#define MAX_N 1000
using namespace std;
int x;
int main()
{
	int cnt = 0;
	cin >> x;
	if (x < 100)
	{
		cnt = x;
	}
	else if ((x>=100)&&(x<1000))
	{
		int result[3];
		int num;
		cnt = 99;
		for (int i = 100; i <= x; i++)
		{
			num = i;
			result[0] = num % 10;
			num = num / 10;
			result[1] = num % 10;
			num = num / 10;
			result[2] = num % 10;
			if ((result[0] - result[1]) == (result[1] - result[2]))
			{
				cnt++;
			}
		}
	}
	else if((1000<=x)&&(x<=10000))
	{
		cnt = 144;
		int result2[4];
		int num2;
		for (int i = 1000; i <= x; i++)
		{
			num2 = i;
			result2[0] = num2 % 10;
			num2 = num2 / 10;
			result2[1] = num2 % 10;
			num2 = num2 / 10;
			result2[2] = num2 % 10;
			num2 = num2 / 10;
			result2[3] = num2 % 10;
			if (((result2[0] - result2[1]) == (result2[1] - result2[2]))&& ((result2[1] - result2[2]) ==(result2[2]-result2[3])))
			{
				cnt++;
			}
		}

	}

	cout << cnt << endl;
	return 0;
}