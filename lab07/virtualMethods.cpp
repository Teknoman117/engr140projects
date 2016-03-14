#include <iostream>
#include <vector>

#include "Header.h"

using namespace std;

int main(int argc, const char * argv[])
{
    AppRect r(2,0,4,2);
    
    vector<AppWindow*> w;
    
    w.push_back(new CircleWin(r));
    w.push_back(new RectWin(r));
    w.push_back(new RectWin(1,1,2,3));
    w.push_back(new CircleWin(1,1,3,2));
    
    for (int i = 0; i < w.size(); i++){
        w[i]->resize(w[i]->rect().x, w[i]->rect().h * 2);
    }
    
    return 0;
}

