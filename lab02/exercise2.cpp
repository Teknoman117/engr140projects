#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

int main ()
{
    // Fetch the input string
    int number;
    
    while(1)
    {
        cin >> number;
        if(number < 0)
            break;
    
        if(number % 2)
            cout << "ODD" << endl;
        else
            cout << "EVEN" << endl;
    }
    
    return 0;
}

