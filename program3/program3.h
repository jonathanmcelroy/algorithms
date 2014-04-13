#ifndef PROGRAM3_H
#define PROGRAM3_H

#include <utility>
#include <sstream>
using namespace std;

pair<int, int> closestPairOfPoints(pair<float, float> pairs[], int size);
pair<int, int> closestPairOfPoints(pair<float, float> pairs[], float x[], int start, int end);
string pairString(pair<float, float> a){
    //cout << "Received: " << a.first << ", " << a.second << endl;
    stringstream s;
    s << "(" << a.first << ", " << a.second << ")";
    //cout << "Returned: " << s.str() << endl;
    return s.str();
}

#endif // define PROGRAM3_H
