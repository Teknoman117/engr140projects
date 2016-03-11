#include <iostream>
#include <map>
#include <string>

using namespace std;

int main ()
{
    map<char, string> strings;
    
    while(1)
    {
        string input;
        cin >> input;
        
        if(input == "quit")
            break;
        
        if(input.size() > 1)
        {
            strings[input[0]] = input;
        }
        else
        {
            cout << strings[input[0]] << endl;
        }
    }
    
    return 0;
}
