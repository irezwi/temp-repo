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
  uint32_t mantissa;

public:
  ExtendedPrecission(long double num);
  ExtendedPrecission operator+(ExtendedPrecission num);
  ExtendedPrecission operator-(ExtendedPrecission num);
  ExtendedPrecission operator*(ExtendedPrecission num);
  ExtendedPrecission operator/(ExtendedPrecission num);
  std::string toString();
  std::string intToBinaryString(long long int num);
};