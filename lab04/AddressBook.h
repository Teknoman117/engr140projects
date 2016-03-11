#ifndef __ADDRESSBOOK_H__
#define __ADDRESSBOOK_H__

#include "Entry.h"
#include <vector>

struct AddressBook
{
	std::vector<Entry> entries;

	AddressBook(const std::string& name, const std::string& lastName, const std::string& email)
	{
		entries.resize(1);

		entries[0].setName(name);
		entries[0].setLastname(lastName);
		entries[0].setEmail(email);
	}

	void add(AddressBook *book)
	{
		entries.insert(entries.end(), book->entries.begin(), book->entries.end());
	}

	void print()
	{
		for(std::vector<Entry>::iterator it = entries.begin(); it != entries.end(); it++)
		{
			it->print();
			std::cout << std::endl;
		}
	}
};

#endif
