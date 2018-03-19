#include "BigInt.h"


/**
 * Hàm đặt giá trị các bit cho 1 số nguyên lớn.
 * Chuyển số 1 nguyên lớn hệ 10 thành các bit trong bộ nhớ
 * 
 * @dec_string chuỗi ký tự chứa số nguyên lớn
 */
void BigInt::set_bit(std::string dec_string)
{
	// kiểm tra tham số
	const std::regex r("\\-?[[:digit:]]+");
	if (!std::regex_match(dec_string, r))
		dec_string = "0";

	// đánh dấu nếu số âm
	const bool negative = dec_string[0] == '-';
	// đếm số chữ số
	short nDigits = negative ? dec_string.length() - 1 : dec_string.length();

	short nBits = 0;
	const short last_digit_index = dec_string.length() - 1;
	short first_digit_index = negative ? 1 : 0;
	memset(m_bits, 0, MAX_BYTES);

	while (nDigits > 0)
	{
		if ((dec_string[last_digit_index] - '0') % 2 == 1)
			push_back(1, nBits);
		else
			push_back(0, nBits);

		// cập nhật thương số
		int i = last_digit_index;
		while (i >= first_digit_index)
		{
			if (i > first_digit_index && (dec_string[i - 1] - '0') % 2 == 1)
			{
				dec_string[i - 1]--;
				dec_string[i] = (dec_string[i] - '0' + 10) / 2 + '0';
			}
			else
			{
				dec_string[i] = (dec_string[i] - '0') / 2 + '0';
			}

			i--;
		}

		if (dec_string[first_digit_index] == '0')
		{
			nDigits--;
			first_digit_index++;
		}

		//std::cout << dec_string.c_str() + first_digit_index << std::endl;
	}
	// bù 2 nếu số âm
	if (negative)
	{
		unsigned char carry = 1;
		// đảo tất cả các bit & cộng 1
		std::for_each(std::begin(m_bits), std::end(m_bits),
			[&carry](unsigned char& byte) -> void
		{
			unsigned char temp = byte ^ UCHAR_MAX;
			byte = temp + carry;
			carry = temp == UCHAR_MAX && carry == 1 ? 1 : 0;
		});
	}
}

/**
 * hàm thêm 1 bit vào đầu dãy bit (MSB)
 * 
 * @bit giá trị bit cần thêm (0 hoặc 1)
 * @nBits số bit dãy đang có
 */
void BigInt::push_back(const short bit, short& nBits)
{
	const size_t block_size = sizeof(*m_bits) * 8;
	m_bits[nBits / block_size] |= (bit << (nBits % block_size));
	nBits++;
}

/**
 * Hàm chuyển dãy bit trong bộ nhớ của số nguyên lớn thành hệ 10.
 * Kết quả được chứa trong chuỗi.
 * 
 * @return chuỗi chứa dãy số nguyên lớn hệ 10
 */
std::string BigInt::get_dec_string() const
{
	std::string res("0");

	for (int i = MAX_BYTES - 2; i >= 0; --i)
	{
		const auto byte_value = static_cast<unsigned char>(m_bits[i]);
		std::string temp = std::to_string(byte_value*pow(2, 8 * i));
		res = add_dec_string(res, temp.substr(0, temp.length() - 7));
	}

	const auto byte_value = static_cast<char>(m_bits[MAX_BYTES - 1]);
	std::string temp = std::to_string(byte_value * pow(2, 8 * (MAX_BYTES - 1)));
	res = add_dec_string(res, temp.substr(0, temp.length() - 7));

	const int index = res.find_first_not_of('0');
	if (index == std::string::npos)
		return "0";
	return res.substr(index, res.length() - index);
}

/**
 * Hàm cộng 2 số nguyên lớn chứa trong chuỗi.
 * 
 * @dec_string1 chuỗi chứa số nguyên số hạng đầu tiên, có thể âm hoặc dương
 * @dec_string2 chuỗi chứa số nguyên số hạng thứ 2, có thể âm hoặc dương
 * 
 * @return chuỗi chứa tổng
 */
std::string BigInt::add_dec_string(std::string dec_string1, std::string dec_string2)
{
	// hàm kiểm tra âm/dương của số nguyên chứa trong chuỗi
	const auto is_negative =
		[](const std::string& dec_string) -> bool
		{
			return dec_string[0] == '-';
		};
	// hàm trả về trị tuyệt đối của số nguyên trong chuỗi
	const auto abs =
		[](const std::string& dec_string) -> std::string
		{
			int index = dec_string.find_first_not_of('-');
			return dec_string.substr(index, dec_string.length() - index);
		};

	if (!is_negative(dec_string1) && !is_negative(dec_string2))			// cả 2 đều dương
	{
		const int max_len = std::max(dec_string1.length(), dec_string2.length());
		// chèn thêm các số 0 ở đầu chuỗi ngắn => đảm bảo chiều dài 2 chuỗi bằng nhau
		if (max_len > dec_string1.length())
			dec_string1.insert(0, max_len - dec_string1.length(), '0');
		if (max_len > dec_string2.length())
			dec_string2.insert(0, max_len - dec_string2.length(), '0');

		std::string result("0", max_len + 1);
		int carry = 0;
		// thực hiện phép cộng trên 2 chuỗi (toán tiểu học), lưu kết quả vào 'result'
		// thực hiện cộng theo thứ tự tăng dần hệ số => duyệt chuỗi từ sau về đầu
		std::transform(dec_string1.rbegin(), dec_string1.rend(), dec_string2.rbegin(), result.rbegin(),
			[&carry](const char a, const char b) -> char
			{
				int temp = a - '0' + b - '0' + carry;
				carry = temp / 10;
				return temp % 10 + '0';
			});

		result[0] = carry + '0';		// cộng thêm nếu còn nhớ
		// xóa các số 0 thừa ở đầu chuỗi kết quả
		const int index = result.find_first_not_of('0');
		if (index != std::string::npos)
			return result.substr(index, result.length() - index);

		return result;
	}
	else if (is_negative(dec_string1) && is_negative(dec_string2))		// cả 2 số đều âm
	{
		// -a - b = - (a + b)
		std::string temp = add_dec_string(dec_string1.erase(0, 1), dec_string2.erase(0, 1));
		return temp.insert(0, 1, '-');
	}
	else			// 1 âm, 1 dương
	{
		std::string abs_dec_string1 = abs(dec_string1);
		std::string abs_dec_string2 = abs(dec_string2);
		const int max_len = std::max(abs_dec_string1.length(), abs_dec_string2.length());
		// chèn 0 vào đầu => đảm bảo chiều dài 2 chuỗi = nhau
		if (max_len > abs_dec_string1.length())
			abs_dec_string1.insert(0, max_len - abs_dec_string1.length(), '0');
		if (max_len > abs_dec_string2.length())
			abs_dec_string2.insert(0, max_len - abs_dec_string2.length(), '0');
		// xác định kết quả âm hay dương
		const bool negative = (is_negative(dec_string1) && abs_dec_string1 > abs_dec_string2) ||
			(is_negative(dec_string2) && abs_dec_string2 > abs_dec_string1);

		std::string result("0", std::max(abs_dec_string1.length(), abs_dec_string2.length()) + 1);
		// đảm bảo số bị trừ luôn >= số trừ
		if (abs_dec_string1 < abs_dec_string2)
			std::swap(abs_dec_string1, abs_dec_string2);

		int carry = 0;
		// thực hiện phép trừ 2 chuỗi số dương (toán tiểu học)
		std::transform(abs_dec_string1.rbegin(), abs_dec_string1.rend(), abs_dec_string2.rbegin(), result.rbegin(),
			[&carry](const char a, const char b) -> char
			{
				int temp = a - '0' - (b - '0' + carry);
				if (temp < 0)
				{
					temp += 10;
					carry = 1;
				}
				else
					carry = 0;

				return temp % 10 + '0';
			});

		// thêm dấu '-' nếu đã xác định trước kết quả là âm
		if (negative)
		{
			const int index = result.find_first_not_of('0');
			if (index > 0)
				result[index - 1] = '-';
			else
				result.insert(0, 1, '-');
		}

		// loại bỏ các số 0 thừa ở đầu
		const int index = result.find_first_not_of('0');
		if (index == std::string::npos)
			return "0";
		if (index > 0)
			return result.substr(index, result.length() - index);

		return result;
	}
}

BigInt::BigInt(std::string dec_string)
{
	set_bit(dec_string);
}

void BigInt::ShowBit() const
{
	for (int i = MAX_BYTES - 1; i >= 0; --i)
		std::cout << std::bitset<8>(*const_cast<unsigned char*>(m_bits + i)) << "  ";
}

std::istream& operator>>(std::istream& inDev, BigInt& num)
{
	std::string dec_string;
	inDev >> dec_string;
	num.set_bit(dec_string);

	return inDev;
}

std::ostream& operator<<(std::ostream& outDev, const BigInt& num)
{
	outDev << num.get_dec_string();

	return outDev;
}

/*
Hàm chia 2 số BigInt với nhau
Có sử dụng đến toán tử dịch bit (<<) và toán tử (-) trong thuật toán
Kiểu trả về là Bigint
Thực hiện phép tính: Q / M, kết quả trả về lưu trong biến Q
*/

BigInt operator/(const BigInt & lhs, const BigInt & rhs)
{
	bool isZero = true;
	for (int i = 0; i < MAX_BYTES; i++)
	{
		if (rhs.m_bits[i] != 0)
		{
			isZero = false;
			break;
		}
	}
	if (isZero)
		throw std::invalid_argument("Can't divide by zero");

	BigInt A, Q, M;
	int k = MAX_BYTES * 8;
	Q = lhs;
	M = rhs;

	//Xác định Q âm hay dương
	if ((Q.m_bits[MAX_BYTES - 1] >> 7) & 1)
	{
		for (int i = 0; i < MAX_BYTES; i++)
			A.m_bits[i] = 0xFF;
	}	
	else
	{
		for (int i = 0; i < MAX_BYTES; i++)
			A.m_bits[i] = 0;
	}

	while (k)
	{
		//Dịch trái từng bit trên mảng bit [A, Q]
		unsigned char msbQ = Q.m_bits[MAX_BYTES - 1] >> 7;
		A << 1;
		A.m_bits[0] = A.m_bits[0] | msbQ;
		Q << 1;

		//KT A âm hay dương
		A = A - M;
		if ((A.m_bits[MAX_BYTES - 1] >> 7) & 1)
		{
			Q.m_bits[0] = Q.m_bits[0] & (~1);	//Tắt bit 0 (Q0 = 0)
			A = A + M;
		}
		else
			Q.m_bits[0] = Q.m_bits[0] | 1;		//Bật bit 0 (Q0 = 1)
		k--;
	}
	return Q;
}

BigInt operator%(const BigInt & lhs, const BigInt & rhs)
{
	bool isZero = true;
	for (int i = 0; i < MAX_BYTES; i++)
	{
		if (rhs.m_bits[i] != 0)
		{
			isZero = false;
			break;
		}
	}
	if (isZero)
		throw std::invalid_argument("Can't divide by zero");

	BigInt A, Q, M;
	int k = MAX_BYTES * 8;
	Q = lhs;
	M = rhs;

	//Xác định Q âm hay dương
	if ((Q.m_bits[MAX_BYTES - 1] >> 7) & 1)
	{
		for (int i = 0; i < MAX_BYTES; i++)
			A.m_bits[i] = 0xFF;
	}
	else
	{
		for (int i = 0; i < MAX_BYTES; i++)
			A.m_bits[i] = 0;
	}

	while (k)
	{
		//Dịch trái từng bit trên mảng bit [A, Q]
		unsigned char msbQ = Q.m_bits[MAX_BYTES - 1] >> 7;
		A << 1;
		A.m_bits[0] = A.m_bits[0] | msbQ;
		Q << 1;

		//KT A âm hay dương
		A = A - M;
		if ((A.m_bits[MAX_BYTES - 1] >> 7) & 1)
		{
			Q.m_bits[0] = Q.m_bits[0] & (~1);	//Tắt bit 0 (Q0 = 0)
			A = A + M;
		}
		else
			Q.m_bits[0] = Q.m_bits[0] | 1;		//Bật bit 0 (Q0 = 1)
		k--;
	}
	return A;
}

BigInt operator*(const BigInt & lhs, const BigInt & rhs)
{
	BigInt A, M, Q;
	M = lhs;
	Q = rhs;
	for (int i = 0; i < MAX_BYTES; i++)
		A.m_bits[i] = 0;
	int k = MAX_BYTES * 8;
	char Q_1, Q_0;
	while (k)
	{
		//Lấy 2 bit cuối Q_0, Q_1
		Q_1 = k == MAX_BYTES * 8 ? 0 : Q_0;
		Q_0 = Q.m_bits[0] & 1;

		if (Q_0 == 1 && Q_1 == 0)
			A = A - M;
		else if (Q_0 == 0 && Q_1 == 1)
			A = A + M;

		//Dịch phải mảng bit [A, Q, Q_1]
		unsigned char lsbA = A.m_bits[0] & 1;
		A = A >> 1;
		Q = Q >> 1;
		if (lsbA == 1)
			Q.m_bits[MAX_BYTES - 1] = Q.m_bits[MAX_BYTES - 1] | (1 << 7); //  bật bit nhớ
		else
			Q.m_bits[MAX_BYTES - 1] = Q.m_bits[MAX_BYTES - 1] & (~(1 << 7)); // tắt bit nhớ
		k--;
	}

	// Nếu A khác 0 hoặc A khác -1 thì kết quả nhân tràn số
	for (int i = 0; i < MAX_BYTES - 1; i++)
		if (A.m_bits[i] != A.m_bits[i + 1])
			throw std::overflow_error("Overflow!");
	return Q;
}