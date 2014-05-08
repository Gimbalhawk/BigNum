#include <cxxtest/TestSuite.h>
#include <limits.h>

#include "BigInt.h"

class Tests : public CxxTest::TestSuite
{
public:
	void testEquality() 
	{ 
		BigInt zero(0);
		BigInt one(1);
		BigInt two(2);
		BigInt negone(-1);
		BigInt maxlong(ULLONG_MAX);

		// Basic integer tests
		TS_ASSERT_EQUALS(zero, 0);
		TS_ASSERT_EQUALS(one, 1);
		TS_ASSERT_EQUALS(two, 2);
		TS_ASSERT_EQUALS(negone, -1);
		TS_ASSERT_EQUALS(maxlong, ULLONG_MAX);

		TS_ASSERT_DIFFERS(zero, 1);
		TS_ASSERT_DIFFERS(one, 0);
		TS_ASSERT_DIFFERS(maxlong, UINT_MAX);

		// Now, test BigInt/BigInt equality
		TS_ASSERT_EQUALS(zero, zero);
		TS_ASSERT_EQUALS(one, one);
		TS_ASSERT_DIFFERS(zero, one);
		TS_ASSERT_DIFFERS(one, zero);
		TS_ASSERT_DIFFERS(one, negone);
	}

	void testAddition() 
	{
		
	}

	void testSubtraction() 
	{

	}

};
