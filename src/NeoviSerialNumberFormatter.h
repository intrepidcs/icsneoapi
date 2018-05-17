#ifndef NEOVI_SERIAL_NUMBER_FORMATTER__H_
#define NEOVI_SERIAL_NUMBER_FORMATTER__H_

#include <string>

class CNeoviSerialNumberFormatter
{
public:
	static std::string ConvertToStr(unsigned int number);
	static unsigned int ConvertToInt(const char* str);
	static bool IsBase36Ser(const char* str);
private:
	static unsigned int Convert36(const char* pSerial);
};

#endif /*NEOVI_SERIAL_NUMBER_FORMATTER__H_*/
