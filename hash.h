#pragma once
#include <vector>
using namespace std;

namespace CLOSED
{

	//��ɢ��
	enum STATE { EMPTY, EXIST, DELETE };

	template <class K, class V>
	struct HashNode
	{
		pair<K, V> _kv;
		STATE _state = EMPTY;
	};

	template <class K, class V>
	class HashTable
	{
	public:
		typedef HashNode<K, V> Node;
		typedef Node* pNode;
		HashTable(size_t N = 1)
		{
			_table.resize(N);  // _table[]
			_size = 0;
		}

		bool Insert(const pair<K, V>& kv)
		{
			CheckCapacity();
			size_t index = kv.first % _table.size();
			while (_table[index]._state == EXIST)
			{
				//����Ƿ�key�Ѿ�����
				if (_table[index]._kv.first == kv.first)
					return false;

				++index;
				//�ߵ�ĩβ����ͷ��ʼ����
				if (index == _table.size())
					index = 0;
			}

			_table[index]._kv = kv;
			_table[index]._state = EXIST;
			++_size;
			return true;
		}

		void CheckCapacity()
		{
			//��������
			if (_size * 10 / _table.size() >= 7)  // 5   3,4
			{
				size_t newC = _table.size() == 0 ? 1 : 2 * _table.size();
				//_table.resize(newC);
				//vector<Node> newT;
				//newT.resize(newC);
				HashTable<K, V> newHt(newC);
				for (size_t i = 0; i < _table.size(); i++)
				{
					if (_table[i]._state == EXIST)
					{
						newHt.Insert(_table[i]._kv);
					}
				}
				_table.swap(newHt._table);
			}
		}
		// (1, 0) (2,0)  (3,0)
		pNode Find(const K& key)
		{
			size_t index = key % _table.size();
			while (_table[index]._state != EMPTY)
			{
				if (_table[index]._state == EXIST
					&& _table[index]._kv.first == key)
					return &_table[index];

				++index;
				if (index == _table.size())
					index = 0;
			}

			return nullptr;

		}

		bool Erase(const K& key)
		{
			pNode ret = Find(key);
			if (ret)
			{
				ret->_state = DELETE;
				--_size;
				return true;
			}
			return false;
		}
	private:
		vector<Node> _table;   //new Node[N]
		size_t _size;   // ��ЧԪ�ص�����

	};

}

//��ϣͰ
template <class V>
struct HashNode
{
	V _data;
	HashNode<V>* _next;

	HashNode(const V& data)
		:_data(data)
		, _next(nullptr)
	{}
};

template <class K, class V, class KeyOfValue>
struct __HIerator
{
	typedef HashNode<V> Node;
	typedef Node* pNode;

	typedef __HIerator<K, V, KeyOfValue> Self;
	typedef HashTable<K, V, KeyOfValue> HashT;

	HashT* _ht;
	pNode _node;

	__HIerator(pNode node, HashT* ht)
		:_node(node)
		, _ht(ht)
	{}

	V& operator*()
	{
		return _node->_data;
	}

	V* operator->()
	{
		return &operator*();
	}

	bool operator!=(const Self& it)
	{
		return _node != it._node;
	}

	Self& operator++()
	{
		if (_node->_next)
			_node = _node->_next;
		else
		{
			//�ҵ���һ���ǿ������ͷ�ڵ�
			//���㵱ǰ_node���ڵ�λ��
			KeyOfValue kov;
			size_t index = kov(_node->_data) % _ht->_table.size();
			//��_node���ڵ�λ�õ���һ���ط�����
			++index;
			for (; index < _ht->_table.size(); ++index)
			{
				//�ҵ��ǿ������ͷ�ڵ�
				if (_ht->_table[index])
				{
					_node = _ht->_table[index];
					break;
				}	
			}
			//�Ⱥ����֮��û���ҵ��κ�һ���ǿսڵ�
			if (index == _ht->_table.size())
				_node = nullptr;
		}
		return *this;
	}
};

template <class K, class V, class KeyOfValue>
class HashTable
{
public:
	typedef HashNode<V> Node;
	typedef Node* pNode;
	typedef __HIerator<K, V, KeyOfValue> iterator;

	iterator begin()
	{
		//��һ���ǿ������ͷ�ڵ�
		for (size_t i = 0; i < _table.size(); ++i)
		{
			if (_table[i])
				return iterator(_table[i], this);
		}
		return iterator(nullptr, this);
	}

	iterator end()
	{
		return iterator(nullptr, this);
	}

	bool Insert(const V& data)
	{
		CheckCapacity();
		//ͨ���º��������ȡdata��key
		//ͨ��key����index
		KeyOfValue kov;
		size_t index = kov(data) % _table.size();
		pNode cur = _table[index];
		//������ͷ��ʼ����
		while (cur)
		{
			if (kov(cur->_data) == kov(data))
				return false;
			cur = cur->_next;
		}

		cur = new Node(data);
		//ͷ��
		cur->_next = _table[index];
		_table[index] = cur;

		++_size;
		return true;
	}

	void CheckCapacity()
	{
		if (_size == _table.size())
		{
			KeyOfValue kov;
			size_t newC = _table.size() == 0 ? 1 : 2 * _table.size();
			vector<pNode> newT;
			newT.resize(newC);
			for (size_t i = 0; i < _table.size(); ++i)
			{
				pNode cur = _table[i];
				//���������ѽڵ���ص��µı���
				while (cur)
				{
					pNode next = cur->_next;
					//����ÿһ���ڵ����±��е�λ��
					size_t index = kov(cur->_data) % newT.size();
					//ͷ��
					cur->_next = newT[index];
					newT[index] = cur;
					cur = next;
				}
				_table[i] = nullptr;
			}
			_table.swap(newT);
		}
	}

private:
	size_t _size = 0;
	vector<pNode> _table;
};