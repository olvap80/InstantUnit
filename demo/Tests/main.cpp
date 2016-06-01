#include "InstantUnit/InstantUnit.h"
#include <iostream>


int fcn(int val){
   return 2*val;
}

TEST("Test 1")
{
    std::cout << "InstantUnit 111" << std::endl;

    //EXPECT(true);
    //EXPECT(10) == 10;
    //EXPECT(fcn)(25) == 50;
}

TEST("Test 2")
{
    std::cout << "InstantUnit 222" << std::endl;
}

int main()
{
    /*InstantUnit::ContextBeforeTestSuite* cb = nullptr;
    InstantUnit::ContextAfterTestSuite* ca = static_cast<InstantUnit::ContextAfterTestSuite*>(cb);*/

    InstantUnit::RunTests();
    return 0;
}
