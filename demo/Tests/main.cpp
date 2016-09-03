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

TEST_SUITE("Test Suite 1")
{
    std::cout << "InstantUnit 333" << std::endl;
}

int main()
{
    /*InstantUnit::TestSuiteContextBefore* cb = nullptr;
    InstantUnit::TestSuiteContextAfter* ca = static_cast<InstantUnit::TestSuiteContextAfter*>(cb);*/

    InstantUnit::RunTests();
    return 0;
}
