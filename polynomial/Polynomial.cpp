#include "Polynomial.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	Polynomial A, B, C, D;
	cin >> A >> B;
	cout << A << B;
	C = A + B;
	D = A * B;
	cout << C << D;
	system("PAUSE");
	return 0;
}