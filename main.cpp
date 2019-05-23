#include "ExtendedPrecission.hpp"

int main()
{
    ExtendedPrecission ep(2.0);
    std::cout << "Exponent: " << ep.getExponent() << std::endl
              << "Mantissa: " << ep.getMantissa() << std::endl
              << "2" << std::endl
              << ep.toString() << std::endl;

    ExtendedPrecission ep2(2.0);
    std::cout << "Exponent: " << ep2.getExponent() << std::endl
              << "Mantissa: " << ep2.getMantissa() << std::endl
              << "2" << std::endl
              << ep2.toString() << std::endl;

    ExtendedPrecission sum = ep + ep2;
    std::cout << "sumExponent: " << sum.getExponent() << std::endl
              << "sumMantissa: " << sum.getMantissa() << std::endl
              << "wynik: " << std::endl
              << sum.toString() << std::endl;

    ExtendedPrecission dif = ep - ep2;
    std::cout << "subExponent: " << sum.getExponent() << std::endl
              << "subMantissa: " << sum.getMantissa() << std::endl
              << "wynik: " << std::endl
              << sum.toString() << std::endl;

    ExtendedPrecission first(13.5);
    ExtendedPrecission second(21.5);
    ExtendedPrecission result = first * second;
    ExtendedPrecission right_result(13.5 * 21.5);
    std::cout << "===============================================" << std::endl;
    std::cout << "first_sign: " << first.getSign() << std::endl;
    std::cout << "first_exp: " << first.getExponent() << std::endl;
    std::cout << "first_man: " << first.getMantissa() << std::endl;
    std::cout << "first_str: " << first.toString() << std::endl;
    std::cout << "second_sign: " << second.getSign() << std::endl;
    std::cout << "second_exp: " << second.getExponent() << std::endl;
    std::cout << "second_man: " << second.getMantissa() << std::endl;
    std::cout << "second_str: " << second.toString() << std::endl;
    std::cout << "result_sign: " << result.getSign() << std::endl;
    std::cout << "result_exp: " << result.getExponent() << std::endl;
    std::cout << "result_man: " << result.getMantissa() << std::endl;
    std::cout << "result_str: " << result.toString() << std::endl;
    std::cout << "right__str: " << right_result.toString() << std::endl;
    return 0;
}