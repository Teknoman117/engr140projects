#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

bool isPrime(int num) {
    if (num <= 3) {
        return num > 1;
    } else if (num % 2 == 0 || num % 3 == 0) {
        return false;
    } else {
        for (int i = 5; i * i <= num; i += 6) {
            if (num % i == 0 || num % (i + 2) == 0) {
                return false;
            }
        }
        return true;
    }
}

int main ()
{
    // Fetch the input string
    int number;
    cin >> number;
        
    for(int i = 0; i < number; i++)
    {
        if(isPrime(i))
            cout << i << endl;
    }
    
    return 0;
}

