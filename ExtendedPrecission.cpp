#include "ExtendedPrecission.hpp"
//#define DEBUG 1 // comment this line to skip debugging output

void ConvertToIeeeExtended(long double num, char *bytes);
void printBinary(char c);
int64_t carry = 0;
const int exponentWeight = 16383;
enum OperationType
{
    osum,
    osub
};

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
    if (carry > 0)
        exp += carry;
    else
        exp -= carry;
    carry = 0;
    return exp;
}

uint64_t scaleMantissa(ExtendedPrecission num1, ExtendedPrecission num2, OperationType op)
{
    __int128_t scaledMantissa;
    int64_t expDiff;

    if (num1.getExponent() > num2.getExponent())
        expDiff = num1.getExponent() - num2.getExponent();
    else
        expDiff = num2.getExponent() - num1.getExponent();

    if (op == OperationType::osum)
    {   
        if (num1.getExponent() > num2.getExponent())
        {
            scaledMantissa = pow(2, num2.getExponent() - exponentWeight) * (num1.getMantissa() * pow(2, expDiff) + num2.getMantissa());
            if(scaledMantissa > pow(2, num1.getExponent() - exponentWeight + 63))
                carry++;
        }
        else
        {
            scaledMantissa = pow(2, num1.getExponent() - exponentWeight) * (num2.getMantissa() * pow(2, expDiff) + num1.getMantissa());
            if(scaledMantissa > pow(2, num2.getExponent() - exponentWeight + 63))
                carry++;
        }
        
        while (scaledMantissa > UINT64_MAX)
        {
            scaledMantissa = scaledMantissa/2;
        }
    }

    if (op == OperationType::osub)
    {
        if (num1.getExponent() > num2.getExponent())
        {
            scaledMantissa = pow(2, num2.getExponent() - exponentWeight) * ((num1.getMantissa() * pow(2, expDiff)) - num2.getMantissa());
            if(scaledMantissa > pow(2, num1.getExponent() - exponentWeight + 63))
                carry--;
        }
        else
        {
            scaledMantissa = pow(2, num1.getExponent() - exponentWeight) * (num2.getMantissa() * pow(2, expDiff) - num1.getMantissa());
            if(scaledMantissa > pow(2, num2.getExponent() - exponentWeight + 63))
                carry--;
        }
        if (num1.getMantissa() == num2.getMantissa())
            scaledMantissa = pow(2, 63);
        while (scaledMantissa > UINT64_MAX)
        {
            scaledMantissa = scaledMantissa/2;
        }
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
    if(this->exponent == 0 && num.getExponent() == 0)
    {
        return result;
    }
    if (this->exponent == 0){
        result = num;
        return result;   
    }
    else if(num.getExponent() == 0)
    {
        result = *this;
        return result;
    }

    if (this->sign == num.getSign())
    {
        result.sign = this->sign;
        if(this->exponent > num.getExponent())
            result.exponent = this->exponent;
        else
            result.exponent = num.getExponent();
        if (this->exponent == num.getExponent())
        {
            result.mantissa = scaleMantissa(*this, num, OperationType::osum);
        }
        else
        {
            result.mantissa = scaleMantissa(*this, num, OperationType::osum);
        }
    }
    else if (*this > num)
    {
        result.sign = this->sign;
        if (this->exponent == num.getExponent())
        {
            result.exponent = this->exponent;
            result.mantissa = scaleMantissa(*this, num, OperationType::osub);
        }
        else
        {
            result.exponent = num.getExponent();
            result.mantissa = scaleMantissa(*this, num, OperationType::osub);
        }
    }
    else
    {
        result.sign = num.getSign();
        if (this->exponent == num.getExponent())
        {
            result.exponent = this->exponent;
            result.mantissa = scaleMantissa(*this, num, OperationType::osub);
        }
        else
        {
            result.exponent = num.getExponent();
            result.mantissa = scaleMantissa(*this, num, OperationType::osub);
        }
    }
    result.exponent = carryExponent(result.exponent);
    return result;
}

ExtendedPrecission ExtendedPrecission::operator-(ExtendedPrecission num)
{
    ExtendedPrecission result(0.0);
    if(*this == num || (this->exponent == 0 && num.getExponent() == 0))
    {
        return result;
    }
    if (this->exponent == 0){
        result = num;
        if(result.sign == 0)
            result.sign = 1;
        else
            result.sign = 0;
        return result;       
    }
    else if(num.getExponent() == 0)
    {
        result = *this;
        return result;
    }
    if (this->exponent > num.getExponent())
            result.exponent = this->exponent;
        else
            result.exponent = num.getExponent();
    if (this->sign == num.getSign() && this->sign == 0) //if ++
    {
        if (*this < num)
        {
            result.sign = 1;
        }
        else
        {
            result.sign = 0;
        }
        result.mantissa = scaleMantissa(*this, num, OperationType::osub);        
    }
    else if (this->sign == num.getSign() && this->sign != 0) //if --
    {
        if (*this > num)
        {
            result.sign = 0;  
            result.exponent--; 
        }
        else
        {
            result.sign = 1;
        }
        result.mantissa = scaleMantissa(*this, num, OperationType::osub);
    }
    else if (this->sign != num.getSign() && this->sign == 0) //if +-
    {
        result.sign = 0;
        result.mantissa = scaleMantissa(*this, num, OperationType::osum);

    }
    else if (this->sign != num.getSign() && this->sign != 0) //if-+
    {
        result.sign = 1;
        result.mantissa = scaleMantissa(*this, num, OperationType::osum);  
    }
    result.exponent = carryExponent(result.exponent);
    return result;
}

bool ExtendedPrecission::operator==(ExtendedPrecission num)
{
    if (this->sign == num.getSign() && this->exponent == num.getExponent() && this->mantissa == num.getMantissa())
        return true;
    return false;
}

bool ExtendedPrecission::operator>(ExtendedPrecission num)
{
    if (this->sign < num.getSign())
        return true;
    else if (this->sign > num.getSign())
        return false;
    else if (this->sign == 0) //if both >0
    {
        if (this->exponent > num.getExponent())
            return true;

        if (this->exponent < num.getExponent())
            return false;

        return this->mantissa > num.getMantissa();
    }
    else if (this->sign != 0) // if both <0
    {
        if (this->exponent > num.getExponent())
            return false;

        if (this->exponent < num.getExponent())
            return true;

        return this->mantissa < num.getMantissa();
    }
}

bool ExtendedPrecission::operator<(ExtendedPrecission num)
{
    if (*this > num || *this == num)
        return false;
    return true;
}

ExtendedPrecission ExtendedPrecission::operator*(ExtendedPrecission num)
{
    ExtendedPrecission result(0.0);

    if (this->sign == num.getSign())
        result.sign = 0;
    else
        result.sign = 1;

    result.exponent = this->exponent + num.getExponent() - 16383;

    __uint128_t resultMantissa = 0;
    __uint128_t thisMantissa = this->mantissa;
    uint64_t mask = 0b1;
    std::cout << std::endl;
    resultMantissa += (num.getMantissa() & mask) * this->mantissa;
    for (int i = 1; i < 64; i++)
    {
        resultMantissa += ((num.getMantissa() >> i) & mask) * (thisMantissa << i);
    }

    if ((resultMantissa >> 126) > 1)
        result.exponent += 1;

    result.mantissa = resultMantissa >> 64;

    return result;
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