/** @file InstantUnit.h
    @brief Mimimalistic (header only) Unit Test framework for C++11 and above.

Simple usage sample (single test, all in one):
@code

    TEST("My test name"){
        //Setup statements
        std::vector<int> v;
        v.push_back(10);
        v.push_back(20);
        v.push_back(31);

        //Asserts or expectations
        ASSERTION(v.empty()) == false;
        ASSERTION(v.size) == 3;
        EXPECTATION(v.front()) == 10;
        EXPECTATION(v.back()) == 31
    }

@endcode
Note: if condition fails then corresponding ASSERTION causes test to complete
      immediately, the rest of the failed test is skipped.
      In contrast conditions marked with EXPECTATION just mark surrounding test
      case as failed, but execution continues.

You can write condition to be checked directly inside of ASSERTION
or EXPECTATION macro:
@code
    EXPECTATION(x > 3);
    EXPECTATION(y >= 2.9 && y <= 3.1);
@endcode
but when condition fails there will be no additional information why.
Use following syntax:
@code
    EXPECTATION(x) > 3;
    EXPECTATION(y, CloseTo, 3, 0.1);
@endcode
Now value of x will go tho the output, and

EXPECTATION(v.back(), IsPrime)

More complex sample (Add common setup and teardown, notice how those
setup and teardown are located - there is no need to inherit from any
classes, override any functions, etc):
@code

    TEST_SUITE("My Suite name"){ // ... __LINE__##_Run(NU::TestCaseRunner& NU_TestCaseRunner) {
        //optional setup code to be executed before every test case
        ... //declare variables, setup environment, etc

        TEST_CASE("My TC 1"){ //NU_TestCaseRunner(__FILE__, __LINE__, "TC 1") ->* [&]() {
           ... //Checks and asserts (IU_CHECK, IU_IS_EQUAL, UI_IS_CLOSE etc)
        };
        TEST_CASE("My TC 2"){ //NU_TestCaseRunner(__FILE__, __LINE__, "TC 2") ->* [&]() {
           ... //Checks and asserts (IU_CHECK, IU_IS_EQUAL, UI_IS_CLOSE, etc)
        };
        ...//etc

        //optional teardown code to be executed after every test case
        ... //optionally cleanup the environment, if needed
        //Note: (teardown is automatically executed even in case of exception!)
    }

@endcode

Here test suite is not only a collection of test cases but also a way
to surround every test case in the collection with setup and teardown code.

Setup code from the test suite is executed before each test case like it was
pasted directly before the test case body. You can declare variables in setup
and they will be visible from the test case body.

Teardown code from the test suite is executed after each test case like it is
pasted directly after the test case body. Teardown code is still automatically
executed even in case of exception in a test case body.
Every variable declared in Setup code is visible from Teardown code.

*/

#ifndef INSTANTUNIT_HDR_
#define INSTANTUNIT_HDR_

#include <cstddef>
#include <stdexcept>
#include <string>

///Expand and concatenate macro arguments into combined identifier
#define IU_CAT_ID(a,b) IU_CAT_ID_EXPANDED_HELPER(a,b)
//helper macro to concatenate expanded macro arguments
#define IU_CAT_ID_EXPANDED_HELPER(a,b) a##b

///
#define IU_HAS_MANY_ARGUMENTS(...) \
      IU_HAS_MANY_ARGUMENTS_EXPAND_HELPER(__VA_ARGS__, true, true, true, true, true, true, true, true, true, true, false)
//helper macro to expand arguments
#define IU_HAS_MANY_ARGUMENTS_EXPAND_HELPER(...) IU_HAS_MANY_ARGUMENTS_EXPANDED_HELPER(__VA_ARGS__)
//helper macro (used by IU_HAS_MANY_ARGUMENTS to obtain shifted argument)
#define IU_HAS_MANY_ARGUMENTS_EXPANDED_HELPER(     _1,   _2,   _3,   _4    _5,   _6,   _7,   _8,   _9,  _10,  _11,   res) res

///Simple test
/** Place test code in braces after IU_TEST */
#define TEST(testNameString) \
    class IU_CAT_ID(Test_,__LINE__): private InstantUnit::details::SimpleRunner{ \
        virtual void DoTest(); \
    }; \
    IU_CAT_ID(Test_,__LINE__) IU_CAT_ID(Instance_,__LINE__); \
    void IU_CAT_ID(Test_,__LINE__)::DoTest()

///Group test cases together to support common setup/teardown
/**Place test setup, then test cases and then teardown.
   See example above on how to use this stuff.*/
#define TEST_SUITE(testSuiteNameString) \
    class testSuiteNameString##_TestSuite:

///Single test case item in test duite (share setup/teardown) with others
#define TEST_CASE(testCaseNameString)


///Mark variable or entire expression as being subject to ""
#define ASSERTION()

#define EXPECTATION
/*
#define IU_CHECK(textExplain, conditionToCheck)

#define IU_IS_EQUAL(expectedValue, actualValue)

#define UI_IS_CLOSE

#define UI_CHECK_THROWS
*/
//TODO


namespace InstantUnit{

///Execute all known Test Suites
void Run();

///Target variable or expression being currently tested
class Target{
public:
    ///Text representation
    std::string Text() const {}
private:
    std::string text;
};

class Reporter{
public:
    ///Called when test case is pased
    virtual void OnTestPassed(const std::string& testName) = 0;
    ///Called when test case is failed
    virtual void OnTestFailled(const std::string& testName) = 0;

    ///Uncaught exception (derived from std::exception) is detected
    virtual void OnUncaughtException(
        const std::string& testName,
        const std::exception& e
    ) = 0;
    ///Uncaught exception (not derived from std::exception) is detected
    virtual void OnUnknownUncaughtException(const std::string& testName) = 0;
};


/*=============================================================================
*  Implementation details
*=============================================================================*/

namespace details{
    ///Exception to be used to signal that test case failed
    class TestCaseFailed{};

    //Implementation details

    ///Helper to create list of static/global object instances
    /**Every derived instance will be a part of the global list.
     Access to all instances via InstanceListBase<T>::ForEachInstance.
     Allocate derived instances as global/static variables only.*/
    template<class DerivedT>
    class InstanceListBase{
    public:
        ///Invoke callback for each known instance (in the order as they were added)
        /**Global list of all existing instances */
        template<class FcnToCallOnEachPtr>
        static void ForEachInstance(FcnToCallOnEachPtr fcn){
            for(InstanceListBase* ptr = head; ptr != nullptr; ptr = ptr->next){
                fcn((DerivedT*)ptr);
            }
        }

    protected:
        ///Add this item to the end of the list (preserve order)
        InstanceListBase() : next(nullptr) {
            *pointerToTailPtr = this; //this will be new end of list
            pointerToTailPtr = &this->next; //set up new tail to update next time
        }

    private:
        ///All known instances list head
        /**use simple pointer chain to prevent problems with initialization order*/
        static InstanceListBase* head;
        ///Remember current tail position here
        static InstanceListBase* *pointerToTailPtr;

        ///Pointer to the next known instance
        InstanceListBase* next;

        //ban all dynamic allocation stuff
        void* operator new(std::size_t);          // standard new
        void* operator new(std::size_t, void*);   // placement new
        void* operator new[](std::size_t);        // array new
        void* operator new[](std::size_t, void*); // placement array new
    };

    template<class DerivedT>
    InstanceListBase<DerivedT>* InstanceListBase<DerivedT>::head = nullptr;
    template<class DerivedT>
    InstanceListBase<DerivedT>* *InstanceListBase<DerivedT>::pointerToTailPtr = &InstanceListBase<DerivedT>::head;


    ///Base class for object to be executed while running tests
    class Runnable: public InstanceListBase<Runnable>{
    protected:
        ///Method to be runned for all found Runnable instances
        virtual void Run() = 0;

    private:
        friend void InstantUnit::Run();
    };

    ///Base class to be executed while running tests
    class SimpleRunner: public Runnable{
    protected:
        ///Method called to do actual testing
        virtual void DoTest() = 0;

    private:
        ///Method to be runned for all found Runnable instances
        virtual void Run(){
            try{

                DoTest();
            }
            catch(const TestCaseFailed&){
            }
            catch(const std::exception& e){
            }
            catch(...){
            }
        }


        friend void Run();
    };
}

inline void Run(){
    details::InstanceListBase<details::Runnable>::ForEachInstance(
        [](details::Runnable* ptr){
            ptr->Run();
        }
    );
}


} //namespace InstantUnit

#endif /*INSTANTUNIT_HDR_*/
