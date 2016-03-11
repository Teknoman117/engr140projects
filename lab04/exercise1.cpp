#include <iostream>
#include "Stash.h"

using namespace std;

int main (int argc, char **argv)
{
	int N;
	Stash stash;

	cin >> N;
	stash.initialize((int) sizeof(double));
	
	for(int i = 0; i < N; i++)
	{
		double val;
		cin >> val;
		stash.add(static_cast<void *>(&val));
	}

	for(int i = 0; i < N; i++)
	{
		double val = *static_cast<double *>(stash.fetch(i));
		cout << val << endl;
	}

	stash.cleanup();

	return 0;
}
