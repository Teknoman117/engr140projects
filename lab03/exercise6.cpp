#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

int main ()
{
    vector<string> values;
    
    while(1)
    {
        string input;
        cin >> input;
        
        if(input != "quit")
        {
            if(input.size() <= 3)
                values.push_back(input);
            
            else
            {
                for(vector<string>::iterator strings = values.begin(); strings != values.end(); strings++)
                {
                    size_t s;
                    while((s = input.find(*strings)) != string::npos)
                    {
                        input.erase(s, strings->size());
                    }
                }
                cout << input << endl;
            }
        }
        
        else
        {
            break;
        }
    }
    
    return 0;
}
