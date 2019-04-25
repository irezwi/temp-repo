#include "ExtendedPrecission.hpp"

int main()
{
    ExtendedPrecission ep(12.5);
    std::cout << "12.5" << std::endl
              << ep.toString() << std::endl;

    ExtendedPrecission ep2(127.8);
    std::cout << "127.8" << std::endl
              << ep2.toString() << std::endl;
    return 0;
}