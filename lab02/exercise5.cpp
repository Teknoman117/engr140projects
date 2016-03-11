#include <iostream>
#include <string>

using namespace std;

int main ()
{
    while(1)
    {
        // Fetch the input string
        string input = "";
        cin >> input;
        
        if(input == "-1")
            break;
        
        int number = 0;
        string::reverse_iterator r = input.rbegin();
        for(int i = 0; i < 8, r != input.rend(); i++, r++)
        {
            number += (1 << i) * (*r == '1' ? 1 : 0);
        }
        cout << number << " ";
        
        for(string::reverse_iterator f = input.rbegin(); f != input.rend(); f++)
        {
            cout << *f;
        }
        cout << " ";
        
        for(string::iterator f = input.begin(); f != input.end(); f++)
        {
            cout << (*f == '1' ? '0' : '1');
        }
        cout << std::endl;
    }
    
    return 0;
}

