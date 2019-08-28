#include <iostream>
using namespace std;
template <class T>
struct AVLNode
{
	AVLNode(const T& val = T())
	:_left(nullptr)
	, _right(nullptr)
	, _parent(nullptr)
	, _data(val)
	, _bf(0)
	{}
	AVLNode<T>* _left;
	AVLNode<T>* _right;
	AVLNode<T>* _parent;
	
	T _data;
	int _bf;
};

template <class T>
class AVLTree
{
public:
	typedef AVLNode<T> Node;

	bool Insert(const T& val)
	{
		if (_root == nullptr)
		{
			_root = new Node(val);
			return true;
		}

		Node* cur = _root;
		Node* parent = nullptr;
		while (cur)
		{
			if (val > cur->_data)
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (val < cur->_data)
			{
				parent = cur;
				cur = cur->_left;
			}
			else
				return false;
		}

		cur = new Node(val);
		if (parent->_data > cur->_data)
			parent->_left = cur;
		else
			parent->_right = cur;
		cur->_parent = parent;

		//调平衡
		while (parent)
		{
			//更新平衡因子
			if (parent->_left == cur)
				--parent->_bf;
			else
				++parent->_bf;
			if (parent->_bf == 0)
				break;
			else if (parent->_bf == 1 || parent->_bf == -1)
			{
				//继续向上更新
				cur = parent;
				parent = parent->_parent;
			}
			else if (parent->_bf == -2 || parent->_bf == 2)
			{
				//旋转
				if (parent->_bf == -2 && cur->_bf == -1)
				{
					//右旋
					RotateR(parent);
				}
				else if (parent->_bf == 2 && cur->_bf == 1)
				{
					RotateL(parent);
				}
				else if (parent->_bf == 2 && cur->_bf == -1)
				{
					Node* subR = parent->_right;
					Node* subRL = subR->_left;
					int bf = subRL->_bf;
					RotateR(subR);
					RotateL(parent);

					if (bf == -1)
					{
						parent->_bf = 0;
						subR->_bf = 1;
					}
					else if (bf == 1)
					{
						parent->_bf = -1;
						subR->_bf = 0;
					}
				}
				else if (parent->_bf == -2 && cur->_bf == 1)
				{
					RotateL(cur);
					RotateR(parent);
				}

				break;
			}
		}
		return true;

	}

	void RotateL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		subR->_left = parent;
		parent->_right = subRL;

		if (subRL)
			subRL->_parent = parent;

		if (parent != _root)
		{
			Node* gParent = parent->_parent;
			if (gParent->_left == parent)
				gParent->_left = subR;
			else
				gParent->_right = subR;
			subR->_parent = gParent;
		}
		else
		{
			subR->_parent = nullptr;
			_root = subR;
		}

		parent->_parent = subR;

		subR->_bf = parent->_bf = 0;
	}

	void RotateR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		//单向链接 subL , parent, subLR
		subL->_right = parent;
		parent->_left = subLR;

		//向上链接subLR
		if (subLR)
			subLR->_parent = parent;
		
		//subL，parent->parent 双向链接
		if (parent != _root)
		{
			Node* gParent = parent->_parent;
			if (gParent->_left == parent)
				gParent->_left = subL;
			else
				gParent->_right = subL;
			subL->_parent = gParent;
		}
		else
		{
			_root = subL;
			subL->_parent = nullptr;
		}
		

		//向上链接parent, subL
		parent->_parent = subL;

		//更新平衡因子
		subL->_bf = parent->_bf = 0;
	}

	bool IsBalance()
	{
		return _IsBalance(_root);
	}

	void InOrder()
	{
		_Inorder(_root);
		cout << endl;
	}

	void _Inorder(Node* root)
	{
		if (root)
		{
			_Inorder(root->_left);
			cout << root->_data << " ";
			_Inorder(root->_right);
		}
	}

	int Height(Node* root)
	{
		if (root == nullptr)
			return 0;
		int left = Height(root->_left);
		int right = Height(root->_right);
		return left > right ? left + 1 : right + 1;
	}

	bool _IsBalance(Node* root)
	{
		if (root == nullptr)
			return true;

		int right = Height(root->_right);
		int left = Height(root->_left);

		if (root->_bf != right - left)
		{
			cout << "节点： " << root->_data << "异常" << endl;
			return false;
		}

		return abs(root->_bf) < 2 && _IsBalance(root->_left)
			&& _IsBalance(root->_right);
	}

private:
	Node* _root = nullptr;
};

//int main()
//{
//	AVLTree<int> avlt;
//	int arr[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 }; //{ 16, 3, 7, 11, 9, 26, 18, 14, 15 };
//	//{ 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 }; 
//	for (const auto& e : arr)
//	{
//		avlt.Insert(e);
//		cout << avlt.IsBalance() << endl;
//	}
//	avlt.InOrder();
//	//cout << avlt.IsBalance() << endl;
//	
//	return 0;
//}