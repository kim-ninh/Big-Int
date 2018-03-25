
#ifndef _TEST_H
#define _TEST_H

#include "BigInt.h"
#include <ostream>
#include <fstream>
#include <string>


void Test(std::istream &is, std::ostream &os);
BigInt calculate(std::string oprt, const BigInt &n1, const BigInt &n2);

#endif