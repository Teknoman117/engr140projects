//
//  main.cpp
//  Entries
//
//  Created by Angelo Kyrilov on 2/12/13.
//  Copyright (c) 2013 Angelo Kyrilov. All rights reserved.
//

#include <iostream>
#include "Entry.h"
int main(int argc, const char * argv[])
{
    string name, lastname, email;
    
    cin >> name;
    cin >> lastname;
    cin >> email;
    
    Entry myEntry;
    
    myEntry.setName(name);
    myEntry.setLastname(lastname);
    myEntry.setEmail(email);
    
    myEntry.print();
    
    return 0;
}

