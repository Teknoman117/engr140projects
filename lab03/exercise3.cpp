#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <numeric>

using namespace std;

int main ()
{
    vector<int> values;
    
    while(1)
    {
        int number;
        cin >> number;
        
        if(number > 0)
            values.push_back(number);
        
        else if(number < 0)
        {
            vector<int>::iterator toErase;
            while((toErase = find(values.begin(), values.end(), abs(number))) != values.end())
            {
                values.erase(toErase);
            }
        }
        
        else
        {
            int sum = accumulate(values.begin(), values.end(), 0);
            cout << values.size() << " " << sum << endl;
            break;
        }
    }
    
    return 0;
}
