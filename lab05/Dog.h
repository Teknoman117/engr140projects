#ifndef __DOG_H__
#define __DOG_H__

#include "Animal.h"

#include <iostream>
#include <string>

class Dog : public Animal
{
public:
	Dog() : Animal()
	{
		std::cout << "Creating Dog" << std::endl;
	}
	
	Dog(std::string name, int age)
	{
		std::cout << "Creating Dog" << std::endl;

		setName(name);
		setAge(age);
	}

	~Dog()
	{
		std::cout << "Deleting Dog" << std::endl;
	}
	
	void feed()
	{
		std::cout << "A bone, please!" << std::endl;
	}
};

#endif
