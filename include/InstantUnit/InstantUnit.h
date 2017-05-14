/** @file InstantUnit.h
    @brief Minimalistic (header only) Unit Test framework for C++11 and above.

To use just include this header.
There are no other dependencies then standard libraries,
no installation needed, simple copy-paste of this single file will also work :)

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
        ASSERT(v.capacity()) > 0;

        //EXPECTs will still continue Test, even after failure
        EXPECT(v.front()) == 10;
        EXPECT(v[1]) == 20;
        EXPECT(v.back()) == 31
    }

    MAIN_RUN_TESTS

@endcode

Here:
    -TEST   defines simple named Test Case, where Setup/Teardown are not shared
            with others. You can define as many TEST items as you like,
            in different files of the project and they all will be automatically
            registered by InstantUnit framework for execution.

    -ASSERT shall report failed condition and cause Test Case to complete
            immediately if corresponding condition fails,
            so the rest of the failed Test Case is skipped.

    -EXPECT (in contrast) on failure shall just mark the surrounding Test Case
            as "failed", but the Test Case execution continues.

    -MAIN_RUN_TESTS is a macro to be used in place of main.
                    Instead of placing MAIN_RUN_TESTS to run tests,
                    you can just call to InstantUnit::RunTests()
                    from any place you like.

Note1: Both ASSERT and EXPECT macro are intended to produce test output
       and update test execution statistics.

Note2: there are also SANITY_CHECK and CRITICAL_CHECK macro to ensure
       "critical conditions", that do not produce any test output when passed,
       but scream loudly on failure.
       More details in corresponding documentation below.


It is possible to write a condition to be checked directly inside the ASSERT or EXPECT:

@code
    EXPECT(x > 3);
    EXPECT(y >= 2.9 && y <= 3.1);
@endcode

but when condition fails there will be no additional information "why failed".
TODO: output sample assuming x = 2 and y = 4.2.

Use following syntax to make InstantUnit aware of the values being tested:

@code
    EXPECT(x) > 3; //here InstantUnit is aware that we are comparing x with 3
    EXPECT(InstantUnit::IsNear)(y, 3, 0.1); //all parameters are traceable
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

        ASSERT(IsOdd)(g.GetNext());
        ASSERT(IsOdd)(g.GetNext());
        ASSERT(IsOdd)(g.GetNext());
    }
    ...
@endcode

More complex (but very useful) approach
is to add shared common Setup and Teardown.
Please notice how those Setup and Teardown are located:
- there is no need to inherit from any classes, override any functions, etc
- just write your statements according to the template below:

@code
    TEST_SUITE("My Suite name"){ // ... __LINE__##_Run(NU::TestCaseRunner& NU_TestCaseRunner) {
        //optional Setup code to be executed before every Test Case in the Suite
        ... //declare variables, setup environment, etc

        TEST_CASE("My TC 1"){ //NU_TestCaseRunner(__FILE__, __LINE__, "TC 1") ->* [&]() {
           ... //Expects and asserts
        };
        TEST_CASE("My TC 2"){ //NU_TestCaseRunner(__FILE__, __LINE__, "TC 2") ->* [&]() {
           ... //Expects and asserts
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
it was copy-pasted directly before the Test Case body.
You can declare variables in Setup and
they will be visible from the Test Case body.

Teardown code from the Test Suite is executed after each Test Case like when
it is copy-pasted directly after the test case body.
Teardown code is still automatically executed even in case of exception in
a Test Case body.
Every variable declared in the Setup code is visible from the Teardown code.

Sample:
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
            not intended to demonstrate such things as "100% coverage", etc.

                         * * *


Copyright (c) 2015-2017, Pavlo M, https://github.com/olvap80
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
#include <type_traits>
#include <set>

//______________________________________________________________________________
//##############################################################################
//Macros for creating and grouping tests
//##############################################################################

///Simple named Test (standalone Test Case without shared Setup/Teardown stuff)
/** Place Test code in braces after TEST macro.
    Such test suite will be a part of the "DEFAULT" Test Suite*/
#define TEST(testNameString) \
    /*Class to run test code in*/ \
    class IU_CAT_ID(Test_,__LINE__): \
        private InstantUnit::details::SimpleStandaloneTestRunner \
    { \
    public: \
        IU_CAT_ID(Test_,__LINE__)( \
            const std::string& fileWherePlaced, \
            unsigned lineWhereStarts \
        ) \
            :   SimpleStandaloneTestRunner(fileWherePlaced, lineWhereStarts) {} \
    protected: \
        /*Test code will go here, called automatically from SimpleStandaloneTestRunner*/ \
        void Runner_DoTest(bool& allAssertsAndExpectsPassedFlag) override; \
        \
        const char* Runner_CurrentTestName() const override \
            { return testNameString; } \
    }; \
    /*Corresponding static (!) instance to be part of the run*/ \
    static IU_CAT_ID(Test_,__LINE__) IU_CAT_ID(TestInstance_,__LINE__)(__FILE__, __LINE__); \
    /*Test body will follow below*/ \
    void IU_CAT_ID(Test_,__LINE__)::Runner_DoTest(bool& allAssertsAndExpectsPassedFlag)

///Named group of Test Cases tied together to support common Setup/Teardown
/**Place Test Setup at top, then Test Cases and then Teardown at bottom.
   See example above on how to use this stuff.*/
#define TEST_SUITE(testSuiteNameString) \
    /*Class to run contained test cases in*/ \
    class IU_CAT_ID(TestSuite_,__LINE__): \
        private InstantUnit::details::TestSuiteRunner \
    { \
    public: \
        IU_CAT_ID(TestSuite_,__LINE__)( \
            const std::string& fileWherePlaced, \
            unsigned lineWhereStarts \
        ) \
            :   TestSuiteRunner(fileWherePlaced, lineWhereStarts) {} \
    protected: \
        /*Test code will go here, called automatically from SimpleStandaloneTestRunner*/ \
        void Runner_DoNextTest(const Runner_OnTestCase& runner_OnTestCase) override; \
        \
        const char* Runner_CurrentTestSiuteName() const override \
            { return testSuiteNameString; } \
    }; \
    /*Corresponding static (!) instance to be part of the run*/ \
    static IU_CAT_ID(TestSuite_,__LINE__) IU_CAT_ID(TestSuiteInstance_,__LINE__)(__FILE__, __LINE__); \
    /*Test Suite body will follow below*/ \
    void IU_CAT_ID(TestSuite_,__LINE__)::Runner_DoNextTest(const Runner_OnTestCase& runner_OnTestCase)

///Single Test Case item in the Test Suite (share Setup/Teardown with others)
#define TEST_CASE(testCaseNameString)


///Mark an expression as being subject to "assert test"
/** Causes Test Case to complete immediately on "verify fail"
  (when corresponding condition fails),
  the rest of the failed test is skipped.

  Following usages are possible:
  @code
    ASSERT(expression);                       //verify expression is not 0 (not false)
    ASSERT(expression) == valueToCompareWith; //verify expression == valueToCompareWith
    ASSERT(expression) != valueToCompareWith; //verify expression != valueToCompareWith
    ASSERT(expression) <  valueToCompareWith; //verify expression <  valueToCompareWith
    ASSERT(expression) <= valueToCompareWith; //verify expression <= valueToCompareWith
    ASSERT(expression) >  valueToCompareWith; //verify expression >  valueToCompareWith
    ASSERT(expression) >= valueToCompareWith; //verify expression >= valueToCompareWith
  @endcode

  It is also possible to call predicate or function using following syntax:
  @code
    ASSERT(f)(args);                       //verify f(args) is not 0 (not false)
    ASSERT(f)(args) == valueToCompareWith; //verify f(args) == valueToCompareWith
    ASSERT(f)(args) != valueToCompareWith; //verify f(args) != valueToCompareWith
    ASSERT(f)(args) <  valueToCompareWith; //verify f(args) <  valueToCompareWith
    ASSERT(f)(args) <= valueToCompareWith; //verify f(args) <= valueToCompareWith
    ASSERT(f)(args) >  valueToCompareWith; //verify f(args) >  valueToCompareWith
    ASSERT(f)(args) >= valueToCompareWith; //verify f(args) >= valueToCompareWith
  @endcode

  Note1: only comparison operations are allowed
         and ASSERT(expression) always goes first.
  Note2: ASSERT can only be placed inside TEST or TEST_CASE */
#define ASSERT(expression)
//



///Mark an expression as being subject of "expect test"
/** Just mark surrounding Test Case as failed on "verify fail",
    but Test Case execution continues.
    Usage is similar to ASSERT from above */
#define EXPECT(expression) \
    InstantUnit::


///Check for conditions that break/corrupt Test Case or Test Suite on failure
/**This kind of check can fail only in exceptional cases
   and is intended to ensure that test environment is not broken.
   The difference between SANITY_CHECK and ASSERT is that
   SANITY_CHECK does not write anything to output for "passed" condition.
   Also one can place SANITY_CHECK to Setup sections
   (and to Teardown section, but do this
    only after all the Teardown actions are actually done).*/
#define SANITY_CHECK(condition)

///Check for conditions that break/corrupt entire process on failure
/** Intended to make "Fatal" check macro for "critical condition checks".
 Never goes to output for "passed" condition and does not affect statistics.
 Failed SANITY check means entire test session is corrupted/broken
 and cannot continue.
 Once SANITY failed, no more test can be executed in the process (exit process).
 Usage is similar to ASSERT and EXPECT from above.*/
#define CRITICAL_CHECK(condition)




///Use this macro in the code to not define main manually
#define MAIN_RUN_TESTS \
    int main(int argc, char *argv[]) \
    { \
        return InstantUnit::RunTests(argc, argv); \
    }

//______________________________________________________________________________
//##############################################################################
//Predefined predicates, running tests options, reporting support, etc
//##############################################################################

namespace InstantUnit{


//==============================================================================
//Predefined verifiers (predicates) --------------------------------------------

///Test double values are equal with precission
inline bool IsNear(double val1, double val2, double precission){
    return std::fabs(val1 - val2) <= precission;
}

//TODO: IsNear for complex numbers (and vectors via iterators? - euclidian distance)

///Test value is within limits (fromInclusive <= val <= toInclusive)
template<class T>
inline bool IsBetween(T val, T fromInclusive, T toInclusive){
    return val >= fromInclusive && val <= toInclusive;
}



//=============================================================================
//Running tests ---------------------------------------------------------------

///Execute all known Test Suites as part of the full Test Session
/** Test Suites to be run also include "DEFAULT" Test Suite.
    @returns true when all executed tests passed, false otherwise */
bool RunTests();


///Add command line support for running tests
/** @returns EXIT_SUCCESS when all executed tests passed, EXIT_FAILURE otherwise.

TODO: describe command line options here (like those used to run part of tests)
Sample usage:
@code
    #include "InstantUnit/InstantUnit.h"
    ...

    int main(int argc, char *argv[])
    {
        return InstantUnit::RunTests(argc, argv);
    }
@endcode

Note: one can use MAIN_RUN_TESTS instead of main that just redirects to RunTests
      (just place MAIN_RUN_TESTS to one of your project files)*/
int RunTets(int argc, char *argv[]);


///Run specific Test Suite by name
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
//Contexts for reporting and statistics ---------------------------------------
//Note: usually one does not need to look below unless new reporter is needed


//forward declare implementation details referred
namespace details{
    class FullContextForTestSuite;
    class FullContextForTestCase;
}


///This context is created before activity starts
/** Known testing activities: TestSession, TestSuite, TestCase and Checker
    (Note: see corresponding contexts for details) */
class TestingActivityContextBefore{
public:

    ///Name for Testing Activity
    /** There if following mapping between Testing Activity type and Name:
        - for the TestSession by default is autogenerated from date.
        - for a TestSuite is provided by corresponding TEST_SUITE macro
          Note: all TEST macro go to "DEFAULT" Test Suite
        - for a TestCase is provided by corresponding TEST or TEST_CASE macro
        - for a checker - TBD*/
    virtual std::string Name() const = 0;

    ///Indicate that activity did not start due to some error
    /** Error before start, error while starting etc...
        When StartupError happens, the content of this activity will not execute
        and there will be no corresponding TestingActivityContextAfter*/
    virtual bool IsErrorOnStart() const = 0;
};

///This context is ready after activity completed
/** Known testing activities: TestSession, TestSuite, TestCase and Checker
    (Note: see corresponding contexts for details) */
class TestingActivityContextAfter: protected TestingActivityContextBefore{
public:
    //All stuff from TestingActivityContextBefore also available
    using TestingActivityContextBefore::Name;
    using TestingActivityContextBefore::IsErrorOnStart;

    ///"Passed" indicator for entire activity
    /** @returns true only when activity completed and passed, false otherwise

        - For TestSession "Passed" means all contained Test Suites passed.
        - For TestSuite "Passed" means all contained Test Cases passed.
        - For TestCase "Passed" means all contained checks passed.
        - For Checker "Passed" means corresponding condition is true.*/
    virtual bool IsPassed() const = 0;
};





///This context is created before activity starts and starts measuring time
/** Known time measured testing activities: TestSession, TestSuite
    (Note: see corresponding contexts for details) */
class TimeMeasuredTestingActivityContextBefore: public TestingActivityContextBefore{
public:
    ///This Testing Activity start time point
    virtual std::chrono::system_clock::time_point StartTimePoint() const = 0;

    ///This Testing Activity start time point (steady time counting)
    /**Using monotonic clock that will never be adjusted,
       The time points of this clock cannot decrease as physical time
       moves forward.
       This clock is not related to wall clock time (for example,
       it can be time since last reboot),
       and is most suitable for measuring intervals.*/
    virtual std::chrono::steady_clock::time_point StartSteadyTimePoint() const = 0;
};


///This context is ready after activity completed and has time measurements
/** Known time measured testing activities: TestSession, TestSuite
    (Note: see corresponding contexts for details) */
class TimeMeasuredTestingActivityContextAfter:
    public TestingActivityContextAfter,
    protected TimeMeasuredTestingActivityContextBefore{
public:
    //All stuff from TimeMeasuredTestingActivityContextBefore also available
    using TimeMeasuredTestingActivityContextBefore::StartTimePoint;
    using TimeMeasuredTestingActivityContextBefore::StartSteadyTimePoint;

    ///This Testing Activity end time point
    virtual std::chrono::system_clock::time_point EndTimePoint() const = 0;

    ///This Testing Activity end time point (steady time counting)
    /**Using monotonic clock that will never be adjusted,
       The time points of this clock cannot decrease as physical time
       moves forward.
       This clock is not related to wall clock time (for example,
       it can be time since last reboot),
       and is most suitable for measuring intervals.*/
    virtual std::chrono::steady_clock::time_point EndSteadyTimePoint() const = 0;


    ///Total execution time of this Testing Activity
    /** Monotonic (steady) clock is used for calculation */
    std::chrono::steady_clock::duration Duration() const{
        return EndSteadyTimePoint() - StartSteadyTimePoint();
    }

    ///Total execution time of this Testing Activity
    /** Monotonic (steady) clock is used for calculation */
    std::chrono::seconds::rep DurationSeconds() const {
        return  std::chrono::duration_cast<std::chrono::seconds>(
                    Duration()
                ).count();
    }
};


///Support information available before Test Session starts (before any testing)
/** All Tests in the process are executed in the context of the Test Session */
class TestSessionContextBefore: public TimeMeasuredTestingActivityContextBefore{
public:

    ///Number of all test suites found in this test session
    /** Available before actual execution*/
    virtual unsigned TestSuitesFound() const = 0;
};

///Support information available after Test Session completed (after all tests)
/** All Tests in the process are executed in the context of the Test Session */
class TestSessionContextAfter:
                               public TimeMeasuredTestingActivityContextAfter,
                               protected TestSessionContextBefore
{
    friend class details::FullContextForTestSuite;
public:
    //All stuff from TestSessionContextBefore also available
    using TestSessionContextBefore::TestSuitesFound;


    //TODO:
    ///Total number of all test suites executed
    //virtual unsigned TestCasesExecuted() const = 0;

    ///Total number of test suites passed (each nested test case passed)
    //virtual unsigned TestCasesPassed() const = 0;

    ///Total number of test suites failed (at least one nested test case failed)
    //virtual unsigned TestCasesFailed() const = 0;

    ///Total number of all test cases executed in all test suites
    virtual unsigned TestCasesExecuted() const = 0;

    ///Total number of test cases passed (though all test suites)
    virtual unsigned TestCasesPassed() const = 0;

    ///Total number of test cases failed (though all test suites)
    virtual unsigned TestCasesFailed() const = 0;
};


///Information available before Test Suite execution starts
/**Test Suite is a container for Test Cases with shared Setup/Teardown
   Setup is executed before each test case.
   Teardown is executed after each test case.
   Note: those tests created with TEST macro (that do not share Setup or
         Teardown) are part of the "DEFAULT" Test Suite*/
class TestSuiteContextBefore: public TimeMeasuredTestingActivityContextBefore{
public:
    ///Access to available containing TestSession information
    /** Note: only "before" part is available here */
    virtual const TestSessionContextBefore& ContainingTestSession() const = 0;

    ///File where corresponding Test Suite is placed
    virtual std::string File() const = 0;

    ///Line where corresponding Test Suite starts
    virtual unsigned Line() const = 0;
};

///Information available after Test Suite has been executed
/**Test Suite is a container for Test Cases with shared Setup/Teardown
   Setup is executed before each Test Case.
   Teardown is executed after each test case.
   Note: those tests created with TEST macro (that do not share Setup or
         Teardown) are part of the "DEFAULT" Test Session*/
class TestSuiteContextAfter:
                             public TimeMeasuredTestingActivityContextAfter,
                             protected TestSuiteContextBefore
{
    friend class details::FullContextForTestCase;
public:
    //All stuff from TestSuiteContextBefore also available
    using TestSuiteContextBefore::ContainingTestSession;
    using TestSuiteContextBefore::File;
    using TestSuiteContextBefore::Line;


    ///Number of all test cases in this test suite
    virtual unsigned TestCasesExecuted() const = 0;

    ///Number of all test cases passed in this test suite
    virtual unsigned TestCasesPassed() const = 0;

    ///Number of all test cases failed in this test suite
    virtual unsigned TestCasesFailed() const = 0;
};


///Information available before Test Case execution starts
/** Test Case is an item in the Test Suite with set of checks.
    Note: both TEST and TEST_CASE macro map here.*/
class TestCaseContextBefore: public TestingActivityContextBefore{
public:
    ///Access to containing Test Suite
    virtual const TestSuiteContextBefore& ContainingTestSuite() const = 0;

    ///File where corresponding Test Case is placed
    virtual std::string File() const = 0;

    ///Line where corresponding Test Case is placed
    virtual unsigned Line() const = 0;
};

///Information available after test case has been executed
/** Test case is an item in the Test Suite with set of checks.
    Note: both TEST and TEST_CASE macro map here. */
class TestCaseContextAfter: public TestingActivityContextAfter,
                            protected TestCaseContextBefore
{
public:
    //All stuff from TestCaseContextBefore also available
    using TestCaseContextBefore::ContainingTestSuite;
    using TestCaseContextBefore::File;
    using TestCaseContextBefore::Line;
};


///Information available before check statement is executed
/** Check statements are any of EXPECT or ASSERT in any form
    including comparisons and predicate/function calls.*/
class CheckerContextBefore: public TestingActivityContextBefore{
public:
    ///Access entire testing context (whole test or test case)
    virtual const TestCaseContextBefore& ContainingTestCase() const = 0;


    ///File where corresponding check (EXPECT or ASSERT) is placed
    virtual std::string File() const = 0;

    ///Line where corresponding check (EXPECT or ASSERT) is placed
    virtual unsigned Line() const = 0;


    ///Condition text as it is checked by InstantUnit
    /** Variables and arguments names are pasted
        as they are written in original condition,
        only ASSERT and EXPECT words are removed.

    Mapping from ASSERT statement to text
    @code
    | Statement to verify                   | Result of Text property       |
    | ASSERT( expr )                        | expr                          |
    | ASSERT( expr ) OP value               | expr OP value                 |
    | ASSERT(fcn)()                         | fcn()                         |
    | ASSERT(fcn)(arg)                      | fcn(arg)                      |
    | ASSERT(fcn)(arg1,... argN)            | fcn(arg1,... argN)            |
    | ASSERT(fcn)(arg1,... argN) OP value   | fcn(arg1,... argN) OP value   |
    @endcode
    for EXPECT mapping is similar*/
    virtual std::string Text() const = 0;
};


///
class CheckerContextAfter:
                            public CheckerContextBefore
{
public:
    ///Indicate "Passed" mark for check
    virtual bool IsPassed() const = 0;

    ///Indicate "Failed" mark for check
    bool IsFailed() const { return !IsPassed(); }

    ///Left hand side of the expression (as it is written)
    virtual std::string LHS() const = 0;

    ///Right hand side of the expression (as it is written)
    virtual std::string RHS() const = 0;

    ///Operation between LHS and RHS
    /**Could be any on ==, !=, <. <=, >, >=*/
    virtual std::string Operaton() const = 0;
};

/*
///
class ContextBeforeCheckCall{
public:
    ///Access entire testing context (whole test or test case)
    virtual const TestCaseContextBefore& TestCase() const = 0;

    ///
    virtual std::string StepText() const = 0;
};


///
class ContextAfterCheckCall: public ContextBeforeCheckCall{
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

*/

//class OnMessage
//class OnTrace


//Progress reporting ----------------------------------------------------------

///Report tests execution stages/progress
class Reporter{
public:
    virtual void OnFatalError(const char* what) = 0;


    ///Called before the Test Session execution
    virtual void OnItem(const TestSessionContextBefore& context) = 0;
    ///Called after the Test Session has been executed
    virtual void OnItem(const TestSessionContextAfter&  context) = 0;

    ///Called before each Test Suite execution
    virtual void OnItem(const TestSuiteContextBefore& context) = 0;
    ///Called after each Test Suite has been executed
    virtual void OnItem(const TestSuiteContextAfter&  context) = 0;

    ///Called before each Test Case execution
    /**Note: both TEST and TEST_CASE map here*/
    virtual void OnItem(const TestCaseContextBefore& context) = 0;
    ///Called after each Test Case has been executed
    /**Note: both TEST and TEST_CASE map here*/
    virtual void OnItem(const TestCaseContextAfter&  context) = 0;

    ///Called before each Check (ASSERT or EXPECT) execution
    virtual void OnItem(const CheckerContextBefore& context) = 0;
    ///Called after each Check (ASSERT or EXPECT) has been executed
    virtual void OnItem(const CheckerContextAfter&  context) = 0;

    /*
    TODO: use "Replacement only occurrs for a function-like macro if the macro
               name is followed by a left parenthesis"

    see http://stackoverflow.com/questions/4251005/what-is-the-difference-between-a-preprocessor-macro-with-no-arguments-and-one-w
    also https://gcc.gnu.org/onlinedocs/cpp/Function-like-Macros.html

    and

    #include <iostream>
    using namespace std;

    int M = 500;

    #define M(a) (100)

    int main() {
        // your code goes here
        std::cout<<M(1)<<std::endl;
        std::cout<<M()<<std::endl;
        std::cout<<M<<std::endl;
        return 0;
    }

    ///Called before each Check (ASSERT_CALL or EXPECT_CALL) execution
    virtual void OnItem(const ContextBeforeCheckCall& context) = 0;
    ///Called after each Check (ASSERT_CALL or EXPECT_CALL) has been executed
    virtual void OnItem(const ContextAfterCheckCall&  context) = 0;
    */

    /*
    ///Called before test step is being executed
    virtual void OnStep(const ContextForStep& contextForStep) = 0;

    ///Called to place message to output stream
    virtual void Message(const std::string& textMessage) = 0; //TBD

    ///Called when single test step failed
    virtual void OnVerify(const ContextForVerify& contextForVerify) = 0;


    ///Called when test case is passed
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
    */
};

} //namespace InstantUnit

//______________________________________________________________________________
//##############################################################################
/*==============================================================================
*  Implementation details follow                                               *
*=============================================================================*/
//##############################################################################

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

///Exception to be used to signal that Test Case failed
class AssertCheckFailed{};
///Exception to be used to signal that Sanity Check failed
class SanityCheckFailed{};

///Helper to unwind failed Test Case but continue Test Suite execution
class UnwindAndContinueTestSuite{};


///Implement collecting time tracking statistics for continuous activity
template<class Derived, class BaseToOverrideMethodsFrom>
class ContinuousActivity: public BaseToOverrideMethodsFrom{
public:
    //information available once activity started

    std::chrono::system_clock::time_point StartTimePoint() const override{
        checkIfStarted();
        return startTimePoint;
    }

    std::chrono::steady_clock::time_point StartSteadyTimePoint() const override{
        checkIfStarted();
        return startSteadyTimePoint;
    }

    //information available after activity completed

    std::chrono::system_clock::time_point EndTimePoint() const override{
        checkIfStarted();
        checkIfCompleted();
        static_cast<const Derived*>(this)->CheckIfReady("EndTimePoint");
        return endTimePoint;
    }

    std::chrono::steady_clock::time_point EndSteadyTimePoint() const override{
        checkIfStarted();
        checkIfCompleted();
        static_cast<const Derived*>(this)->CheckIfReady("EndSteadyTimePoint");
        return endSteadyTimePoint;
    }

protected:
    ///Called by derived class once activity starts
    void OnActivityStart(){
        startTimePoint = std::chrono::system_clock::now();
        startSteadyTimePoint = std::chrono::steady_clock::now();
        started = true;
    }

    ///Called by derived class once activity completed
    void OnActivityComplete(){
        endTimePoint = std::chrono::system_clock::now();
        endSteadyTimePoint = std::chrono::steady_clock::now();
    }

private:
    bool started = false;
    bool completed = false;

    void checkIfStarted() const {
        if( !started ){
            //TODO: report error
        }
    }
    void checkIfCompleted() const {
        if( !completed ){
            //TODO: report error
        }
    }

    std::chrono::system_clock::time_point startTimePoint;
    std::chrono::steady_clock::time_point startSteadyTimePoint;

    std::chrono::system_clock::time_point endTimePoint;
    std::chrono::steady_clock::time_point endSteadyTimePoint;
};


//Contexts used for testing

///Collect information available before/after Test Session
class FullContextForTestSession:
    public ContinuousActivity<FullContextForTestSession, TestSessionContextAfter>{
public:
    ///Initialize start date/time by current moment and autogenerate name
    FullContextForTestSession()
      : testSessionName("Test session ")
    {
        OnActivityStart();
        std::time_t epoch_time = std::chrono::system_clock::to_time_t(StartTimePoint());
        testSessionName += std::ctime(&epoch_time);
    }

    ///Initialize start date/time by current moment and use name from argument
    /**TODO: command line option for TS name*/
    FullContextForTestSession(
        const std::string& testSessionNameToUse
    ) : testSessionName(testSessionNameToUse) {}


    //override from TestingActivityContextBefore

    bool IsErrorOnStart() const override{
        return true; //TODO:
    }


    //override from TestingActivityContextAfter

    bool IsPassed() const override{
        return false; //TODO:
    }


    //override from TestSessionContextBefore

    std::string Name() const override{
        return testSessionName;
    }

    virtual unsigned TestSuitesFound() const override{
        //TODO:
        return 0;
    }


    //override from TestSessionContextAfter


    unsigned TestCasesExecuted() const override{
        CheckIfReady("TestCasesExecuted");
        return testCasesTotalExecuted;
    }

    unsigned TestCasesPassed() const override{
        CheckIfReady("TestCasesPassed");
        return testCasesTotalPassed;
    }

    unsigned TestCasesFailed() const override{
        CheckIfReady("TestCasesFailed");
        return testCasesTotalFailed;
    }


    //TestSession level events processing


    void OnTestCaseStart(){
        ++testCasesTotalExecuted;
    }

    void OnTestCasePassed(){
        ++testCasesTotalPassed;
    }

    void OnTestCaseFailed(){
        ++testCasesTotalFailed;
    }


    void OnTestSessionComplete(){
        OnActivityComplete();
        //make "after" part available
        ready = true;
    }


    //TODO: collection of test suites ?
private:
    friend class ContinuousActivity<FullContextForTestSession, TestSessionContextAfter>;

    ///Ensure "after" part is available
    void CheckIfReady(const char* calledFrom) const {
        if( !ready ){
            //TODO: one should call this only after Test Session completed
        }
    }
    ///Flag is set to true after test session is executed
    bool ready = false;

    std::string testSessionName; ///<by default is derived from start time

    unsigned testCasesTotalExecuted = 0;
    unsigned testCasesTotalPassed = 0;
    unsigned testCasesTotalFailed = 0;
};



///Collect information available before/after Test Suite
class FullContextForTestSuite:
    public ContinuousActivity<FullContextForTestSuite, TestSuiteContextAfter>{
public:
    ///Create named TestSuite (package of test cases)
    FullContextForTestSuite(
        const std::string& testSuiteNameToUse,
        FullContextForTestSession& parentTestSessionUsed,
        const std::string& fileWherePlaced,
        unsigned lineWhereStarts
    )
        :   testSuiteName(testSuiteNameToUse),
            parentTestSession(parentTestSessionUsed),
            file(fileWherePlaced),
            line(lineWhereStarts)  {}


    //override from TestingActivityContextBefore

    bool IsErrorOnStart() const override{
        return true; //TODO:
    }


    //override from TestingActivityContextAfter

    bool IsPassed() const override{
        return false; //TODO:
    }


    //override from TestSuiteContextBefore

    std::string Name() const override{
        return testSuiteName;
    }

    const TestSessionContextBefore& ContainingTestSession() const override{
        return parentTestSession;
    }


    std::string File() const override { return file; }

    unsigned Line() const override { return line; }


    //override from TestSuiteContextAfter

    unsigned TestCasesExecuted() const override{
        CheckIfReady("TestCasesExecuted");
        return testCasesExecuted;
    }

    unsigned TestCasesPassed() const override{
        CheckIfReady("TestCasesPassed");
        return testCasesPassed;
    }

    unsigned TestCasesFailed() const override{
        CheckIfReady("TestCasesFailed");
        return testCasesFailed;
    }


    //TestSuite level events processing

    //Actual Test Suite execution

    ///Function to executed single test case
    /**TBD*/
    typedef std::function<
                bool()
            > TestCaseExecutorFunction;

    ///Iterate over available test cases in test suite
    /**\returns empty function (nullptr) when iteration completes*/
    typedef std::function<
                TestCaseExecutorFunction()
            > GetNextTestCaseExecutorFunction;


    bool ProcessTestCases(
        const GetNextTestCaseExecutorFunction& getNextTestCaseExecutorFunction
    ){
        bool allNestedTCSucceeded = true;

        for(;;){
            TestCaseExecutorFunction
                executeNextTestCase = getNextTestCaseExecutorFunction();
            if( !executeNextTestCase ){
                break;
            }


            onTestCaseStart();

            bool testCaseExecutionResult = false;

            try{
                testCaseExecutionResult = executeNextTestCase();
            }
            catch(const AssertCheckFailed&){
                //That was ASSERT not included in any Test Case body (report wrong usage)

            }
            catch(const SanityCheckFailed&){
                //That was SANITY at Test Suite level (in setup or teardown sections of TC)

                //TODO: report no way to continue such Test Suite
            }
            catch(const std::exception& e){
                //Report exception is detected at Test Suite level

                //TODO: report no way to continue such Test Suite
            }
            catch(...){
                //Report unknown exception detected at Test Suite level
            }


            if( testCaseExecutionResult  ){
                onTestCasePassed();
            }
            else{
                onTestCaseFailed();
            }

            allNestedTCSucceeded = allNestedTCSucceeded && testCaseExecutionResult;
        }
        onTestSuiteComplete();

        return allNestedTCSucceeded;
    }



private:
    friend ContinuousActivity<FullContextForTestSuite, TestSuiteContextAfter>;

    ///Ensure "after" part is available
    void CheckIfReady(const char* calledFrom) const {
        if( !ready ){
            //TODO: one should call this only after Test Suite completed
        }
    }
    ///Flag is set to true after TestSuite is executed
    bool ready = false;

    std::string testSuiteName;
    FullContextForTestSession& parentTestSession;

    std::string file;
    unsigned line;


    unsigned testCasesExecuted = 0;
    unsigned testCasesPassed = 0;
    unsigned testCasesFailed = 0;

    void onTestCaseStart(){
        parentTestSession.OnTestCaseStart();
        ++testCasesExecuted;
    }

    void onTestCasePassed(){
        parentTestSession.OnTestCasePassed();
        ++testCasesPassed;
    }

    void onTestCaseFailed(){
        parentTestSession.OnTestCaseFailed();
        ++testCasesFailed;
    }

    void onTestSuiteComplete(){
        //make "after" part available
        ready = true;
    }
};


///Collect information available before/after TestCase
class FullContextForTestCase: public TestCaseContextAfter{
public:
    ///Create named TestCase
    FullContextForTestCase(
        const std::string& testCaseNameToUse,
        FullContextForTestSuite& parentTestSuiteUsed,
        const std::string& fileWherePlaced,
        unsigned lineWhereStarts
    )
        :   testCaseName(testCaseNameToUse),
            parentTestSuite(parentTestSuiteUsed),
            file(fileWherePlaced),
            line(lineWhereStarts) {}


    //override from TestingActivityContextBefore

    bool IsErrorOnStart() const override{
        return true; //TODO:
    }


    //override from TestingActivityContextAfter

    bool IsPassed() const override{
        CheckIfReady("IsPassed");
        return isPassed;
    }


    //override from TestCaseContextBefore

    std::string Name() const override{
        return testCaseName;
    }

    const TestSuiteContextBefore& ContainingTestSuite() const override{
        return parentTestSuite;
    }

    std::string File() const override { return file; }

    unsigned Line() const override { return line; }

    //override from TestCaseContextAfter


    //Actual Test Case execution

    ///Function to be executed as test case body
    /**That function contains all those ASSERT, EXPECT etc stuff*/
    typedef std::function<
            void(bool& allAssertsAndExpectsPassedFlag)
        > TestCaseBodyFunction;

    ///Execute single Test Case, return true on TC success, false on TC failure
    bool ExecuteTestCase(
        const TestCaseBodyFunction& tcBody
    ){
        try{
            bool allAssertsAndExpectsPassedFlag = true;

            tcBody(allAssertsAndExpectsPassedFlag);

            return allAssertsAndExpectsPassedFlag;
        }
        catch(const AssertCheckFailed&){
            //Assert failed inside Test Case
            //TODO: report failure
            //(and continue execution of other TCs!)
        }
        catch(const SanityCheckFailed&){
            //Sanity failed inside Test Case
            //TODO: report failure
            //(and continue execution of other TCs!)
        }
        catch(const std::exception& e){
            //TODO: Report exception detected
            //(and continue execution of other TCs!)
        }
        catch(...){
            //TODO: Report unknown exception detected at Test Case level
            //(and continue execution of other TCs!)
        }
        return false;
    }

private:
    ///Ensure "after" part is available
    void CheckIfReady(const char* calledFrom) const {
        if( !ready ){
            //TODO: one should call this only after Test Case completed
        }
    }
    ///Flag is set to true after TestCase is executed
    bool ready = false;

    std::string testCaseName;
    FullContextForTestSuite& parentTestSuite;

    std::string file;
    unsigned line;

    bool isPassed = false;
};





class Context{};

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

/*
struct ValueWrapMaker{
    ValueWrapMaker(Reporter& reporterToUse, const char* stepText)
        : reporter(reporterToUse)
        //, contextBeforeCheck()
    {

    }
private:
    Reporter& reporter;
};*/

///Helper base to collect list of static/global class instances
/**Every derived instance of type DerivedT will be a part of the global list.
 For each DerivedT we have own corresponding list of instances.
 (Note: DerivedT also can be a base for other classes).
 Access to all DerivedT instances (and all instances derived from DerivedT)
 via CollectInstancesOf<DerivedT>::ForEachInstance method.
 Allocate any derived instances as global/static variables only.*/
template<class DerivedT>
class CollectInstancesOf{
public:
    ///Invoke callback for each known instance (in the order as they were added)
    /**Global list of all existing instances is readly
       only after corresponding instances were initialized.
       You will not see here those that are placed in functions,
       so do not use "Mayers singleton" trick with derived objects.*/
    template<class FcnToCallOnEachPtr>
    static void ForEachInstance(FcnToCallOnEachPtr fcn){
        //Ensure one uses CollectInstancesOf in right way
        static_assert(
            std::is_base_of< CollectInstancesOf, DerivedT >::value,
            "DerivedT is listed in CollectInstancesOf"
            " only when CollectInstancesOf<DerivedT> is base class for DerivedT"
        );

        for(CollectInstancesOf* ptr = head; ptr != nullptr; ptr = ptr->next){
            fcn(*static_cast<DerivedT*>(ptr));
        }
    }


    //Ban copy
    CollectInstancesOf(const CollectInstancesOf&) = delete;
    CollectInstancesOf& operator=(CollectInstancesOf&) = delete;

    //ban all dynamic allocation stuff
    void* operator new(std::size_t) = delete;          // standard new
    void* operator new(std::size_t, void*) = delete;   // placement new
    void* operator new[](std::size_t) = delete;        // array new
    void* operator new[](std::size_t, void*) = delete; // placement array new
    //also ban corresponding deletes
    void operator delete(void*) = delete;          // standard delete
    void operator delete(void*, void*) = delete;   // placement delete
    void operator delete[](void*) = delete;        // array delete
    void operator delete[](void*, void*) = delete; // placement array delete

protected:
    ///Add this item to the end of the list (preserve order)
    CollectInstancesOf() : next(nullptr) {
        *pointerToTailPtr = this; //this will be new end of list
        pointerToTailPtr = &this->next; //set up new tail to update next time
    }

private:
    ///All known instances list head
    /**use simple pointer chain to prevent problems with initialization order*/
    static CollectInstancesOf* head;
    ///Remember current list tail position here
    static CollectInstancesOf* *pointerToTailPtr;

    ///Pointer to the next known instance
    CollectInstancesOf* next;
};

template<class DerivedT>
CollectInstancesOf<DerivedT>* CollectInstancesOf<DerivedT>::head = nullptr;
template<class DerivedT>
CollectInstancesOf<DerivedT>* *CollectInstancesOf<DerivedT>::pointerToTailPtr = &CollectInstancesOf<DerivedT>::head;


///Base class for object to be executed while running tests
/*class Runnable: public CollectInstancesOf<Runnable>{
protected:
    ///Method to be runned for all found Runnable instances
    virtual bool Run() = 0;

private:
    friend bool InstantUnit::RunTests();
};*/


/*

typedef std::function<
            void(const Runner_TestCaseBodyFunction& testCaseBody)
        > TestCaseExecutorFunction;

///Called by TEST_CASE macro inside Runner_DoNextTest
typedef std::function<
            TestCaseExecutorFunction(
                const char* testCaseFileName, unsigned testCaseLineNumber,
                const char* testCaseName
            )
        > MakeTestCaseExecutorFunction;

*/

///Base class to be executed while running tests
/** Collect those tests, that are part of the "DEFAULT" test suite*/
class SimpleStandaloneTestRunner:
    public CollectInstancesOf<SimpleStandaloneTestRunner>
{
protected:
    SimpleStandaloneTestRunner(
        const std::string& fileWherePlaced,
        unsigned lineWhereStarts
    )
        :   file(fileWherePlaced),
            line(lineWhereStarts) {}


    ///Method called to do actual testing
    virtual void Runner_DoTest(bool& allAssertsAndExpectsPassedFlag) = 0;

    ///Name for test being runned
    virtual const char* Runner_CurrentTestName() const = 0;


/*
    ///
    void SetTestFaulureFlag(){
        //TODO:
    }

    bool IsTestFailed(){
        //TODO
        return false;
    }*/

private:
    //see also http://stackoverflow.com/questions/23834845/c-lambda-friendship
    friend bool InstantUnit::RunTests();

    ///Method to be run for all found Runnable instances
    /**\returns true on TC success, false on TC failure*/
    bool RunTest(FullContextForTestSuite& fullContextForTestSuite){
        FullContextForTestCase fullContextForTestCase(
            Runner_CurrentTestName(), fullContextForTestSuite,
            file, line
        );

        /* TC Body is executed inside FullContextForTestCase
          to collect execution results/stats in that context */
        return fullContextForTestCase.ExecuteTestCase(
            [&](bool& allAssertsAndExpectsPassedFlag)
                { Runner_DoTest(allAssertsAndExpectsPassedFlag); }
        );
    }

    std::string file;
    unsigned line;
};


///Base class to be executed while running tests
/** Collect those tests, that are part of the "DEFAULT" test suite*/
class TestSuiteRunner:
    public CollectInstancesOf<TestSuiteRunner>
{
protected:
    TestSuiteRunner(
        const std::string& fileWherePlaced,
        unsigned lineWhereStarts
    )
        :   file(fileWherePlaced),
            line(lineWhereStarts) {}

    ///Called by TEST_CASE macro inside Runner_DoNextTest
    typedef std::function<
                void(
                    const char* fileName, unsigned lineNumber,
                    const char* testCaseName,
                    const std::function<
                        void(bool& allAssertsAndExpectsPassedFlag)
                    >& testCaseBody
                )
            > Runner_OnTestCase;

    ///Method called to do actual testing (invoke test cases)
    virtual void Runner_DoNextTest(
        const Runner_OnTestCase& runner_OnTestCase ///<
    ) = 0;

    ///Name for Test Suite being runned
    virtual const char* Runner_CurrentTestSiuteName() const = 0;


private:
    //see also http://stackoverflow.com/questions/23834845/c-lambda-friendship
    friend bool InstantUnit::RunTests();


    ///Method to be run for all found Runnable instances
    /**\returns true when all nested TC succeeded, false otherwise*/
    bool RunTestSuite(FullContextForTestSession& fullContextForTestSession){

        bool allNestedTCSucceeded = true;

        //current Test Suite execution state
        enum{
            StateJustStartedRunFirstTC,
            StateCollectOtherTCInformation,
            StateRunRestOfTC,
            StateCompleted
        } state;

        bool totalTestCasesFound = 0;

        //all test cases in the test suite shall be in the same file
        std::string sameFileNameForTCSanityCheck;
        //track line numbers of test cases to prevent same TC from being executed several times
        std::set<unsigned> executedLineNumbersForSanityCheck;

        //Current Test Suite context instance
        details::FullContextForTestSuite fullContextForCurrentTestSuite(
            Runner_CurrentTestSiuteName(), fullContextForTestSession,
            file, line
        );

        fullContextForCurrentTestSuite.ProcessTestCases(
            [&]()-> details::FullContextForTestSuite::TestCaseExecutorFunction
            {
                return [&]()->bool{
                    return false;
                };
            }
        );

        /*Call Runner_DoNextTest number of times to execute each nested TC
          (Note: each time different TC is executed) */
#if 0
        while( StateCompleted != state ){
            try{
                bool testCaseResult = false;

                Runner_DoNextTest(
                    [&] (
                        const char* fileName, unsigned lineNumber,
                        const char* testCaseName,
                        const std::function<
                            void(bool& allAssertsAndExpectsPassedFlag)
                        >& testCaseBody
                    ){
                        //This code is executed after setup and before teardown

                        testCaseResult = RunTestCase(testCaseBody);

                        state = StateCompleted;

                    }
                );

                allNestedTCSucceeded = testCaseResult && allNestedTCSucceeded;


                continue;
            }
            catch(const AssertCheckFailed&){
                //That was ASSERT not included in any Test Case (report wrong usage)
            }
            catch(const SanityCheckFailed&){
                //That was SANITY at Test Suite level (in setup or teardown)

                //TODO: report no way to continue such Test Suite
            }
            catch(const std::exception& e){
                //Report exception is detected at Test Suite level

                //TODO: report no way to continue such Test Suite
            }
            catch(...){
                //Report unknown exception detected at Test Suite level
            }
            return false;
        }
#endif
        return true;
    }

    std::string file;
    unsigned line;
};

} //namespace InstantUnit::details

inline bool RunTests()
{
    details::FullContextForTestSession fullContextForTestSession;

    bool allPassed = true;

    //Run "DEFAULT" Test Suite first
    {
        //"DEFAULT" Test Suite context instance
        details::FullContextForTestSuite fullContextForDefaultTestSuite(
            "DEFAULT", fullContextForTestSession,
            "DEFAULT test suite for simple TEST macro", 0
        );

        //actual iteration for all elements in "DEFAULT" Test Suite
        details::CollectInstancesOf<
            details::SimpleStandaloneTestRunner
        >::ForEachInstance(
            [&](details::SimpleStandaloneTestRunner& test){
                allPassed = test.RunTest(fullContextForDefaultTestSuite) && allPassed;
            }
        );
    }
/*

    //Run other Test Suites available
    details::CollectInstancesOf<
        details::TestSuiteRunner
    >::ForEachInstance(
        [&](details::TestSuiteRunner& test){
            allPassed = test.RunTestSuite(fullContextForTestSession) && allPassed;
        }
    );
*/
    return allPassed;
}

int RunTets(int argc, char *argv[]){
    //TODO invent and process command line options (like filtering, etc)
    return 0;
}

} //namespace InstantUnit

/*22469275616224 /22469275616224*/

#endif /*INSTANTUNIT_HDR_*/
