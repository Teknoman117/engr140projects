#include <iostream>
#include "Stack.h"

using namespace std;

int main (int argc, char **argv)
{
	int N;
	Stack stack;

	stack.initialize();
	cin >> N;
	
	for(int i = 0; i < N; i++)
	{
		double val;
		cin >> val;
		stack.push(new double(val));
	}

	// Print out everything
	Stack::Link *current = stack.head;
	while(current != NULL)
	{
		double *val = static_cast<double *>(current->data);
		cout << *val << endl;
		current = current->next;
	}

	/*void *datum = NULL;
	while((datum = stack.pop()) != NULL)
	{
		delete static_cast<double *>(datum);
	}*/
	for(int i = 0; i < N; i++)
	{
		stack.pop();
	}

	stack.cleanup();

	return 0;
}

