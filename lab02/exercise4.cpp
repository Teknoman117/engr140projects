#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

int main ()
{
    while(1)
    {
        // Fetch the input string
        int number;
        cin >> number;
        
        if(number < 0)
            break;
            
        cout << hex << number << " ";
        
        // convert to binary
        for(int i = log2(number); i >= 0; i--)
        {
            if(number & (1 << i))
                cout << "1";
            else
                cout << "0";
        }
        
        cout << endl;
    }
    
    return 0;
}

