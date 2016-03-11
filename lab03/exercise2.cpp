#include <iostream>
#include <string>

using namespace std;

int main ()
{
    while(1)
    {
        char character;
        int  count;
        
        cin >> character >> count;
        
        if(count == -2)
            break;
        
        else if(count == -1)
            cout << endl;
        
        else
            cout << string(count, character);
    }
    
    return 0;
}
