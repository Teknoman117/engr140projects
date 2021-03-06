#include <iostream>
#include <string>

using namespace std;

int main ()
{
    bool shouldrun = true;
    while(shouldrun)
    {
        // Fetch the input string
        string input = "";
        for(int i = 0; i < 8; i++)
        {
            char c;
            cin.get(c);
            
            if(c == 'X')
            {
                shouldrun = false;
                break;
            }
                
            input += c;
        }
        
        int number = 0;
        string::reverse_iterator r = input.rbegin();
        for(int i = 0; i < 8, r != input.rend(); i++, r++)
        {
            number += (1 << i) * (*r == '1' ? 1 : 0);
        }
        
        cout << number;
        if(shouldrun)
        {
            cout << " ";
        }
    }
    cout << endl;
    return 0;
}

