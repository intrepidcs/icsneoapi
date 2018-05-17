#include "NeoviSerialNumberFormatter.h"
#include "OSAbstraction.h"
#include <string>
#include <sstream>
#include <stdlib.h>

#define MIN_BASE36_SERIAL (16796160)

unsigned int CNeoviSerialNumberFormatter::ConvertToInt(const char* str)
{
	unsigned int ret;
	if (!isdigit(str[0]) || !isdigit(str[1]))
	{
		ret = Convert36(str);
	}
	else
	{
		ret = atoi(str);
	}
	return ret;
}

bool CNeoviSerialNumberFormatter::IsBase36Ser(const char* str)
{
	return !isdigit(str[0]) || !isdigit(str[1]);
}

static const uint8_t fromBase36Table[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12,
	13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 };

static const char* toBase36Table[36] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E",
	"F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" };

static const uint32_t powers[7] = { 1, 36, 1296, 46656, 1679616, 60466176, 2176782336 };

unsigned int CNeoviSerialNumberFormatter::Convert36(const char* pSerial)
{
	int i = 0;
	const char* str = pSerial;
	unsigned int ret = 0;
	unsigned int nextValue;

	while (*pSerial != 0 && i < 6)
	{
		if (*pSerial == L' ')
		{
			pSerial++;
		}
		else
		{
			nextValue = fromBase36Table[(unsigned char)*pSerial];
			ret *= 36;
			ret += nextValue;
			pSerial++;
			i++;
		}
	}
	return ret;
}

std::string CNeoviSerialNumberFormatter::ConvertToStr(unsigned int serial_no)
{
	int i, tmp;
	std::stringstream s;

	if (serial_no == 0 || serial_no > 2176782335)
	{
		return "0";
	}
	if (serial_no < MIN_BASE36_SERIAL)
	{
		s << serial_no;	
	}
	else
	{
		for (i = 5; i >= 0; i--)
		{
			tmp = serial_no / powers[i];
			serial_no = serial_no % powers[i];
			s << toBase36Table[tmp];
		}
	}
	return s.str();
}
