#ifndef _BIG_INT_H
#define _BIG_INT_H

#include <iostream>
#include <iomanip>
#include <regex>
#include <bitset>
#include <exception>


#define MAX_BYTES 16
// chiều dài chuỗi số lớn nhất có thể biễu diễn được của BigInt
#define MAX_DEC_DIGITS ((std::to_string(pow(2, MAX_BYTES * 8 - 1) - 1.0).length()) - 7)

using WORD = unsigned short;
using BYTE = unsigned char;

class BigInt
{
private:
	unsigned char m_bits[MAX_BYTES];
public:
	void set_bit(std::wstring&& dec_string);
	void push_back(const short bit, short& nBits);
	std::wstring get_dec_string() const;
	static std::wstring add_dec_string(std::wstring dec_string1, std::wstring dec_string2);
public:
	BigInt() = default;
	BigInt(const std::wstring& dec_string);
	BigInt(std::wstring&& dec_string);
	BigInt(const wchar_t * dec_string);
	BigInt(const int& num);

	friend std::wistream& operator >> (std::wistream& inDev, BigInt& num);
	friend std::wostream& operator << (std::wostream& outDev, const BigInt& num);

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

	friend std::wstring to_wstring(const BigInt& num);

	//test
	void ShowBit() const;
};

BigInt operator + (const BigInt& lhs, const BigInt& rhs);
BigInt operator - (const BigInt& lhs, const BigInt& rhs);
BigInt operator * (const BigInt& lhs, const BigInt& rhs);
BigInt operator / (const BigInt& lhs, const BigInt& rhs);
BigInt operator % (const BigInt & lhs, const BigInt & rhs);

BigInt operator & (const BigInt& lhs, const BigInt& rhs);
BigInt operator | (const BigInt& lhs, const BigInt& rhs);
BigInt operator ^ (const BigInt& lhs, const BigInt& rhs);

std::wstring DecToBin(BigInt num);
BigInt BinToDec(const std::wstring& bin_string);
std::wstring DecToHex(BigInt num);
BigInt HexToDec(const std::wstring& hex_string);
std::wstring BinToHex(const std::wstring& hex_string);
std::wstring HexToBin(const std::wstring& bin_string);

std::wstring ConvertBase(std::wstring number, int old_base, int new_base);


#endif
