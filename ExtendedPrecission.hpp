#include <string>
#include <bitset>
#include <iostream>
#include <cmath>

class ExtendedPrecission
{
private:
  char byteBuffer[10];
  uint8_t sign;
  uint16_t exponent;
  uint64_t mantissa;

public:
  ExtendedPrecission(long double num);
  ExtendedPrecission operator+(ExtendedPrecission num);
  ExtendedPrecission operator-(ExtendedPrecission num);
  ExtendedPrecission operator*(ExtendedPrecission num);
  ExtendedPrecission operator/(ExtendedPrecission num);
  bool operator==(ExtendedPrecission num);
  bool operator>(ExtendedPrecission num);
  bool operator<(ExtendedPrecission num);
  std::string toString();
  std::string intToBinaryString(unsigned long long int num);
  uint8_t getSign();
  uint16_t getExponent();
  uint64_t getMantissa();
};