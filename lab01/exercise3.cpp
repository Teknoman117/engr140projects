#include <iostream>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <string>
#include <cctype>

using namespace std;

int isnotalnum(int c)
{
    return !isalnum(c);
}

void toLowercase(std::string& data)
{
    std::transform(data.begin(), data.end(), data.begin(), ::tolower);
}

void toAlphanumeric(std::string& data)
{
    data.erase(std::remove_if(data.begin(), data.end(), (int(*)(int))isnotalnum), data.end());
}

void sanitize(std::string& data)
{
    toLowercase(data);
    toAlphanumeric(data);
}

int main ()
{
    ifstream file("words.txt");
    string line;
    char *saveptr = NULL; 
    size_t count = 0;

    string compareWord;
    cin >> compareWord;
    sanitize(compareWord);

    while(getline(file, line))
    {
        char *token = strtok_r((char *) line.data(), " ", &saveptr);
        if(token == NULL)
            continue;

        do
        {
            string test(token);
            sanitize(test);
            if(test == compareWord)
                count++;
        }
        while((token = strtok_r(NULL, " ", &saveptr)) != NULL);
    }

    cout << count << endl;

    return 0;
}
