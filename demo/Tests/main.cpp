#include "InstantUnit/InstantUnit.h"
#include <iostream>


IU_TEST("Test 1")
{
    std::cout << "InstantUnit 111" << std::endl;
}

IU_TEST("Test 2")
{
    std::cout << "InstantUnit 222" << std::endl;
}

int main()
{

    InstantUnit::Run();
    return 0;
}
