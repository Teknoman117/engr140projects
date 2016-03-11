#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

int main ()
{
    map<int, vector<string> > values;
    
    while(1)
    {
        string input;
        cin >> input;
        
        if(input != "quit")
        {
            vector<string>& entry = values[input.size()];
            
            if(find(entry.begin(), entry.end(), input) == entry.end())
            {
                entry.push_back(input);
            }
        }
        
        else
        {
            break;
        }
    }
    
    for(map<int, vector<string> >::iterator strings = values.begin(); strings != values.end(); strings++)
    {
        cout << strings->second[0].size() << " " << strings->second.size() << endl;
    }
    
    return 0;
}
