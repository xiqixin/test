#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stack>
#include <list>
using namespace std;

template <class T, class container>
class Stack {
public:
	Stack() {};
	void Push(const T& x)
	{
		con.push_back(x);
	}
	void Pop()
	{
		con.pop_back();
	}
	T& Top()
	{
		return con.back();
	}
	const T& Top()const
	{
		return con.back();
	}
	size_t Size()
	{
		return con.size();
	}
	bool Empty()
	{
		return con.empty();
	}
private:
	container con;
};
int main()
{
	Stack<int, list<int>> s;
	s.Push(1);
	system("pause");
	return 0;
}