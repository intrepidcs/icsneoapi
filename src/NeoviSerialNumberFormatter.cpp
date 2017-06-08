#include "NeoviSerialNumberFormatter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MIN_BASE36_SERIAL (16796160)

std::string NeoviSerialNumberFormatter::ToString(unsigned int number)
{
	char buffer[24];
	if (number < MIN_BASE36_SERIAL)
	{
		sprintf(buffer, "%u", number);
		return std::string(buffer);
	}
	else
		return To36(number);
}

unsigned int NeoviSerialNumberFormatter::ToInt(const char* str)
{
	if (IsBase36Serial(str))
		return From36(str);
	else
		return strtoul(str, NULL, 10);
}

bool NeoviSerialNumberFormatter::IsBase36Serial(const char* str)
{
	return !isdigit(str[0]) || !isdigit(str[1]);
}

const uint8_t NeoviSerialNumberFormatter::fromBase36Table[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
	21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
	24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 };

const char* NeoviSerialNumberFormatter::toBase36Table[36] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K",
	"L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" };

const uint32_t NeoviSerialNumberFormatter::powers[7] = { 1, 36, 1296, 46656, 1679616, 60466176, 2176782336 };

unsigned int NeoviSerialNumberFormatter::From36(const char* pSerial)
{
	int i;
	unsigned int ret = 0;

	for (i = 0; i < 6; i++)
	{
		ret *= 36;
		ret += fromBase36Table[(unsigned char)pSerial[i]];
	}
	return ret;
}

std::string NeoviSerialNumberFormatter::To36(unsigned int serial_no)
{
	int i, tmp;
	char work[7];

	if (serial_no == 0)
		return std::string("0");
	for (i = 5; i >= 0; i--)
	{
		tmp = serial_no / powers[i];
		serial_no = serial_no % powers[i];
		work[5 - i] = toBase36Table[tmp][0];
	}
	work[6] = '\0';
	return work;
}
