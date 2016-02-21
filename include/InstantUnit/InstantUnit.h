/** @file InstantUnit.h
    @brief Mimimalistic (header only) Unit Test framework for C++11 and above.

To use just include this header.
There are no other dependencies then standard libraries.

Simplest usage sample:

@code

    #include "InstantUnit/InstantUnit.h"
    #include <vector>

    TEST("My test name"){
        //Setup statements (local for this test, not shared with others)
        std::vector<int> v;
        v.push_back(10);
        v.push_back(20);
        v.push_back(31);

        //On failure those ASSERTs will end current Test Case
        ASSERT( !v.empty() );
        ASSERT(v.size()) == 3;

        //EXPECTs will still continue Test, even after failure
        EXPECT(v.front()) == 10;
        EXPECT(v[1]) == 20;
        EXPECT(v.back()) == 31
    }

    MAIN_RUN_TESTS

@endcode

Here:
    -TEST   defines simple named Test Case, here Setup/Teardown are not shared
            with others. You can define as many TEST items as you like,
            in different files of the project and they all will be automatically
            registered by InstantUnit framework for execution.

    -ASSERT shall cause Test Case to complete immediately when corresponding
            condition fails, so the rest of the failed Test Case is skipped.

    -EXPECT (in contrast) shall just mark the surrounding Test Case as "failed",
            but the Test Case execution continues.

    -MAIN_RUN_TESTS is a macro to be used in place of main.
                    Instead of placing MAIN_RUN_TESTS to run tests,
                    you can just call to InstantUnit::RunTests()
                    from any place you like.

Note1: Both ASSERT and EXPECT macro are intended to produce test output
and update test execution statistics.

Note2: there is also a set of SANITY_* macro to ensure "critical conditions",
see documentation below.

You can write a condition to be checked directly inside the ASSERT
or EXPECT macro:

@code
    EXPECT(x > 3);
    EXPECT(y >= 2.9 && y <= 3.1);
@endcode

but when condition fails there will be no additional information "why failed".
TODO: output sample assuming x = 2 and y = 4.2.
Use following syntax to make InstantUnit aware of the values being tested:

@code
    EXPECT(x) > 3; //here InstantUnit is aware that we are comparing x with 3
    EXPECT_CALL(InstantUnit::IsNear, y, 3, 0.1); //all parameters are traceable
@endcode

Now value of x will go to the test output, and arguments passed to predicate
are printed to simplify debugging on failure.
TODO: output sample assuming x = 2 and y = 4.2.
Note: InstantUnit::IsNear, from the sample above, is a predicate built
into the InstantUnit framework, but you can write your own:

@code
    bool IsOdd(int v){
        return v & 0x1
    }
    ...
    TEST("Test OddGenerator"){
        OddGenerator g;

        ASSERT_CALL(IsOdd, g.GetNext());
        ASSERT_CALL(IsOdd, g.GetNext());
        ASSERT_CALL(IsOdd, g.GetNext());
    }
    ...
@endcode

More complex approach: add shared common Setup and Teardown. Please notice how
those Setup and Teardown are located - there is no need to inherit from any
classes, override any functions, etc
    - just write your statements according to the template below:

@code
    TEST_SUITE("My Suite name"){ // ... __LINE__##_Run(NU::TestCaseRunner& NU_TestCaseRunner) {
        //optional Setup code to be executed before every Test Case in the Suite
        ... //declare variables, setup environment, etc

        TEST_CASE("My TC 1"){ //NU_TestCaseRunner(__FILE__, __LINE__, "TC 1") ->* [&]() {
           ... //Checks and asserts (IU_CHECK, IU_IS_EQUAL, UI_IS_CLOSE etc)
        };
        TEST_CASE("My TC 2"){ //NU_TestCaseRunner(__FILE__, __LINE__, "TC 2") ->* [&]() {
           ... //Checks and asserts (IU_CHECK, IU_IS_EQUAL, UI_IS_CLOSE, etc)
        };
        ...//etc

        //optional Teardown code to be executed after every Test Case
        ... //optionally cleanup the environment, if needed
        //Note: (teardown is automatically executed even in case of exception!)
    }
@endcode

Here Test Suite is not only a collection of Test Cases but also a way
to surround every Test Case in that collection with the Setup and Teardown code.

Setup code from the Test Suite is executed before each Test Case like when
it was pasted directly before the Test Case body.
You can declare variables in Setup and
they will be visible from the Test Case body.

Teardown code from the Test Suite is executed after each Test Case like when
it is pasted directly after the test case body.
Teardown code is still automatically executed even in case of exception in
a Test Case body.
Every variable declared in the Setup code is visible from the Teardown code.

Practical sample:
@code
    #include "InstantUnit/InstantUnit.h"
    #include <vector>

    TEST_SUITE("General std::vector teting"){
        //Setup code to be executed before every Test Case in the Suite
        std::vector<int> v;
        v.push_back(10);
        v.push_back(20);
        v.push_back(31);
        //Note: now v with three filled items will be visible to every TEST_CASE below

        SANITY_FOR_TEST_SUITE( !v.empty() );

        TEST_CASE("Test const operations"){
            SANITY_FOR_TEST_CASE(v.size()) == 3;

            //Expects will still continue Test after failure
            EXPECT(v.front()) == 10;
            EXPECT(v[1]) == 20;
            EXPECT(v.back()) == 31
        }
        TEST_CASE("Test clear method"){
            v.clear();
            ASSERT( v.empty() );
            ASSERT(v.size()) == 0;
        }
        TEST_CASE("Test pop_front"){
            //...
        }
        //etc...
    }

    MAIN_RUN_TESTS
@endcode

Disclaimer: all samples here are just for illustration purposes and they are
            not intended to demonstrate such techniques as 100% coverage, etc.

                         * * *


Copyright (c) 2015, Pavlo M, https://github.com/olvap80
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of InstantJSON nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef INSTANTUNIT_HDR_
#define INSTANTUNIT_HDR_

//depends only on standard stuff:

#include <cstddef>
#include <cstdlib>
#include <cmath>
#include <stdexcept>
#include <string>
#include <chrono>
#include <functional>


//#############################################################################
//Macros for creating and grouping tests
//#############################################################################

///Simple named Test (standalone Test Case without shared Setup/Teardown stuff)
/** Place Test code in braces after TEST macro.
    Such test suite will be a part of the "DEFAULT" Test Session*/
#define TEST(testNameString) \
    /*Class to run test code in*/ \
    class IU_CAT_ID(Test_,__LINE__): private InstantUnit::details::SimpleRunner{ \
        /*Test code will go here, called automatically from SimpleRunner*/ \
        virtual void DoTest() override; \
        \
        virtual const char* TestName() override { return testNameString; }\
    }; \
    /*Corresponding static (!) instance to be part of the run*/ \
    static IU_CAT_ID(Test_,__LINE__) IU_CAT_ID(Instance_,__LINE__); \
    /*Test body will follow below*/ \
    void IU_CAT_ID(Test_,__LINE__)::DoTest()

///Named group of Test Cases together to support common Setup/Teardown
/**Place Test Setup at top, then Test Cases and then Teardown at bottom.
   See example above on how to use this stuff.*/
#define TEST_SUITE(testSuiteNameString) \
    class testSuiteNameString##_TestSuite:

///Single Test Case item in the Test Suite (share Setup/Teardown with others)
#define TEST_CASE(testCaseNameString)


///Mark an expression or call as being subject to "assert test"
/** Causes Test Case to complete immediately on "verify fail"
  (when corresponding condition fails),
  the rest of the failed test is skipped.
  Following usages are possible:
  @code
    ASSERT(expression);                       //verify expression is not 0
    ASSERT(expression) == valueToCompareWith; //verify expression == valueToCompareWith
    ASSERT(expression) != valueToCompareWith; //verify expression != valueToCompareWith
    ASSERT(expression) <  valueToCompareWith; //verify expression <  valueToCompareWith
    ASSERT(expression) <= valueToCompareWith; //verify expression <= valueToCompareWith
    ASSERT(expression) >  valueToCompareWith; //verify expression >  valueToCompareWith
    ASSERT(expression) >= valueToCompareWith; //verify expression)>= valueToCompareWith
  @endcode
  Note1: only comparison operations are allowed
         and ASSERT(expression) always goes first.
  Note2: ASSERT can only be placed inside TEST or TEST_CASE */
#define ASSERT()
//

///Mark an expression or call as being subject of "expect test"
/** Just mark surrounding Test Case as failed on "verify fail",
    but Test Case execution continues.
    Usage is similar to ASSERT from above */
#define EXPECT()
//



#define SANITY_FOR_TEST
#define SANITY_FOR_TEST_CASE

#define SANITY_FOR_TEST_SUITE

/*
once SANITY has failed this means for the InstantUnit that the host process
is corrupted/broken or there is no sense to continue, and now no more tests can be executed
in this process instance,  */

///Check for conditions that break/corrupt entire process on failure
/** Intended to make "Fatal" check macro for "critical condition checks".
 Never goes to output for "passed" case and does not affect statistics.
 Failed SANITY check means entire test session is broken and cannot continue.
 Once SANITY failed, no more test can be executed in the process (exit process).
 Usage is similar to ASSERT and EXPECT from above.*/
#define SANITY_FOR_TEST_SESSION()


///Specify this macro to not define main manually
#define MAIN_RUN_TESTS \
    int main(int argc, char *argv[]) \
    { \
        return InstantUnit::RunTests(argc, argv); \
    }

//#############################################################################
//Predefined predicates, running tests options, reporting support
//#############################################################################

namespace InstantUnit{

//=============================================================================
//Predefined verifiers --------------------------------------------------------

///Test double values are equal with precission
inline bool IsNear(double val1, double val2, double precission){
    return std::fabs(val1 - val2) <= precission;
}

//TODO: IsNear for complex numbers (and vectors via iterators? - euclidian distance)

///Test value is within limits (fromInclusive <= val <= toInclusive)
template<class T>
inline bool IsBetween(T val, T fromInclusive, T toInclusive){
    return false;
}


//=============================================================================
//Running tests ---------------------------------------------------------------

///Execute all known Test Suites as part of the full Test Session
/** Test Suites to be run also include DEFAULT Test Suite.
    @returns true when all executed tests passed, false otherwise
*/
bool RunTests();

///Add command line support for running tests
/** @returns EXIT_SUCCESS when all executed tests passed, EXIT_FAILURE otherwise
TODO: describe command line options here
Sample usage
@code
    #include "InstantUnit/InstantUnit.h"

    ...

    int main(int argc, char *argv[])
    {
        return InstantUnit::RunTests(argc, argv);
    }
@endcode
Note: use MAIN_RUN_TESTS instead of main that just redirects to RunTests*/
int RunTets(int argc, char *argv[]);

///Run Test Suite by name
bool RunTestSuite(const std::string& suiteName);


///Execute all Tests/Test Cases that pass predicate by name
void RunTests(
    const std::function<
        bool(
            const std::string& testSuiteName,
            const std::string& testCaseName
        )
    >& testCaseFilter
);


//=============================================================================
//Contexts for reporting ------------------------------------------------------

///All Tests in the process are executed in the context of Test Session
class ContextBeforeTestSession{
public:
    ///Name of the test session (by default is autogenerated from date)
    virtual std::string TestSessionName() const = 0;

    ///test session start date
    virtual std::chrono::system_clock::time_point TestSessionStartTimePoint() const = 0;

    ///Number of all test suites found in this test session
    /** Available before actual execution*/
    virtual unsigned TestSuitesTotal() const = 0;
};

///All Tests in the process are executed in the context of Test Session
class ContextAfterTestSession: public ContextBeforeTestSession{
public:
    //test session end date
    virtual std::chrono::system_clock::time_point TestSessionEndTimePoint() const = 0;

    ///Total number of all test cases in all test suites
    virtual unsigned TestCasesTotal() const = 0;

    ///Total number or test cases passed
    virtual unsigned TestCasesPassed() const = 0;

    ///Total number or test cases failed
    virtual unsigned TestCasesFailed() const = 0;

    ///Total execution time of all suites
    virtual unsigned ExecutionTimeTotalSeconds() const = 0;

    //collection of test suites ?
};

///Information available before test suite execution starts
/**Test Suite is a container for Test Cases with shared Setup/Teardown
   Setup is executed before each test case.
   Teardown is executed after each test case.*/
class ContextBeforeTestSuite{
public:
    ///Get name provided to corresponding TestSuite macro
    /**Note: all TEST macro go to "default test suite"*/
    virtual std::string TestSuiteName() const = 0;

};

///Information available after test suite has been executed
/**Test Suite is a container for Test Cases with shared Setup/Teardown
   Setup is executed before each test case.
   Teardown is executed after each test case.*/
class ContextAfterTestSuite: public ContextBeforeTestSuite{
public:

    ///Number of all test cases in this test suite
    virtual unsigned TestCasesTotal() const = 0;

    ///Number of all test cases passed in this test suite
    virtual unsigned TestCasesPassed() const = 0;

    ///number of all test cases failed in this test suite
    virtual unsigned TestCasesFailed() const = 0;
};

///Information available before test case execution starts
/** Test case is an item in the Test Suite with set of checks */
class ContextBeforeTestCase{
public:
    ///Access to containing Test Suite
    virtual const ContextBeforeTestSuite& TestSuite() const = 0;

    ///Name used in corresponding TEST or TEST_CASE macro
    virtual std::string TestCaseName() const = 0;

};

///Information available after test case has been executed
/** Test case is an item in the Test Suite with set of checks */
class ContextAfterTestCase: public ContextBeforeTestCase{
public:
};

///
class ContextForStatement{
public:
    ///Access entire testing context (whole test or test case)
    virtual const ContextBeforeTestCase& TestCase() const = 0;
};

///
class ContextForStep: public ContextForStatement{
public:
    ///
    virtual std::string StepText() const = 0;
};


///Target variable or expression being currently verified
class ContextForVerify: public ContextForStatement{
public:
    ///
    virtual bool IsPassed() const = 0;

    ///Text representation of entire expression to be verified
    virtual std::string ExpresionText() const = 0;

    ///
    virtual std::string ExpectedValue() const = 0;

    ///
    virtual std::string ActualValue() const = 0;
};


//class OnMessage
//class OnTrace


//Progress reporting ----------------------------------------------------------

///Report tests execution progress
class Reporter{
public:
    ///Called before test step is being executed
    virtual void OnStep(const ContextForStep& contextForStep) = 0;

    ///Called to place message to output stream
    virtual void Message(const std::string& textMessage) = 0; //TBD

    ///Called when single test step failed
    virtual void OnVerify(const ContextForVerify& contextForVerify) = 0;


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

} //namespace InstantUnit

/*=============================================================================
*  Implementation details
*=============================================================================*/

//Expand and concatenate macro arguments into combined identifier
#define IU_CAT_ID(a,b) IU_CAT_ID_EXPANDED_HELPER(a,b)
//helper macro to concatenate expanded macro arguments
#define IU_CAT_ID_EXPANDED_HELPER(a,b) a##b

//helper macro to determine if we assert value or function call
#define IU_GET_ARG_COUNT(...) \
    IU_GET_ARG_COUNT_HELPER(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,  4,  3,  2, 1)
//helper macro (used by IU_HAS_MANY_ARGUMENTS to obtain shifted argument)
#define IU_GET_ARG_COUNT_HELPER(     _1, _2, _3, _4, _5, _6, _7, _8,_9,_10,_11,_12,_13,_14,_15,_16, res) res

namespace InstantUnit{

namespace details{
    ///Exception to be used to signal that test case failed
    class TestCaseFailed{};

    ///
    class Context{
    public:
        //void Step
    };

    ///Wrap value being tested
    template<class T>
    struct ValueWrap{
        ValueWrap(const T& val):value(val){}
        const T& value;

        template<class T2>
        std::function<void(Context&)> operator<(const T2& compareWith) const{
            return [&] (Context&){
                //TODO: report actual and expected
                if( value < compareWith ){
                    //TODO: report how passed
                }
                //TODO: report why failed
            };
        }
    };

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
        virtual bool Run() = 0;

    private:
        friend bool InstantUnit::RunTests();
    };

    ///Base class to be executed while running tests
    class SimpleRunner: public Runnable{
    protected:
        ///Method called to do actual testing
        virtual void DoTest() = 0;

        ///Name for test being runned
        virtual const char* TestName()  = 0;

    private:
        ///Method to be runned for all found Runnable instances
        virtual bool Run(){
            try{
                DoTest();
                return true;
            }
            catch(const TestCaseFailed&){
            }
            catch(const std::exception& e){
            }
            catch(...){
            }
            return false;
        }

    };
} //namespace InstantUnit::details

inline bool RunTests(){
    bool allPassed = true;
    details::InstanceListBase<details::Runnable>::ForEachInstance(
        [&](details::Runnable* ptr){
            allPassed = allPassed && ptr->Run();
        }
    );
    return allPassed;
}

} //namespace InstantUnit

#endif /*INSTANTUNIT_HDR_*/
