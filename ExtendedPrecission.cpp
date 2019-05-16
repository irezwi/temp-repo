#include "ExtendedPrecission.hpp"
//#define DEBUG 1 // comment this line to skip debugging output

void ConvertToIeeeExtended(long double num, char *bytes);
void printBinary(char c);

std::string ExtendedPrecission::toString()
{
    /*std::string s = "";
    for (auto &c : this->byteBuffer)
    {
        for (int i = 7; i >= 0; --i)
        {
            s += ((c & (1 << i)) ? '1' : '0');
        }
        s += ' ';
    }
    return s;*/
    std::string s = "";
    s += intToBinaryString(this->sign);
    s += " ";
    s += intToBinaryString(this->exponent);
    s += " ";
    s += intToBinaryString(this->mantissa);
    if (s.length() < 82)
    {
        for (int i = s.length(); i < 82; i++)
        {
            s += "0";
        }
    }
    return s;
}

ExtendedPrecission::ExtendedPrecission(long double num)
{
    ConvertToIeeeExtended(num, this->byteBuffer);

    // Set exponent field
    uint16_t exp = 0;
    exp = (this->byteBuffer[0] & 0b01111111);
    exp = exp << 8;
    exp += this->byteBuffer[1];
    this->exponent = exp;

    // Set sign field
    uint8_t s = (int)this->byteBuffer[0];
    s = s >> 7;
    this->sign = s;

    // Set mantissa field
    int byteWithLastOne = 2;
    for (int i = byteWithLastOne; i < 10; i++)
    {
        if ((int)this->byteBuffer[i] != 0)
        {
            byteWithLastOne = i;
        }
    }
    uint64_t man = 0;
    uint64_t manMask = 0b0000000000000000000000000000000000000000000000000000000011111111;
    man = this->byteBuffer[2];
    for (int i = 2; i != byteWithLastOne; i++)
    {
        man = man << 8;
        man += this->byteBuffer[i];
        manMask = manMask << 8;
        manMask += 0b11111111;
    }
    this->mantissa = man & manMask;

    //std::cout << "sign: " << std::to_string(this->sign) << std::endl;
    //std::cout << "exponent: " << this->exponent << std::endl;
    //std::cout << "mantissa: " << this->mantissa << std::endl;
}

std::string ExtendedPrecission::intToBinaryString(long long int num)
{
    std::string s = "";
    while (num > 0)
    {
        if (num % 2 == 0)
            s += '0';
        else
            s += '1';
        num = num / 2;
    }
    s = std::string(s.rbegin(), s.rend());
    return s;
}

#ifndef HUGE_VAL
#define HUGE_VAL HUGE
#endif /*HUGE_VAL*/

#define FloatToUnsigned(f) ((unsigned long)(((long)(f - 2147483648.0)) + 2147483647L) + 1)

void ConvertToIeeeExtended(long double num, char *bytes)
{
    int sign;
    int expon;
    long double fMant, fsMant;
    unsigned long hiMant, loMant;

    if (num < 0)
    {
        sign = 0x8000;
        num *= -1;
    }
    else
    {
        sign = 0;
    }

    if (num == 0)
    {
        expon = 0;
        hiMant = 0;
        loMant = 0;
    }
    else
    {
        fMant = frexp(num, &expon);
        if ((expon > 16384) || !(fMant < 1))
        { /* Infinity or NaN */
            expon = sign | 0x7FFF;
            hiMant = 0;
            loMant = 0; /* infinity */
        }
        else
        { /* Finite */
            expon += 16382;
            if (expon < 0)
            { /* denormalized */
                fMant = ldexp(fMant, expon);
                expon = 0;
            }
            expon |= sign;
            fMant = ldexp(fMant, 32);
            fsMant = floor(fMant);
            hiMant = FloatToUnsigned(fsMant);
            fMant = ldexp(fMant - fsMant, 32);
            fsMant = floor(fMant);
            loMant = FloatToUnsigned(fsMant);
        }
    }

    bytes[0] = expon >> 8;
    bytes[1] = expon;
    bytes[2] = hiMant >> 24;
    bytes[3] = hiMant >> 16;
    bytes[4] = hiMant >> 8;
    bytes[5] = hiMant;
    bytes[6] = loMant >> 24;
    bytes[7] = loMant >> 16;
    bytes[8] = loMant >> 8;
    bytes[9] = loMant;

#ifdef DEBUG
    for (int i = 0; i < 10; ++i)
    {
        std::cout << bytes[i];
    }
#endif
}

void printBinary(char c)
{
    for (int i = 7; i >= 0; --i)
    {
        std::cout << ((c & (1 << i)) ? '1' : '0');
    }
}

ExtendedPrecission ExtendedPrecission::operator+(ExtendedPrecission num)
{
    // TODO: implement this
    ExtendedPrecission result(0.0);

    if (this->sign == num.getSign())
        result.sign = this->sign;
    else if (this->exponent > num.getExponent())
        result.sign = this->sign;
    else
        result.sign = num.getSign();
    
    
    if(this->exponent == num.getExponent())
    {
        result.exponent = this->exponent;
        result.mantissa = this->mantissa + num.getMantissa();
    }
    else if(this->exponent < num.getExponent())
    {
        result.exponent = num.getExponent();
        result.mantissa = this->mantissa * pow(2, this->exponent - num.getExponent()) + num.getMantissa();
    }
    else
    {
        result.exponent = this->exponent;
        result.mantissa = num.getMantissa() * pow(2, num.getExponent() - this->exponent) + this->mantissa;
    }
    
    return result;
}

bool ExtendedPrecission::operator==(ExtendedPrecission num)
{
    if (this->sign == num.getSign() && this->exponent == num.getExponent() && this->mantissa == num.getMantissa())
    {
        return true;
    }
    return false;
}

bool ExtendedPrecission::operator>(ExtendedPrecission num)
{
    if (this->sign > num.getSign())
        return true;

    if (this->sign < num.getSign())
        return false;

    if (this->exponent > num.getExponent())
        return true;

    if (this->exponent < num.getExponent())
        return false;

    return this->mantissa > num.getMantissa();
}

bool ExtendedPrecission::operator<(ExtendedPrecission num)
{
    if (*this > num || *this == num)
        return false;
    return true;
}

uint8_t ExtendedPrecission::getSign()
{
    return this->sign;
}

uint16_t ExtendedPrecission::getExponent()
{
    return this->exponent;
}

uint64_t ExtendedPrecission::getMantissa()
{
    return this->mantissa;
}