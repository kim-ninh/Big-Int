#include "BigInt.h"


/**
* Hàm đặt giá trị các bit cho 1 số nguyên lớn.
* Chuyển số 1 nguyên lớn hệ 10 thành các bit trong bộ nhớ
*
* @dec_string chuỗi ký tự chứa số nguyên lớn
*/
void BigInt::set_bit(std::string&& dec_string)
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
		BYTE carry = 1;
		// đảo tất cả các bit & cộng 1
		std::for_each(std::begin(m_bits), std::end(m_bits),
			[&carry](BYTE& byte) -> void
		{
			BYTE temp = byte ^ UCHAR_MAX;
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
		const auto byte_value = static_cast<BYTE>(m_bits[i]);
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

BigInt::BigInt(const std::string& dec_string)
{
	set_bit(std::string(dec_string));
}

BigInt::BigInt(std::string&& dec_string)
{
	set_bit(std::move(dec_string));
}

BigInt::BigInt(const char* dec_string) : BigInt(std::string(dec_string)) {}

bool BigInt::operator!=(const BigInt& other) const
{
	for (int i = 0; i < MAX_BYTES; ++i)
	{
		if (this->m_bits[i] != other.m_bits[i])
			return true;
	}

	return false;
}


bool BigInt::operator==(const BigInt& other) const
{
	for (int i = 0; i < MAX_BYTES; ++i)
	{
		if (this->m_bits[i] != other.m_bits[i])
			return false;
	}

	return true;
}

void BigInt::ShowBit() const
{
	for (int i = MAX_BYTES - 1; i >= 0; --i)
		std::cout << std::bitset<8>(*const_cast<BYTE*>(m_bits + i)) << "  ";
}

std::string DecToBin(BigInt num) //Minh Nhật code
{
	std::string bin_string;
	bin_string.reserve(MAX_BYTES * CHAR_BIT);

	bool negative = to_string(num)[0] == '-' ? true : false; // kiểm tra số âm

	//NOTE: nếu phép %2 k tạo ra tình trạng dư "-1" thì có thể bỏ dòng lệnh dưới
	if (negative) num = "0" - num; //âm thì đảo dấu

	while (num != "0")
		{
			bin_string.insert(0, to_string(num % "2"));
			num = num / "2";
		}
	
	if (negative) //âm thì xử lý theo bù 2
	{
		bin_string.insert(0, MAX_BYTES * CHAR_BIT - bin_string.length(), '0'); //bù bit 0 cho đủ 128 bit
		for (int i = 0; i < MAX_BYTES * CHAR_BIT; i++) //Đảo bit
			bin_string[i] = (bin_string[i] == '0') ? '1' : '0';
		//+1
		if (bin_string[MAX_BYTES * CHAR_BIT - 1] = '1') //bit cuối == 1
		{
			char bit_nho = 1, i = MAX_BYTES * CHAR_BIT - 2;
			bin_string[MAX_BYTES * CHAR_BIT - 1]--; //về 0 và nhớ 1
			while (bit_nho == 1)
			{
				if (bin_string[i] == '1') bin_string[i]--; //về 0 vẫn nhớ 1
				else
				{
					bin_string[i]++;
					bit_nho = 0;
				}
				i--;
			}
		}
		else bin_string[MAX_BYTES * CHAR_BIT - 1]++;
	}
	return bin_string;
}

BigInt BinToDec(const std::string& bin_string)//Văn Nhật code
{
	BigInt res("0");
	std::string temp = "0";

	int j = 0, i;
	for (i = bin_string.length() - 1; i >= 0; i--) {
		if (bin_string[i] == '1') {
			temp = std::to_string(pow(2, j));
			temp = temp.substr(0, temp.length() - 7);
			res = res + BigInt(temp);
		}
		j++;
	}

	return res;
}

std::string DecToHex(BigInt num) //Minh Nhật code, dùng 2 hàm đã viết: DecToBin() và BinToHex()
{
	std::string bin_string = DecToBin(num);
	return BinToHex(bin_string);
}

BigInt HexToDec(const std::string& hex_string) //Văn Nhật code (Cách 1 - Xử lý KHÔNG dùng những hàm đã viết)
{
	BigInt res("0");
	std::string temp = "0";
	int l = hex_string.length() - 1;
	int i = 0;

	// xử lí khi có dấu âm => k có dấu âm k cần xử lí
	/*if (hex_string[0] == '-') {
	i++;
	l--;
	}*/

	for (; i < hex_string.length(); i++) {
		if (hex_string[i] >= 'A'&&hex_string[i] <= 'F') {
			temp = std::to_string(hex_string[i] - 55);
			res = res + BigInt(temp << (4 * l));
			l--;
		}
		else if (hex_string[i] >= 'a'&&hex_string[i] <= 'f') {
			temp = std::to_string(hex_string[i] - 87);
			res = res + BigInt(temp << (4 * l));
			l--;
		}
		else {
			temp = std::to_string(hex_string[i] - 48);
			res = res + BigInt(temp << (4 * l));
			l--;
		}
	}

	// nếu số âm thì biểu diễn sang bù 2
	/*if (hex_string[0] == '-') {
	res = ~(res);
	res = res + BigInt("1");
	}*/
	return res;
}

/// Cách 2: dựa vào các hàm đã viết
/*BigInt HexToDec(const std::string& hex_string)
{
	BigInt res("0");
	std::string bin_string = HexToBin(hex_string);
	res = BinToDec(bin_string);
	return res;
}*/



std::string BinToHex(const std::string& bin_string) //Minh Nhật code
{
	//Trường hợp số 0
	if (bin_string == "0") return "0";

	std::string hex_string;
	hex_string.reserve(MAX_BYTES*CHAR_BIT / 4);
	std::string sample("0123456789ABCDEF"); //Mẫu ký tự hex

	char bit_pos = 0, value = 0; //Ý tưởng: Gom nhóm 4 bit thành 1 ký tự hex từ phải sang trái

	while (bit_pos < bin_string.length())
	{
		value += (bin_string[bin_string.length() - bit_pos - 1] - '0')*(char)pow(2, bit_pos % 4);
		if (bit_pos % 4 == 3) //Mỗi 4 bit tiếng hành chuyển thành 1 ký tự hex rồi xóa giá trị nhớ value
		{
			hex_string.insert(0, 1, sample[value]);
			value = 0;
		}
		bit_pos++;
	}
	if (value != 0) hex_string.insert(0, 1, sample[value]); //Nếu số bit của chuỗi k chia hết cho 4 tức là còn ít hơn 4 bit -> tiếp tục chuyển giá trị nhớ value thành ký tự hex
	
	return hex_string;
}

std::string HexToBin(const std::string& hex_string) //Minh Nhật code
{
	if (hex_string == "0") return "0";

	std::string bin_string;
	bin_string.reserve(MAX_BYTES*CHAR_BIT);
	std::string sample("0123456789ABCDEF"); //mẫu ký tự hex

	char ch_pos = 0, value = 0;
	while (ch_pos < hex_string.length() - 1)
	{
		value = sample.find_first_of(hex_string[hex_string.length() - ch_pos - 1]);
		
		char bit_fill = 4; //số bit 0 bù cho đủ nhóm 4-bit
		while (value !=0)
		{
			bin_string.insert(0, 1, value % 2 + '0');
			value /= 2;
			bit_fill--;
		}
		bin_string.insert(0, bit_fill, '0'); //làm cho đủ 4-bit

		ch_pos++;
	}

	//xử lý ký tự đầu tiên
	value = sample.find_first_of(hex_string[0]);
	while (value != 0)
	{
		bin_string.insert(0, 1, value % 2 + '0');
		value /= 2;
	}

	return bin_string;
}

std::istream& operator>>(std::istream& inDev, BigInt& num)
{
	std::string dec_string;
	inDev >> dec_string;
	num.set_bit(std::move(dec_string));

	return inDev;
}

std::ostream& operator<<(std::ostream& outDev, const BigInt& num)
{
	outDev << num.get_dec_string();

	return outDev;
}

BigInt operator>>(BigInt num, int shift)
{
	BYTE carry = 0x00;
	BYTE fill_value = 0x00;
	const bool negative = num.m_bits[MAX_BYTES - 1] >> 7;		// kiểm tra số âm

	const int n_byte_shift = shift / CHAR_BIT;
	shift %= 8;

	if (negative)
	{
		carry = (0x7f >> (7 - shift)) & 0xff;		// carry = 
		fill_value = UCHAR_MAX;
	}

	// dịch shift/8 đơn vị byte
	if (n_byte_shift > 0)
	{
		memcpy(num.m_bits, num.m_bits + n_byte_shift, (MAX_BYTES - n_byte_shift) * sizeof(char));
		memset(num.m_bits + MAX_BYTES - n_byte_shift, fill_value, n_byte_shift * sizeof(char));
	}

	// dịch (shift % 8) đơn vị bit
	if (shift > 0)
	{
		for (int i = MAX_BYTES - 1; i >= 0; --i)
		{
			// nhớ các bit được dịch sang byte phía sau
			BYTE temp = (num.m_bits[i] << (CHAR_BIT - shift)) >> (CHAR_BIT - shift);
			// dịch byte hiện tại
			num.m_bits[i] >>= shift;
			num.m_bits[i] |= carry << (CHAR_BIT - shift);
			carry = temp;
		}
	}

	return num;
}

std::string to_string(const BigInt& num)
{
	return num.get_dec_string();
}

char Nho_Bit_Dich_Trai(char a, int shift)
{
	char tmp = 0;
	int t = 0, s = 7;
	while (shift != t) {
		tmp = tmp << 1;
		tmp = tmp | (a >> (s) & 1);
		s--; t++;
	}
	return tmp;
}
BigInt operator << (BigInt num, int shift)
{

	while (shift > 8) {
		char tmp = 0;
		int i = MAX_BYTES - 2;
		for (; i >= 0; i--)
		{
			tmp = Nho_Bit_Dich_Trai(num.m_bits[i], 8);
			num.m_bits[i + 1] = (num.m_bits[i + 1] << 8) | tmp;
		}
		num.m_bits[0] = num.m_bits[0] << 8;
		shift -= 8;
	}
	char tmp = 0;
	int i = MAX_BYTES - 2;
	for (; i >= 0; i--)
	{
		tmp = Nho_Bit_Dich_Trai(num.m_bits[i], shift);
		num.m_bits[i + 1] = (num.m_bits[i + 1] << shift) | tmp;
	}
	num.m_bits[0] = num.m_bits[0] << shift;
	return num;
}

BigInt operator+(const BigInt & lhs, const BigInt & rhs)
{
	bool carry = 0; // bit nhớ, mặc định là 0.
	BigInt result_int;
	for (int i = 0; i < MAX_BYTES; i++)
	{
		WORD temp_num; // lưu tạm kết quả cộng cặp byte thứ i.
		temp_num = lhs.m_bits[i] + rhs.m_bits[i] + carry; // cộng thêm bit nhớ nếu có.
		carry = 0; // cộng xong thì trả lại 0.

		if (temp_num > 255) // tổng vượt quá giới hạn của uchar thì bật bit nhớ.
		{
			carry = 1;
		}
		result_int.m_bits[i] = temp_num; // gán kết quả cuối.
	}
	return result_int;
}

BigInt operator - (const BigInt& lhs, const BigInt& rhs)	// Toán tử -
{
	BigInt kq;
	BigInt Bu_2;

	// Bù 1 của số rhs
	for (int i = 0; i < MAX_BYTES; i++) {
		Bu_2.m_bits[i] = ~(rhs.m_bits[i]);
	}

	// Bù 2 = Bù 1 + 1 của số rhs
	BYTE Nho = 1;
	for (int i = 0; i <MAX_BYTES; i++) {
		WORD tmp1 = Bu_2.m_bits[i] + Nho;
		Bu_2.m_bits[i] = tmp1;
		Nho = tmp1 / (UCHAR_MAX + 1);
	}


	// Cộng lsh với bù 2 của rhs
	BYTE carry = 0x00;
	for (int i = 0; i <MAX_BYTES; i++) {
		WORD tmp2 = lhs.m_bits[i] + Bu_2.m_bits[i] + carry;
		kq.m_bits[i] = tmp2;
		carry = tmp2 / (UCHAR_MAX + 1);
	}
	return kq;
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

	if (Q.m_bits[MAX_BYTES - 1] >> 7)
		Q = BigInt("0") - Q;

	if (M.m_bits[MAX_BYTES - 1] >> 7)
		M = BigInt("0") - M;

	for (int i = 0; i < MAX_BYTES; i++)
		A.m_bits[i] = 0;

	while (k)
	{
		//Dịch trái từng bit trên mảng bit [A, Q]
		unsigned char msbQ = Q.m_bits[MAX_BYTES - 1] >> 7;
		A = A << 1;
		A.m_bits[0] = A.m_bits[0] | msbQ;
		Q = Q << 1;

		A = A - M;
		if ((A.m_bits[MAX_BYTES - 1] >> 7) & 1)
		{
			A = A + M;
		}
		else
			Q.m_bits[0] = Q.m_bits[0] | 1;		//Bật bit 0 (Q0 = 1)

		k--;
	}

	//Nếu số chia và số bị chia khác dấu, đổi dấu thương
	if ((lhs.m_bits[MAX_BYTES - 1] >> 7) != (rhs.m_bits[MAX_BYTES - 1] >> 7))
		return BigInt("0") - Q;
	return Q;
}

//Hàm chia lấy phần dư tương tự chia nguyên
//Kết quả trả về là BigInt A thay vì BigInt Q
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

	if (Q.m_bits[MAX_BYTES - 1] >> 7)
		Q = BigInt("0") - Q;

	if (M.m_bits[MAX_BYTES - 1] >> 7)
		M = BigInt("0") - M;

	for (int i = 0; i < MAX_BYTES; i++)
		A.m_bits[i] = 0;

	while (k)
	{
		//Dịch trái từng bit trên mảng bit [A, Q]
		unsigned char msbQ = Q.m_bits[MAX_BYTES - 1] >> 7;
		A = A << 1;
		A.m_bits[0] = A.m_bits[0] | msbQ;
		Q = Q << 1;

		A = A - M;
		if ((A.m_bits[MAX_BYTES - 1] >> 7) & 1)
		{
			A = A + M;
		}
		else
			Q.m_bits[0] = Q.m_bits[0] | 1;		//Bật bit 0 (Q0 = 1)

		k--;
	}

	//Nếu số chia và số bị chia khác dấu, đổi dấu phần dư
	if ((lhs.m_bits[MAX_BYTES - 1] >> 7) != (rhs.m_bits[MAX_BYTES - 1] >> 7))
		return BigInt("0") - A;
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

// toán tử &
BigInt operator&(const BigInt & lhs, const BigInt & rhs)
{
	BigInt ResultInt;
	for (int i = 0; i < MAX_BYTES; i++)
	{
		ResultInt.m_bits[i] = lhs.m_bits[i] & rhs.m_bits[i];
	}
	return ResultInt;
}

// toán tử |
BigInt operator|(const BigInt & lhs, const BigInt & rhs)
{
	BigInt ResultInt;
	for (int i = 0; i < MAX_BYTES; i++)
	{
		ResultInt.m_bits[i] = lhs.m_bits[i] | rhs.m_bits[i];
	}
	return ResultInt;
}

//toán tử ^
BigInt operator^(const BigInt & lhs, const BigInt & rhs)
{
	BigInt ResultInt;
	for (int i = 0; i < MAX_BYTES; i++)
	{
		ResultInt.m_bits[i] = lhs.m_bits[i] ^ rhs.m_bits[i];
	}
	return ResultInt;
}

//toán tử ~
BigInt operator~(const BigInt & lhs)
{
	BigInt ResultInt;
	for (int i = 0; i < MAX_BYTES; i++)
	{
		ResultInt.m_bits[i] = ~lhs.m_bits[i];
	}
	return ResultInt;
}