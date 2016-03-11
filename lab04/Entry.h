#ifndef __ENTRY_H__
#define __ENTRY_H__

#include <iostream>
#include <string>

using namespace std;

struct Entry
{
	std::string name;
	std::string lastName;
	std::string email;

	void setName(const std::string& name)
	{
		this->name = name;
	}
	void setLastname(const std::string& lastName)
	{
		this->lastName = lastName;
	}
	void setEmail(const std::string& email)
	{
		this->email = email;
	}
	const std::string& getName() const
	{
		return name;
	}
	const std::string& getLastname() const
	{
		return lastName;
	}
	const std::string& getEmail() const
	{
		return email;
	}

	void print() const
	{
		std::cout << "First Name: " << name << std::endl;
		std::cout << "Last Name: " << lastName << std::endl;
		std::cout << "Email: " << email << std::endl;
	}
};

#endif
