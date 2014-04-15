#include "BigInt.h"

#include <iostream>

#include <fstream>
#include <stdlib.h>
#include <time.h>

using namespace std;

void genHex(int num)
{
	srand((unsigned int)time(NULL));
	ofstream out("output.txt");

	for (int j = 0; j < num; ++j)
	{
		//out << "0x";
		for (int i = 0; i < 503; ++i)
		{
			out << hex << rand() % 16;
		}
		out << endl << endl << endl;
	}

	out.close();
}



int main(int argc, char* argv[])
{
	BigInt zero(0);
	BigInt one(1);
	BigInt one2(1);

	one += 1;
	one2 += 1;

	bool a = one == zero;
	bool t = one2 == 2;

	cout << boolalpha << a << endl;



	cout << endl << "Paused, press ENTER to continue." << endl;
	cin.ignore(1000000, '\n');
}