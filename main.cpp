#include "ExtendedPrecission.hpp"

int main()
{
    ExtendedPrecission ep(12.5);
    std::cout << ep.toString() << std::endl;

    ExtendedPrecission ep2(-12456.1244624246);
    std::cout << ep2.toString() << std::endl;
    return 0;
}
