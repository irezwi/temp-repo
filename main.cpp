#include "ExtendedPrecission.hpp"

int main()
{
    ExtendedPrecission ep(4.0);
    std::cout << "4" << std::endl
              << ep.toString() << std::endl;

    ExtendedPrecission ep2(-2.0);
    std::cout << "-2" << std::endl
              << ep2.toString() << std::endl;

    ExtendedPrecission sum = ep+ep2;
    std::cout << "wynik: " << std::endl
              << sum.toString() << std::endl;

    return 0;
}