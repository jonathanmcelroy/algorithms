#ifndef H_H
#define H_H

#include <iostream>
#include <list>
#include <utility>
using namespace std;

typedef list<pair<int, int> >::iterator iter;

void printArray(const int L[], const int size) {
    cout << "[" << L[0];
    for(int i=1; i<size; i++){
        cout << ", " << L[i];
    }
    cout << "]" << endl;
}
void printRuns(const list<pair<int, int> >& runs){
    cout << '[';
    for(list<pair<int, int> >::const_iterator index = runs.begin(); index != runs.end(); index++) {
        if(index != runs.begin()){
            cout << "><";
        }
        for(int i=0; i < 1+3*(index->second - index->first - 1); i++) {
            cout << ' ';
        }
    }
    cout << "]" << endl;
}
void printCurrentIndex(list<pair<int, int> >& runs, iter index){
    cout << ' ';
    for(list<pair<int, int> >::const_iterator temp_index = runs.begin(); temp_index != runs.end(); temp_index++) {
        if(temp_index == index){
            cout << "_";
            break;
        }
        for(int i=0; i < 3*(temp_index->second - temp_index->first); i++) {
            cout << ' ';
        }
    }
    cout << endl;
}
int canMakeRun(list<int> L);
list<int> getList(int L[], pair<int, int> left, pair<int, int> right);
void joinRun(list<pair<int, int> >& runs, iter& from, iter& to);
int solveSingleSet(int L[], const int begin, const int end);
int solve(int L[], const int size);
int solveRecursive(int L[], const int size, list<pair<int, int> >& runs, const int sizeRuns);
iter groupForward(int L[], const int size, list<pair<int, int> >& runs);
iter groupBackward(int L[], const int size, list<pair<int, int> >& runs);

#endif // define H_H
