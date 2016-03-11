#include <iostream>

using namespace std;

struct A
{
	char a;
	double b;
};

struct B
{
	char a;
	char b;
	double c;
};

struct C
{
	char a;
	char b;
	char c;
	double d;
};

struct D
{
	char a;
	char b;
	char c;
	char d;
	double e;
};

struct E
{

};

struct F
{
	char a;
	int b;
	char c;
};

struct G
{
	char a;
	char b;
	int c;
};

int main (int argc, char **argv)
{
	cout << sizeof(A) << endl;
	cout << sizeof(B) << endl;
	cout << sizeof(C) << endl;
	cout << sizeof(D) << endl;
	cout << sizeof(E) << endl;
	cout << sizeof(F) << endl;
	cout << sizeof(G) << endl;
	return 0;
}
