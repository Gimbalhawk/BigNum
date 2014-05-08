/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ParenPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ParenPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ParenPrinter >( tmp, argc, argv );
    return status;
}
bool suite_Tests_init = false;
#include "C:\Users\Tim\Documents\Workspace\C++\BigNum\Code\Tests.h"

static Tests suite_Tests;

static CxxTest::List Tests_Tests = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_Tests( "../../Code/Tests.h", 6, "Tests", suite_Tests, Tests_Tests );

static class TestDescription_suite_Tests_testEquality : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_Tests_testEquality() : CxxTest::RealTestDescription( Tests_Tests, suiteDescription_Tests, 9, "testEquality" ) {}
 void runTest() { suite_Tests.testEquality(); }
} testDescription_suite_Tests_testEquality;

static class TestDescription_suite_Tests_testAddition : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_Tests_testAddition() : CxxTest::RealTestDescription( Tests_Tests, suiteDescription_Tests, 36, "testAddition" ) {}
 void runTest() { suite_Tests.testAddition(); }
} testDescription_suite_Tests_testAddition;

static class TestDescription_suite_Tests_testSubtraction : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_Tests_testSubtraction() : CxxTest::RealTestDescription( Tests_Tests, suiteDescription_Tests, 41, "testSubtraction" ) {}
 void runTest() { suite_Tests.testSubtraction(); }
} testDescription_suite_Tests_testSubtraction;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
