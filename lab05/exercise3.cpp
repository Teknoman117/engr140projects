#include <iostream>
#include <string>
#include <vector>

#include "Animal.h"
#include "Dog.h"
#include "display.h"

using namespace std;

int main (int argc, char** argv)
{
	vector<Animal *> animals;
	int N;

	cin >> N;

	for(int i = 0; i < N; i++)
	{
		Animal *animal;
		char    type;
		int     age;
		string  name;

		cin >> type >> name >> age;

		if(type == 'A')
		{
			animal = new Animal();
			animal->setAge(age);
			animal->setName(name);
		}
		else if(type == 'D')
		{
			animal = new Dog(name, age);
		}

		animals.push_back(animal);
	}

	display(animals);

	return 0;
}
