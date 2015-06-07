#include "InstantUnit/InstantUnit.h"
#include <iostream>


TEST("Test 1")
{
    //UI_ASSERT("");
    std::cout << "InstantUnit 111" << std::endl;
}

TEST("Test 2")
{
    std::cout << "InstantUnit 222" << std::endl;
}

int main()
{

    InstantUnit::RunTests();
    return 0;
}
