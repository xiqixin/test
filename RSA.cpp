#include"RSA.h"


void RSA::ecrept(const char* plain_file_in, const char* ecrept_file_out,
	long ekey, long pkey)
{

}

void RSA::decrept(const char* ecrept_file_in, const char* plain_file_out,
	long dkey, long pkey)
{

}

std::vector<long> RSA::ecrept(std::string& str_in, long ekey, long pkey)//对字符串加密
{
	std::vector<long> vec_out;
	for (const auto& e : str_in)
	{
		vec_out.push_back(ecrept(e, ekey, pkey));//将加密的信息放入到vec_out中
	}
	return vec_out;
}

std::string RSA::decrept(std::vector<long>& ecrept_str, long dkey, long pkey)//对加密的字符串解密
{
	std::string str_out;
	for (const auto& e : ecrept_str)
	{
		str_out.push_back((char)ecrept(e, dkey, pkey));//将解密的字符放入到str_out中
	}
	return str_out;
}

long RSA::ecrept(long msg, long key, long pkey)//加密   msg：需要加密的信息
{
	long msg_out = 1;
	long a = msg;
	long b = key;
	int c = pkey;
	while (b)
	{
		if (b & 1)
		{
			msg_out = (msg_out * a) % c;
		}
		b >> 1;
		a = (a * a) % c;
	}
	return msg_out;
}

long RSA::produce_prime()//产生素数
{
	srand(time(nullptr));
	long prime = 0;
	while (1)
	{
		prime = rand() % 30 + 2;//产生一个大于2的素数
		if (is_prime(prime))
		{
			break;
		}
	}
	return prime;
}

bool RSA::is_prime(long prime)//判断是否为素数
{
	if (prime < 2)
	{
		return false;
	}
	for(int i = 2; i <= sqrt(prime); i++)
	{
		if (prime % i == 0)
		{
			return false;
		}
	}
	return true;
}

void RSA::produce_keys()//存放公钥秘钥
{
	long prime1 = produce_prime();
	long prime2 = produce_prime();
	while (prime1 == prime2)
	{
		prime2 = produce_prime();
	}

	_key.pkey = produce_pkey(prime1, prime2);
	long orla = produce_orla(prime1, prime2);
	_key.ekey = produce_ekey(orla);
	_key.dkey = produce_dkey(_key.ekey, orla);
}

long RSA::produce_pkey(long prime1, long prime2)//pq->n
{
	return prime1 * prime2;
}

long RSA::produce_orla(long prime1, long prime2)//(p-1)(q-1)
{
	return (prime1 - 1) * (prime2 - 1);

}

long RSA::produce_ekey(long orla)//产生公钥
{
	long ekey;
	srand(time(nullptr));
	while (1)
	{
		ekey = rand() % orla;
		if (ekey > 1 && produce_gcd(ekey, orla) == 1)
		{
			break;
		}
	}
	return ekey;

}

long RSA::produce_gcd(long ekey, long orla)//公钥要和oral互质
{
	long residual;
	while (residual = ekey % orla)
	{
		ekey = orla;
		orla = ekey % orla;
	}
	return orla;
}

long RSA::produce_dkey(long ekey, long orla)//产生秘钥
{
	long dkey = orla / ekey;
	while (1)
	{
		if ((dkey * ekey) % orla == 1)
		{
			break;
		}
		++dkey;
	}
	return dkey;
}

void RSA::printInfo(std::vector<long>& ecrept_str)
{
	for (const auto& e : ecrept_str)
	{
		std::cout << e << " ";
	}
	std::cout << std::endl;
}


