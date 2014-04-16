#include "BigInt.h"

#include <iostream>

#include <fstream>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main(int argc, char* argv[])
{
//	BigInt zero(0);
//	BigInt one(1);
//	BigInt one2(1);

	//int i = 1;

	BigInt zero(0);
	zero = zero + 1;
	//zero = zero + (long)i;

	bool b = 5 == 5.0f;

	cout << b << endl;

//	one += 1;
//	one2 += 1;

//	bool a = one == zero;
//	bool t = one2 == 2;

//	cout << boolalpha << a << endl;

//	cout << endl << "Paused, press ENTER to continue." << endl;
//	cin.ignore(1000000, '\n');
}