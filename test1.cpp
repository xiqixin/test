//括号匹配
class parenthesis {
public:
	bool chkparenthesis(string a, int n) {
		// write code here
		if (n % 2 != 0)
		{
			return false;
		}
		int count = 0;
		for (auto& e : a)
		{
			if (e == '(')
			{
				count++;
			}
			else if (e == ')')
			{
				count--;
			}
			else {
				return false;
			}
		}
		if (count == 0)
		{
			return true;
		}
		return false;
	}
};
