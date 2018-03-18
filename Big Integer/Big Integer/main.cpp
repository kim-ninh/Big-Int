#include "BigInt.h"
#include <algorithm>
#include <random>
#include <fstream>

#define OUTPUT "output.txt"

std::string dec2IntStr(std::string decimalStr) {
	size_t len = decimalStr.find('.');
	return decimalStr.substr(0, len);
}

void basicTest01(std::ostream &os)
{
	os << (BigInt("-1") << 128) << '\n';
	os << (BigInt("-1") << 1) << '\n';
	os << (BigInt("1") << 9) << '\n';
	os << (BigInt(BigIntMAX) << 1) << '\n';
	os << (BigInt(dec2IntStr(std::to_string(pow(2,126)))) << 1) << '\n';
	
	os << (BigInt("-1") >> 128) << '\n';
	os << (BigInt("-1") >> 1) << '\n';
	os << (BigInt(BigIntMIN) >> 1) << '\n';
	os << (BigInt("128") >> 1) << '\n';
	os << (BigInt("-2") >> 1) << '\n';

	(BigInt("-1") << 128).ShowBit(os); os << '\n';
	(BigInt("-1") << 1).ShowBit(os); os << '\n';
	(BigInt("1") << 9).ShowBit(os); os << '\n';
	(BigInt("170141183460469231731687303715884105727") << 1).ShowBit(os); os << '\n';
	(BigInt(dec2IntStr(std::to_string(pow(2, 126)))) << 1).ShowBit(os); os << '\n';
}

void basicTest02(std::ostream &os) 
{
	os << BigInt("256") - BigInt("128") << '\n';
	os << BigInt("128") - BigInt("256") << '\n';
	os << BigInt("256") - BigInt("0") << '\n';
	os << BigInt("0") - BigInt("256") << '\n';
	os << BigInt("128") - BigInt("-128") << '\n';
	os << BigInt(BigIntMIN) - BigInt("-1") << '\n';
	os << BigInt(BigIntMIN) - BigInt("1") << '\n';

	(BigInt("256") - BigInt("128")).ShowBit(os); os << '\n';
	(BigInt("128") - BigInt("256")).ShowBit(os); os << '\n';
	(BigInt("256") - BigInt("0")).ShowBit(os); os << '\n';
	(BigInt("0") - BigInt("256")).ShowBit(os); os << '\n';
	(BigInt("128") - BigInt("-128")).ShowBit(os); os << '\n';
	(BigInt(BigIntMIN) - BigInt("-1")).ShowBit(os); os << '\n';
	(BigInt(BigIntMIN) - BigInt("1")).ShowBit(os); os << '\n';
}

void Test(std::istream &is, std::ostream &os)
{
	char ToanTu;
	int n;
	std::string lhs, rhs;
	is >> ToanTu;
	is >> n;
	switch (ToanTu)
	{
	case '+':
		for(int i = 0; i < n; i++)
		{
			is >> lhs >> rhs;
			//os << (BigInt(lhs) + BigInt(rhs)) << '\n';
			(BigInt(lhs) + BigInt(rhs)).ShowBit(os); os << '\n';
		}
		break;
	case '-':	
		for(int i = 0; i < n; i++)
		{
			is >> lhs >> rhs;
			//os << (BigInt(lhs) - BigInt(rhs)) << '\n';
			(BigInt(lhs) - BigInt(rhs)).ShowBit(os); os << '\n';
		}
		break;
	case '*':
		for(int i = 0; i < n; i++)
		{
			is >> lhs >> rhs;
			//os << (BigInt(lhs) * BigInt(rhs)) << '\n';
			(BigInt(lhs) * BigInt(rhs)).ShowBit(os); os << '\n';
		}
		break;
	case '/':
		for(int i = 0; i < n; i++)
		{
			is >> lhs >> rhs;
			try {
				//os << (BigInt(lhs) / BigInt(rhs)) << '\n';
				(BigInt(lhs) / BigInt(rhs)).ShowBit(os); os << '\n';
			}
			catch (std::exception &e) {
				os << e.what() << '\n';
			}
			
		}
		break;
	case '&':
		for(int i = 0; i < n; i++)
		{
			is >> lhs >> rhs;
			//os << (BigInt(lhs) & BigInt(rhs)) << '\n';
			(BigInt(lhs) & BigInt(rhs)).ShowBit(os); os << '\n';
		}
		break;
	case '|':
		for(int i = 0; i < n; i++)
		{
			is >> lhs >> rhs;
			//os << (BigInt(lhs) | BigInt(rhs)) << '\n';
			(BigInt(lhs) | BigInt(rhs)).ShowBit(os); os << '\n';
		}
		break;
	case '^':
		for(int i = 0; i < n; i++)
		{
			is >> lhs >> rhs;
			//os << (BigInt(lhs) ^ BigInt(rhs)) << '\n';
			(BigInt(lhs) ^ BigInt(rhs)).ShowBit(os); os << '\n';
		}
		break;
	case '~':
		for(int i = 0; i < n; i++)
		{
			is >> lhs;
			//os << (~BigInt(lhs)) << '\n';
			(~BigInt(lhs)).ShowBit(os); os << '\n';
		}
		break;
	}
}

void main()
{
	std::ofstream outFile;
	std::ifstream inFile;

	for (int i = 1; i <= 6; i++)
	{
		inFile.open(std::string("in_") + std::to_string(i) + std::string(".txt"));
		outFile.open(std::string("out_") + std::to_string(i) + std::string(".txt"));
		Test(inFile, outFile);
		inFile.close();
		outFile.close();
	}
}
