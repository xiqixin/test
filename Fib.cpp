//输出从from到to的斐波那契数列的和
#include <iostream>
#include <vector>
using namespace std;
int main()
{
	int from, to;
	while (cin >> from >> to)
	{
		vector<long> v;
		v.push_back(1);
		v.push_back(1);
		long long sum=0;
		for (int i = 2; i < to; i++)
		{
			v.push_back(v[i - 1] + v[i - 2]);
		}
		if ((from == 1 && to == 1) || (from == 2 && to == 2))
		{
			cout << 1 << endl;
		}
		else if (from == 1 && to == 2) {
			cout << 2 << endl;
		}
		else {
			for (int i = from - 1; i < to; i++)
			{
				sum += v[i];
			}
			cout << sum << endl;
		}

	}
}
