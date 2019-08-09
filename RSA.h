#pragma once
#include<iostream>
#include <string>
#include <vector>
#include<stdlib.h>
#include<time.h>
struct Key
{
	//公钥(ekey, pkey): (e,n)
	long pkey;
	long ekey;
	//私钥(dkey, pkey): (d, n)
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
	//加密单个信息
	long ecrept(long msg, long key, long pkey);//加密
	long produce_prime();//产生素数
	bool is_prime(long prime);//判断是否为素数
	void produce_keys();//存放公钥秘钥
	long produce_pkey(long prime1, long prime2);//pq->n
	long produce_orla(long prime1, long prime2);//(p-1)(q-1)
	long produce_ekey(long orla);//产生公钥
	long produce_gcd(long ekey, long orla);//互质
	long produce_dkey(long ekey, long orla);//产生秘钥

private:
	Key _key;
};
