
#ifndef _TEST_H
#define _TEST_H

#include "BigInt.h"
#include <ostream>
#include <fstream>
#include <string>


void Test(std::wifstream& is, std::wofstream& os);
BigInt calculate(std::wstring oprt, const BigInt &n1, const BigInt &n2);

#endif