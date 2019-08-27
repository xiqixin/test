#include "RSA.h"
#include <time.h>
#include <iostream>
#include <fstream>
RSA::RSA()
{
	produce_keys();
}
void RSA::ecrept(const char* plain_file_in, const char* ecrept_file_out,
	bm::int1024_t ekey, bm::int1024_t pkey)//加密
{
	std::ifstream fin(plain_file_in);
	std::ofstream fout(ecrept_file_out,std::ofstream::app);
	if (!fin.is_open())
	{
		std::cout << "open file failed 1" << std::endl;
		return;
	}
	const int NUM = 256;
	char buffer[NUM];
	bm::int1024_t buffer_out[NUM];
	int curNum;
	//打开文件，按块读取，逐段加密
	while (!fin.eof())
	{
		fin.read(buffer, NUM);
		curNum = fin.gcount();//返回当前读了多少个字节数
		for (int i = 0; i < curNum; ++i)
		{
			buffer_out[i] = ecrept((bm::int1024_t)buffer[i], ekey, pkey);
		}
		fout.write((char*)buffer_out, curNum * sizeof(bm::int1024_t));
	}
	fin.close();
	fout.close();
}
void RSA::decrept(const char* ecrept_file_in, const char* plain_file_out,
	bm::int1024_t dkey, bm::int1024_t pkey)//解密
{
	std::ifstream fin(ecrept_file_in);
	std::ofstream fout(plain_file_out, std::ofstream::app);
	if (!fin.is_open())
	{
		std::cout << "open file failed 2" << std::endl;
		return;
	}
	const int NUM = 256;
	bm::int1024_t buffer[NUM];
	char buffer_out[NUM];
	int curNum;
	//打开文件，按块读取，逐段解密
	while (!fin.eof())
	{
		fin.read((char*)buffer, NUM*sizeof(bm::int1024_t));
		curNum = fin.gcount();//返回当前读了多少个字节数
		curNum /= sizeof(bm::int1024_t);
		for (int i = 0; i < curNum; ++i)
		{
			buffer_out[i] = (char)ecrept(buffer[i], dkey, pkey);
		}
		fout.write(buffer_out, curNum);
	}
	fin.close();
	fout.close();
}
std::vector<bm::int1024_t> RSA::ecrept(std::string& str_in, bm::int1024_t ekey, bm::int1024_t pkey)
{
	//字符串加密
	std::vector<bm::int1024_t> vecout;
	for (const auto& e : str_in)
	{
		vecout.push_back(ecrept(e, ekey, pkey));
	}
	return vecout;
}
std::string RSA::decrept(std::vector<bm::int1024_t>& ercept_str, bm::int1024_t dkey, bm::int1024_t pkey)
{
	//字符串解密
	std::string strout;
	for (const auto& e: ercept_str)
	{
		strout.push_back((char)ecrept(e, dkey, pkey));
	}
	return strout;
}
void RSA::printInfo(std::vector<bm::int1024_t>& ecrept_str)
{
	for (const auto& e : ecrept_str)
	{
		std::cout << e << "";
	}
	std::cout << std::endl;
}

//加密单个信息--模幂运算
bm::int1024_t RSA::ecrept(bm::int1024_t msg, bm::int1024_t key, bm::int1024_t pkey)
{
	bm::int1024_t msg_out = 1;
	bm::int1024_t a = msg;
	bm::int1024_t b = key;
	bm::int1024_t c = pkey;
	while (b)
	{
		if (b & 1)
			//msg_out = (A0*A1*...Ai...An)%c
			msg_out = (msg_out*a) % c;
		b >>= 1;
		//Ai=(A(i-1)*A(i-1))%c
		a = (a*a) % c;
	}
	return msg_out;
}
bm::int1024_t RSA::produce_prime()//产生素数
{
	srand(time(nullptr));
	bm::int1024_t prime = 0;
	while (1)
	{
		prime = rand() % 50 + 2;
		if (is_prime(prime))
			break;
	}
	return prime;
}
bool RSA::is_prime(bm::int1024_t prime)//判断是否为素数
{
	if (prime < 2)
		return false;
	for (int i = 2; i <= sqrt(prime); ++i)
	{
		if (prime%i == 0)
			return false;
	}
	return true;
}
void RSA::produce_keys()//存放公钥密钥
{
	bm::int1024_t prime1 = produce_prime();
	bm::int1024_t prime2 = produce_prime();
	while (prime1 == prime2)
		prime2 = produce_prime();

	_key.pkey = produce_pkey(prime1, prime2);//产生 n-->p*q
	bm::int1024_t orla = produce_orla(prime1, prime2);//产生(p-1)(q-1)
	_key.ekey = produce_ekey(orla);//产生公钥
	_key.dkey = produce_dkey(_key.ekey, orla);//产生密钥
}
bm::int1024_t RSA::produce_pkey(bm::int1024_t prime1, bm::int1024_t prime2)// n-->p*q
{
	return  prime1 * prime2;
}
bm::int1024_t RSA::produce_orla(bm::int1024_t prime1, bm::int1024_t prime2)//(p-1)(q-1)
{
	return (prime1 - 1)*(prime2 - 1);
}
bm::int1024_t RSA::produce_ekey(bm::int1024_t orla)//产生公钥
{
	bm::int1024_t ekey;
	srand(time(nullptr));
	while (1)
	{
		ekey = rand() % orla;
		if (ekey > 1 && produce_gcd(ekey, orla) == 1)
			break;
	}
	return ekey;
}
bm::int1024_t RSA::produce_gcd(bm::int1024_t ekey, bm::int1024_t orla)//判断是否互质
{
	//gcd(a , b) = gcd(b , a%b)
	bm::int1024_t residual;
	while (residual = ekey % orla)
	{
		ekey = orla;
		orla = residual;
	}
	return orla;
}
bm::int1024_t RSA::produce_dkey(bm::int1024_t ekey, bm::int1024_t orla)//产生密钥
{
	bm::int1024_t dkey = orla / ekey;
	while (1)
	{
		if ((dkey * ekey) % orla == 1)
			break;

		++dkey;
	}
	return dkey;
}


