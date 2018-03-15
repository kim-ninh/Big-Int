#include "BigInt.h"
#include <algorithm>
#include <random>

void main()
{
	//std::cout << BigInt("-170141183460469231731687303715884105728");	// MIN = -2^(-127)			39 digits
	//std::cout << BigInt("170141183460469231731687303715884105727");		// MAX = 2^127 - 1		39 digits

	std::random_device rd;
	std::mt19937 func(rd());
	std::string dec_string("12345678912345678912345678912345678912");		// 38 digits
	bool error = false;
	const int N = 10000;

	for (int i = 0; i < N; ++i)
	{
		std::shuffle(dec_string.begin(), dec_string.end(), func);
		const std::string res = BigInt(dec_string).get_dec_string();
		std::cout << res << " - " << dec_string << std::endl;

		if (res != dec_string)
		{
			error = true;
			break;
		}
	}

	std::cout << (error ? "ERROR" : "NO ERROR") << std::endl;
	
	system("pause");
}
