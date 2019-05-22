#include "ExtendedPrecission.hpp"
//#define DEBUG 1 // comment this line to skip debugging output

void ConvertToIeeeExtended(long double num, char *bytes);
void printBinary(char c);
int64_t carry = 0;

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
    if (this->sign == 0)
        s += '0';
    else
        s += '1';
    s += " ";
    s += intToBinaryString(this->exponent);
    s += " ";
    s += intToBinaryString(this->mantissa);
    return s;
}

uint16_t carryExponent(uint16_t exp)
{
    if (carry != 0)
    {
        exp += carry;
        carry = 0;
        return exp;
    }
}

uint64_t sum(uint64_t num1, uint64_t num2)
{
    carry = 0;
    __int128_t result = static_cast<__int128_t>(num1) + static_cast<__int128_t>(num2);
    while (result > UINT64_MAX)
    {
        result -= UINT64_MAX;
        carry++;
    }
    result = static_cast<uint64_t>(result);
    return result;
}
uint64_t sub(uint64_t num1, uint64_t num2)
{
    carry = 0;
    __int128_t result = static_cast<__int128_t>(num1) - static_cast<__int128_t>(num2);
    
    while (result < 0)
    {
        result += UINT64_MAX;
        carry--;
    }
    result = static_cast<uint64_t>(result);

    return result;
}

uint64_t scaleMantissa(ExtendedPrecission num1, ExtendedPrecission num2)
{
    __int128_t scaledMantissa;
    int64_t expDiff;
    if (num1 > num2)
    {
        expDiff = num1.getExponent() - num2.getExponent();
        scaledMantissa = static_cast<__int128_t>(num2.getMantissa()) * pow(2, expDiff) + num1.getMantissa();
    }
    else
    {
        expDiff = num2.getExponent() - num1.getExponent();
        scaledMantissa = static_cast<__int128_t>(num1.getMantissa()) * pow(2, expDiff) + num2.getMantissa(); 
    }
    while (scaledMantissa > UINT64_MAX)
    {
        scaledMantissa -= UINT64_MAX;
        carry++;
    }
    while (scaledMantissa < 0)
    {
        scaledMantissa += UINT64_MAX;
        carry--;
    }
    scaledMantissa = static_cast<uint64_t>(scaledMantissa);
    return scaledMantissa;   
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

    uint64_t man = 0;
    uint64_t manMask = 0b1111111111111111111111111111111111111111111111111111111111111111;
    man = this->byteBuffer[2];
    for (int i = 3; i < 10; i++)
    {
        man = man << 8;
        man += this->byteBuffer[i];
    }
    this->mantissa = man & manMask;

    //std::cout << "sign: " << std::to_string(this->sign) << std::endl;
    //std::cout << "exponent: " << this->exponent << std::endl;
    //std::cout << "mantissa: " << this->mantissa << std::endl;
}

std::string ExtendedPrecission::intToBinaryString(unsigned long long int num)
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
    ExtendedPrecission result(0.0);

    if (this->sign == num.getSign())
    {
        result.sign = this->sign;
        if (this->exponent == num.getExponent())
        {
            result.exponent = this->exponent;
            result.mantissa = sum(this->mantissa, num.getMantissa());
            result.exponent = carryExponent(result.exponent);
        }
        else
        {
            result.mantissa = scaleMantissa(*this, num);
            result.exponent = carryExponent(result.exponent);
        }
    }
    else if (*this > num)
    {
        result.sign = this->sign;
        if (this->exponent == num.getExponent())
        {
            result.exponent = this->exponent;
            result.mantissa = sub(this->mantissa, num.getMantissa());
            result.exponent = carryExponent(result.exponent);
        }
        else 
        {
            result.exponent = num.getExponent();
            result.mantissa = scaleMantissa(*this, num);
            result.exponent = carryExponent(result.exponent);
        }
    }
    else
    {
        result.sign = num.getSign();
        if (this->exponent == num.getExponent())
        {
            result.exponent = this->exponent;
            result.mantissa = sub(num.getMantissa(), this->mantissa);
            result.exponent = carryExponent(result.exponent);
        }
        else
        {
            result.exponent = num.getExponent();
            result.mantissa = scaleMantissa(num, *this);
            result.exponent = carryExponent(result.exponent);
        }
    }
    //TODO weryfikacja formatu wyniku (kiedy dodac do/odjac od wykladnika)
    return result;
}
ExtendedPrecission ExtendedPrecission::operator-(ExtendedPrecission num)
{
    ExtendedPrecission result(0.0);

    if (num.getSign() != 0)
    {
        result = *this + num;
    }
    else if (this->sign != 0)
    {
        result.sign = this->sign;
        if (this->exponent == num.getExponent())
        {
            result.exponent = this->exponent;
            result.mantissa = num.getMantissa() - this->mantissa;
        }
        else if (this->exponent < num.getExponent())
        {
            result.exponent = num.getExponent();
            result.mantissa = num.getMantissa() - (this->mantissa * pow(2, this->exponent - num.getExponent()));
        }
        else
        {
            result.exponent = this->exponent;
            result.mantissa = num.getMantissa() * pow(2, num.getExponent() - this->exponent) - this->mantissa;
        }
    }
    else if (*this > num)
    {
        result.sign = this->sign;
        if (this->exponent == num.getExponent())
        {
            result.exponent = this->exponent;
            result.mantissa = this->mantissa - num.getMantissa();
        }
        else if (this->exponent < num.getExponent())
        {
            result.exponent = num.getExponent();
            result.mantissa = this->mantissa * pow(2, this->exponent - num.getExponent()) - num.getMantissa();
        }
        else
        {
            result.exponent = this->exponent;
            result.mantissa = this->mantissa - (num.getMantissa() * pow(2, num.getExponent() - this->exponent));
        }
    }
    else
        result.sign = 1;
    if (this->exponent == num.getExponent())
    {
        result.exponent = this->exponent;
        result.mantissa = num.getMantissa() - this->mantissa;
    }
    else if (this->exponent < num.getExponent())
    {
        result.exponent = num.getExponent();
        result.mantissa = num.getMantissa() - (this->mantissa * pow(2, this->exponent - num.getExponent()));
    }
    else
    {
        result.exponent = this->exponent;
        result.mantissa = num.getMantissa() * pow(2, num.getExponent() - this->exponent) - this->mantissa;
    }

    //TODO weryfikacja formatu wyniku (kiedy dodac do/odjac od wykladnika)
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
    if (this->sign < num.getSign())
        return true;

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