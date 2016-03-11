#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>

using namespace std;

int main()
{
    ifstream file("code.cpp");
    string line;
    vector<string> lines;

    while(getline(file, line))
        lines.push_back(line);

    int n = 0;
    for(auto rline = lines.begin(); rline != lines.end(); rline++, n++)
        cout << *rline << " ";

    return 0;
}
