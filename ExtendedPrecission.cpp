#include "ExtendedPrecission.hpp"
//#define DEBUG 1 // comment this line to skip debugging output

void ConvertToIeeeExtended(long double num, char *bytes);
void printBinary(char c);

std::string ExtendedPrecission::toString()
{
    std::string s = "";
    for (auto &c : this->byteBuffer)
    {
        for (int i = 7; i >= 0; --i)
        {
            s += ((c & (1 << i)) ? '1' : '0');
        }
    }
    return s;
}

ExtendedPrecission::ExtendedPrecission(long double num)
{
    ConvertToIeeeExtended(num, this->byteBuffer);
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

    // e = expon;
    // s = expon >> 15;
    // m = (hiMant << 32) + loMant;

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