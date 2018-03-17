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

BigInt operator>>(BigInt num, int shift)	// Phú code
{
	unsigned char carry = 0x00;
	unsigned char fill_value = 0x00;
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
			unsigned char temp = (num.m_bits[i] << (CHAR_BIT - shift)) >> (CHAR_BIT - shift);
			// dịch byte hiện tại
			num.m_bits[i] >>= shift;
			num.m_bits[i] |= carry << (CHAR_BIT - shift);
			carry = temp;
		}
	}

	return num;
}

// HÀM >> và << NHẬT CODE
char Nho_Bit_Dich_Phai(char a, int shift) {
	char tmp = 0;
	int t = 0, s = shift;
	while (shift != t) {
		tmp = (tmp << 1);
		tmp = tmp | ((a >> (s - 1)) & 1);
		s--; t++;
	}
	return (tmp << (8 - shift));
}
BigInt operator >> (BigInt num, int shift) {
	while (shift > 8) {
		char tmp = Nho_Bit_Dich_Phai(num.m_bits[MAX_BYTES - 1], 8);

		if ((num.m_bits[MAX_BYTES - 1] >> 7) & 1 == 1) {
			int d = 8;
			num.m_bits[MAX_BYTES - 1] = num.m_bits[MAX_BYTES - 1] >> 8;
			while (1) {
				if (d <= 0)break;
				num.m_bits[MAX_BYTES - 1] = num.m_bits[MAX_BYTES - 1] | (1 << (8 - d));
				d--;
			}
		}
		else {
			num.m_bits[MAX_BYTES - 1] = num.m_bits[MAX_BYTES - 1] >> 8;
		}
		for (int i = MAX_BYTES - 2; i >= 0; i--) {
			char nho_byte_truoc = num.m_bits[i];
			num.m_bits[i] = (num.m_bits[i] >> 8) | tmp;
			tmp = Nho_Bit_Dich_Phai(nho_byte_truoc, 8);
		}
		shift -= 8;
	}

	char tmp = Nho_Bit_Dich_Phai(num.m_bits[MAX_BYTES - 1], shift);
	if ((num.m_bits[MAX_BYTES - 1] >> 7) & 1 == 1) {
		int d = shift;
		num.m_bits[MAX_BYTES - 1] = num.m_bits[MAX_BYTES - 1] >> shift;
		while (1) {
			if (d <= 0)break;
			num.m_bits[MAX_BYTES - 1] = num.m_bits[MAX_BYTES - 1] | (1 << (8 - d));
			d--;
		}
	}
	else {
		num.m_bits[MAX_BYTES - 1] = num.m_bits[MAX_BYTES - 1] >> shift;
	}
	for (int i = MAX_BYTES - 2; i >= 0; i--) {
		char nho_byte_truoc = num.m_bits[i];
		num.m_bits[i] = (num.m_bits[i] >> shift) | tmp;
		tmp = Nho_Bit_Dich_Phai(nho_byte_truoc, shift);
	}
	return num;
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

BigInt operator - (const BigInt& lhs, const BigInt& rhs)	// Toán tử -
{
	BigInt kq;
	using WORD = unsigned short;
	using BYTE = unsigned char;
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