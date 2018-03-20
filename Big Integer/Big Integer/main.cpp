#include "BigInt.h"
#include <algorithm>
#include <random>


//void main()
//{
//	//std::cout << BigInt("-170141183460469231731687303715884105728");	// MIN = -2^(-127)			39 digits
//	//std::cout << BigInt("170141183460469231731687303715884105727");		// MAX = 2^127 - 1		39 digits
//
//	std::random_device rd;
//	std::mt19937 func(rd());
//	bool error = false;
//	const int N = 10000;
//
//	// test 4 toán tử &, |, ^, ~
//	static std::uniform_int_distribution<int> uid(1, 128);
//
//	std::string dec_string1("-256");
//	std::string dec_string2("256");
//
//	BigInt int1 = BigInt(dec_string1);
//	BigInt int2 = BigInt(dec_string2);
//
//	BigInt result_int = int1 + int2;
//
//	std::cout << int1 << std::endl;
//	std::cout << int2 << std::endl;
//	std::cout << result_int << std::endl;
//	//std::cout << ~int1 << std::endl;
//
//
//	// test khởi tạo
//	//std::string dec_string("-1234567891234567891234567891234567891");		// 38 digits
//
//	//for (int i = 0; i < N; ++i)
//	//{
//	//	//// không dấu
//	//	//std::shuffle(dec_string.begin(), dec_string.end(), func);
//	//	// có dấu
//	//	std::shuffle(dec_string.begin() + 1, dec_string.end(), func);
//	//	const std::string res = BigInt(dec_string).get_dec_string();
//	//	std::cout << res << " - " << dec_string << std::endl;
//
//	//	if (res != dec_string)
//	//	{
//	//		error = true;
//	//		break;
//	//	}
//	//}
//
//	
//
//
//	// test 2 toán tử <<, >>
//	//static std::uniform_int_distribution<int> uid(1, 128);
//	//srand(time(nullptr));
//	//không dấu
//	std::string dec_string("123456789");
//
//	//// có dấu
//	//std::string dec_string("-123456789");
//
//	for (int i = 0; i < N; ++i)
//	{
//		// không dấu
//		std::shuffle(dec_string.begin(), dec_string.end(), func);
//
//		//// có dấu
//		//std::shuffle(dec_string.begin() + 1, dec_string.end(), func);
//		
//		
//		// random số đơn vị dịch
//		int shift =  uid(func);
//		//int shift = rand() % 128 + 1;
//		int num = std::stoi(dec_string);
//	//	std::string res = (BigInt(dec_string) >> shift).get_dec_string();		// TOÁN TỬ >>
//		std::string res = (BigInt(dec_string) << shift).get_dec_string();		// TOÁN TỬ <<
//	//	num = num >> shift;
//		num = num << shift;
//		if (res != std::to_string(num))
//		{
//			std::cout << dec_string << std::endl;
//			std::cout << BigInt(dec_string) << " >> " << shift << " = " << num << " (built-in)" << std::endl;
//			std::cout << BigInt(dec_string) << " >> " << shift << " = " << res << " (BigInt)" << std::endl;
//			std::cout << res << " != " << std::to_string(num) << std::endl;
//			error = true;
//			break;
//		}
//	}
//	
//
//
//	/*int nshift = 4;
//	std::cout << ((0x7f >> (7 - nshift)) & 0xff);*/
//
//	//int width = 12;
//	//std::cout << std::left << std::setw(width) << "126: " << std::bitset<16>(126) << std::endl;
//	//std::cout << std::left << std::setw(width) << "126 >> 3: " << std::bitset<16>(126 >> 3) << std::endl;
//	//std::cout << std::left << std::setw(width) << "126 << 3: " << std::bitset<16>(126 << 3) << std::endl;
//	//std::cout << std::left << std::setw(width) << "-126: " << std::bitset<16>(-126) << std::endl;
//	//std::cout << std::left << std::setw(width) << "-126 >> 3: " << std::bitset<16>(-126 >> 3) << std::endl;
//	//std::cout << std::left << std::setw(width) << "-126 << 3: " << std::bitset<16>(-126 << 3) << std::endl;
//	//std::cout << std::left << std::setw(width) << "-126 << 3: " << std::bitset<8>(-126 << 3) << std::endl;
//	//std::cout << std::left << std::setw(width) << "-126 << 3: " << (int)char(-126 << 3) << std::endl;
//
//	std::cout << (error ? "ERROR" : "NO ERROR") << std::endl;
//	
//	system("pause");
//}

void main()
{


	system("pause");
}
