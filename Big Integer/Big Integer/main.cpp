#include "BigInt.h"

void main()
{

	//std::cout << BigInt("-170141183460469231731687303715884105728");
	//std::cout << BigInt("170141183460469231731687303715884105727");

	srand(time(nullptr));
	const int N = 100000;
	const auto MIN = INT_MAX / 2;
	const auto MAX = INT_MIN / 2;
	bool error = false;

	for (int i = 0; i < N; ++i)
	{
		const auto num = MIN + (rand() % static_cast<int>(MAX - MIN + 1));
		std::string res = BigInt(std::to_string(num)).get_dec_string();
		std::cout << res << " - " << num << std::endl;
		if (res != std::to_string(num))
		{
			error = true;
			break;
		}
	}

	std::cout << (error ? "ERROR" : "NO ERROR") << std::endl;

	
	system("pause");
}
