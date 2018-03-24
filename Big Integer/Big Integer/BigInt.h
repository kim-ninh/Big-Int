#ifndef _BIG_INT_H
#define _BIG_INT_H

#include <iostream>
#include <iomanip>
#include <regex>
#include <bitset>
#include <exception>

#define MAX_BYTES 16
#define BigIntMAX  "170141183460469231731687303715884105727"		// 2^127 - 1
#define BigIntMIN "-170141183460469231731687303715884105728"		//-2^127
// chiều dài chuỗi số lớn nhất có thể biễu diễn được của BigInt
#define MAX_DEC_DIGITS ((std::to_string(pow(2, MAX_BYTES * 8 - 1) - 1.0).length()) - 7)

using WORD = unsigned short;
using BYTE = unsigned char;

class BigInt
{
private:
	unsigned char m_bits[MAX_BYTES];
public:
	void set_bit(std::string&& dec_string);
	void push_back(const short bit, short& nBits);
	std::string get_dec_string() const;
	static std::string add_dec_string(std::string dec_string1, std::string dec_string2);
public:
	BigInt() = default;
	BigInt(const std::string& dec_string);
	BigInt(std::string&& dec_string);
	BigInt(const char * dec_string);

	friend std::istream& operator >> (std::istream& inDev, BigInt& num);
	friend std::ostream& operator << (std::ostream& outDev, const BigInt& num);

	friend BigInt operator + (const BigInt& lhs, const BigInt& rhs);
	friend BigInt operator - (const BigInt& lhs, const BigInt& rhs);
	friend BigInt operator * (const BigInt& lhs, const BigInt& rhs);
	friend BigInt operator / (const BigInt& lhs, const BigInt& rhs);
	friend BigInt operator % (const BigInt & lhs, const BigInt & rhs);

	friend BigInt operator & (const BigInt& lhs, const BigInt& rhs);
	friend BigInt operator | (const BigInt& lhs, const BigInt& rhs);
	friend BigInt operator ^ (const BigInt& lhs, const BigInt& rhs);
	friend BigInt operator ~ (const BigInt& lhs);

	friend BigInt operator << (BigInt num, int shift);
	friend BigInt operator >> (BigInt num, int shift);

	bool operator != (const BigInt& other) const;
	bool operator == (const BigInt& other) const;

	friend std::string to_string(const BigInt& num);

	//test
	void ShowBit(std::ostream &os) const;
};

std::string DecToBin(BigInt num);
BigInt BinToDec(const std::string& bin_string);
std::string DecToHex(BigInt num);
BigInt HexToDec(const std::string& hex_string);
std::string BinToHex(const std::string& hex_string);
std::string HexToBin(const std::string& bin_string);
std::string ConvertBase(std::string number, int old_base, int new_base);


#endif
