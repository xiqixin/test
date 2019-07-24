#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

template <class T>
struct ListNode {
	ListNode<T>* _prev;
	ListNode<T>* _next;
	T _data;
};

template<class T,class Ref,class Ptr>
class ListIterator {
	typedef ListNode<T>* PNode;//结构体类型重定义，结点的类型
	typedef ListIterator<T, Ref, Ptr> Self;
public:
	ListIterator(PNode pNode = nullptr)
		:_pNode(pNode)
	{}
	ListIterator(const Self& l)
		:_pNode(l._pNode)
	{}
	T& operator*()
	{
		return _pNode->_data;
	}
	T* operator->()
	{
		//return &(_pNode->_data)
		return &(operator*())
	}
	Self& operator++()
	{
		_pNode = _pNode->_next;
		return *this;
	}
	Self operator++(int)
	{
		Self tmp(*this);
		_pNode = _pNode->_next;
		return *this;
	}
	bool operator!=(const Self& l)
	{
		return _pNode != l._pNode;
	}
	bool operator==(const Self& l)
	{
		return _pNode == l._pNode;
	}
	PNode _pNode;
};


template<class T>
class List {
	typedef ListNode<T> Node;
	typedef Node* PNode;
	List()
	{
	    _head = new Node;
		_head->_prev = _head;
		_head->_data = _head;
	}
	//这里要实现深拷贝
	List(const List<T>& lst)
		:_head(lst._head)
	{
		_head = new Node;
		_head->_prev = _head;
		_head->_next = _head;
	    for (auto& e : lst)
		{
				PushBack(e);
		}
		
	}
	List<T>& operator=(const List<T> lst)
	{
		swap(_head, lst._head);//内部调用拷贝构造
		return *this;
	}
	void clear()
	{

	}
	size_t size()const
	{
		assert(_head != NULL);
		size_t count = 0;
		/*PNode cur=_head->_next;
		while (cur!= _head)
		{
		    cur=cur->_next;
			count++;
		}*/
		for (auto& e : *this)
			count++;
		return count;
	}
	bool empty()const
	{
		return _head->_next == _head;
	}
	void PushFront(const T& val)
	{
		PNode node = new Node(val);
		node->_next = _head->_next;
		node->_next->_prev = node;
		_head->_next = node;
		node->_prev = _head;
	}
	void PushBack(const T& val)
	{
		PNode node = new Node(val);
		node->_next = _head;
		_head->_prev->_next = node;
		node->_prev = _head->_prev;
		_head->_prev = node;
	}
	void PopBack()
	{
		PNode cur = _head->_prev;
		if (cur != _head)
		{
			cur->_prev->_next = _head;
			_head->_prev = cur->_prev;
			delete cur;
		}
	}
	//头删
	void PopFront()
	{
		if (_head->_next != _head)
		{
			PNode cur = _head->_next;
			_head->_next = cur->_next;
			cur->_next->_prev = _head;
			delete cur;
		}
	}
private:
	PNode _head;
};
