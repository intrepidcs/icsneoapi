#ifndef NEOVI_SERIAL_NUMBER_FORMATTER__H_
#define NEOVI_SERIAL_NUMBER_FORMATTER__H_

#include <string>
#include <stdint.h>

class NeoviSerialNumberFormatter
{
public:
	static std::string ToString(unsigned int number);
	static unsigned int ToInt(const char *str);
	static bool IsBase36Serial(const char* str);
private:
	static unsigned int From36(const char* pSerial);
	static std::string To36(unsigned int serial_no);
	static const uint8_t fromBase36Table[];
	static const char* toBase36Table[];
	static const uint32_t powers[];
};

#endif /*NEOVI_SERIAL_NUMBER_FORMATTER__H_*/
