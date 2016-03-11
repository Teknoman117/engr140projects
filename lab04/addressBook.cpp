#include <iostream>

#include "AddressBook.h"

int main(int argc, const char * argv[])
{
    int n;
    
    cin >> n;
    
    string name, lastname, email;
    
    cin >> name;
    cin >> lastname;
    cin >> email;
    
    AddressBook * myAddressBook = new AddressBook(name, lastname, email);
    
    AddressBook * current;
    
    for (int i = 1; i < n; i++) {
        cin >> name;
        cin >> lastname;
        cin >> email;
        
        current = new AddressBook(name, lastname, email);
        myAddressBook->add(current);
    }
    
    myAddressBook->print();
    
    return 0;
}

