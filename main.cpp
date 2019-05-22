#include "ExtendedPrecission.hpp"

int main()
{
    ExtendedPrecission ep(2.0);
    std::cout << "Exponent: " << ep.getExponent() <<std::endl
            << "Mantissa: " << ep.getMantissa() <<std::endl
            << "2" << std::endl
            << ep.toString() << std::endl;

    ExtendedPrecission ep2(2.0);
    std::cout << "Exponent: " << ep2.getExponent() <<std::endl
            << "Mantissa: " << ep2.getMantissa() <<std::endl
            << "2" << std::endl
            << ep2.toString() << std::endl;

    ExtendedPrecission sum = ep+ep2;
    std::cout << "sumExponent: " << sum.getExponent() << std::endl
            << "sumMantissa: " << sum.getMantissa() << std::endl
            << "wynik: " << std::endl
            << sum.toString() << std::endl;

    ExtendedPrecission dif = ep-ep2;
    std::cout << "subExponent: " << sum.getExponent() << std::endl
            << "subMantissa: " << sum.getMantissa() << std::endl
            << "wynik: " << std::endl
            << sum.toString() << std::endl;

    return 0;
}