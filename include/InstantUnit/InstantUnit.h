/** @file InstantUnit.h
    @brief Mimimalistic (header only) Unit Test framework for C++11 and above.

Simple usage sample (single Test Case, without shared Setup/Teardown, all in one):
@code

    TEST("My test name"){
        //Setup statements (local for this test, not shared with others)
        std::vector<int> v;
        v.push_back(10);
        v.push_back(20);
        v.push_back(31);

        //Asserts will end current Test Case when failed
        ASSERT(v.empty()) == false;
        ASSERT(v.size()) == 3;

        //Expects will still continue Test after failed
        EXPECT(v.front()) == 10;
        EXPECT(v[1]) == 20;
        EXPECT(v.back()) == 31
    }

@endcode
Note: ASSERT causes Test to complete immediately when corresponding
             condition fails, the rest of the failed Test is skipped.
      EXPECT (in contrast) just mark surrounding Test Case as failed,
             but Test Case execution continues.

You can write a condition to be checked directly inside of the ASSERT
or EXPECT macro:
@code
    EXPECT(x > 3);
    EXPECT(y >= 2.9 && y <= 3.1);
@endcode
but when condition fails there will be no additional information "why failed".
Use following syntax:
@code
    EXPECT(x) > 3;
    EXPECT(InstantUnit::IsClose, y, 3, 0.1);
@endcode
Now value of x will go to the output, and arguments passed to predicate
are printed. InstantUnit::IsClose, from sample above, is a predicate built into the framework,
but you can write your own:

@code
    bool IsOdd(int v){
        return v & 0x1
    }
    TEST("Test OddGenerator"){
        OddGenerator g;

        ASSERT(IsOdd, g.GetNext());
        ASSERT(IsOdd, g.GetNext());
        ASSERT(IsOdd, g.GetNext());
    }
@endcode

More complex sample (Add shared common Setup and Teardown, notice how those
Setup and Teardown are located - there is no need to inherit from any
classes, override any functions, etc):
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

Setup code from the Test Suite is executed before each Test Case like it was
pasted directly before the Test Case body. You can declare variables in Setup
and they will be visible from the Test Case body.

Teardown code from the Test Suite is executed after each Test Case like it is
pasted directly after the test case body. Teardown code is still automatically
executed even in case of exception in a Test Case body.
Every variable declared in the Setup code is visible from the Teardown code.

*/

#ifndef INSTANTUNIT_HDR_
#define INSTANTUNIT_HDR_

#include <cstddef>
#include <cmath>
#include <stdexcept>
#include <string>
#include <functional>


///Simple Test (standalone Test Case without shared Setup/Teardown stuff)
/** Place Test code in braces after TEST macro*/
#define TEST(testNameString) \
    /*Class to run test code in*/ \
    class IU_CAT_ID(Test_,__LINE__): private InstantUnit::details::SimpleRunner{ \
        /*Test code will go here, called automatically from SimpleRunner*/ \
        virtual void DoTest(); \
    }; \
    /*Corresponding static instance to be part of run*/ \
    static IU_CAT_ID(Test_,__LINE__) IU_CAT_ID(Instance_,__LINE__); \
    /*Test body will follow below*/ \
    void IU_CAT_ID(Test_,__LINE__)::DoTest()

///Group Test Cases together to support common Setup/Teardown
/**Place Test Setup, then Test Cases and then Teardown.
   See example above on how to use this stuff.*/
#define TEST_SUITE(testSuiteNameString) \
    class testSuiteNameString##_TestSuite:

///Single Test Case item in the Test Suite (share Setup/Teardown with others)
#define TEST_CASE(testCaseNameString)


///Mark an expression or call as being subject to "assert test"
/**Causes Test Case to complete immediately on "verify fail"
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

   */
#define ASSERT()
//

///Mark an expression or call as being subject to "expect test"
/**Just mark surrounding Test Case as failed on "verify fail",
   but Test Case execution continues.
   Usage is similar to ASSERT from above */
#define EXPECT()
//


namespace InstantUnit{

///Execute all known Test Suites as part of default Test Session
void RunTests();

//Predefined verifiers --------------------------------------------------------

///Test double values are equal with precission
inline bool IsClose(double val1, double val2, double precission){
    return std::fabs(val1 - val2) <= precission;
}

///Test value is within limits (fromInclusive <= val <= toInclusive)
template<class T>
inline bool IsBetween(T val, T fromInclusive, T toInclusive){
    return false;
}

//Contexts for reporting ------------------------------------------------------

///All Tests in the process are executed in the context of Test Session
class ContextBeforeTestSession{
public:
    ///
    virtual std::string TestSessionName() const = 0;

    //test session start date

};

///All Tests in the process are executed in the context of Test Session
class ContextAfterTestSession: public ContextBeforeTestSession{
public:
    //test session end date

    //number of all test suites found
    //total number of all test cases in all test suites

    //total number or test cases passed
    //total number or test cases failed

    //Total execution time of all suites

    //collection of test suites?
};

///Test Suite is a container for Test Cases with shared Setup/Teardown
class ContextBeforeTestSuite{
public:
    ///
    virtual std::string TestSuiteName() const = 0;

};

///Test Suite is a container for Test Cases with shared Setup/Teardown
class ContextAfterTestSuite: public ContextBeforeTestSuite{
public:

    //number of all test cases in this test suite
    //number of all test cases passed in this test suite
    //number of all test cases failed in this test suite
};

///
class ContextForTestCase{
public:
    ///Access to containing Test Suite
    virtual const ContextBeforeTestSuite& TestSuite() const = 0;

    ///Name used in corresponding TEST or TEST_CASE macro
    virtual std::string TestCaseName() const = 0;

};

///
class ContextForStatement{
public:
    ///Access entire testing context (whole test or test case)
    virtual const ContextForTestCase& TestCase() const = 0;
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

}

/*=============================================================================
*  Implementation details
*=============================================================================*/

//Expand and concatenate macro arguments into combined identifier
#define IU_CAT_ID(a,b) IU_CAT_ID_EXPANDED_HELPER(a,b)
//helper macro to concatenate expanded macro arguments
#define IU_CAT_ID_EXPANDED_HELPER(a,b) a##b

//helper macro to determine if we assert value or function call
#define IU_GET_SUFFIX_FROM_NUM_ARG(...) \
    IU_GET_SUFFIX_FROM_NUM_ARG_HELPER(__VA_ARGS__, CALL, CALL, CALL, CALL, CALL, CALL, CALL, CALL, CALL, CALL, CALL, CALL, CALL, CALL, CALL, VAL)
//helper macro (used by IU_HAS_MANY_ARGUMENTS to obtain shifted argument)
#define IU_GET_SUFFIX_FROM_NUM_ARG_HELPER(     _1,   _2,   _3,   _4,   _5,   _6,   _7,   _8,   _9,  _10,  _11,  _12,  _13,  _14,  _15,  _16, res) res

namespace InstantUnit{

namespace details{
    ///Exception to be used to signal that test case failed
    class TestCaseFailed{};

    ///
    class Context{
    public:
        //void Step
    };

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
        virtual void Run() = 0;

    private:
        friend void InstantUnit::RunTests();
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
} //namespace InstantUnit::details

inline void RunTests(){
    details::InstanceListBase<details::Runnable>::ForEachInstance(
        [](details::Runnable* ptr){
            ptr->Run();
        }
    );
}

} //namespace InstantUnit

#endif /*INSTANTUNIT_HDR_*/
