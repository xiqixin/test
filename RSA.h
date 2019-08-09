#pragma once
#include<iostream>
#include <string>
#include <vector>
#include<stdlib.h>
#include<time.h>
struct Key
{
	//��Կ(ekey, pkey): (e,n)
	long pkey;
	long ekey;
	//˽Կ(dkey, pkey): (d, n)
	long dkey;
};

class RSA
{
public:
	RSA();
	Key getKey()
	{
		return _key;
	}
	void ecrept(const char* plain_file_in, const char* ecrept_file_out,
		long ekey, long pkey);
	void decrept(const char* ecrept_file_in, const char* plain_file_out,
		long dkey, long pkey);

	std::vector<long> ecrept(std::string& str_in, long ekey, long pkey);
	std::string decrept(std::vector<long>& ecrept_str, long dkey, long pkey);

	void printInfo(std::vector<long>& ecrept_str);
private:
	//���ܵ�����Ϣ
	long ecrept(long msg, long key, long pkey);//����
	long produce_prime();//��������
	bool is_prime(long prime);//�ж��Ƿ�Ϊ����
	void produce_keys();//��Ź�Կ��Կ
	long produce_pkey(long prime1, long prime2);//pq->n
	long produce_orla(long prime1, long prime2);//(p-1)(q-1)
	long produce_ekey(long orla);//������Կ
	long produce_gcd(long ekey, long orla);//����
	long produce_dkey(long ekey, long orla);//������Կ

private:
	Key _key;
};
