#include <iostream>
#include "Data.h"
#include "Sortable.h"
#include "Participant.h"

using namespace std;

bool sortByName(const Participant * p1, const Participant * p2){
    return p1->name > p2->name;
}

bool sortByAge(const Participant * p1, const Participant * p2){
    return p1->age > p2->age;
}

bool sortByScore(const Participant * p1, const Participant * p2){
    return p1->score < p2->score;
}

bool (*Participant::comp_cb) (const Participant *, const Participant *) = NULL;


int main(int argc, const char * argv[])
{
    
    Participant::comp_cb = &sortByScore;
    
    string name;
    int age;
    double score;
    
    int n;
    cin >> n;
    
    Data myData;
    
    for (int i = 0; i < n; i++) {
        cin >> name >> age >> score;
        myData.add(new Participant(name, age, score));
    }

    Participant::comp_cb = &sortByName;
    myData.sort();
    myData.print();
    
    Participant::comp_cb = &sortByAge;
    myData.sort();
    myData.print();
    
    Participant::comp_cb = &sortByScore;
    myData.sort();
    myData.print();
    
    return 0;
}

