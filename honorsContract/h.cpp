#include <algorithm>
#include <iostream>
#include <list>
#include <set>
#include <list>
#include <utility>
#include <random>
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

const int INF = 2000000000;

// input: a list
// output:   1 if there are any duplicates in the list
//           0 if the list contain exactly one of each element from 1 to list.size()
//           -1 otherwise
int canMakeRun(list<int> L) {
    int size = L.size();
    L.sort();
    L.unique();

    if (L.size() < size) {
        // too many elements
        return 1;
    }
    int value = 1;
    for(auto i=L.begin(); i != L.end(); i++) {
        if (*i != value) {
            // one of the numbers is missing
            return -1;
        }
        value++;
    }
    // the list contains all the number from 1 to size
    return 0;
}

// gets a list from the first element of the first pair and the second element of the second pair
list<int> getList(int L[], pair<int, int> left, pair<int, int> right) {
    return list<int>(L+left.first, L+right.second);
}

void joinRun(list<pair<int, int> >& runs, iter& from, iter& to) {
    int first = from->first;
    runs.erase(from, to);
    to->first = first;
    from = to;
}

int solveSingleSet(int L[], const int begin, const int end) {
    if(end - begin <= 1) {
        return 0;
    }
    int minLeft = INF;
    int minRight = INF;
    int maximum = L[begin];
    int maxIndex = begin;
    for(int i=begin+1; i<end; i++) {
        if(L[i] > maximum) {
            minLeft = min(min(minLeft, maximum), minRight);
            minRight = INF;
            maximum = L[i];
            maxIndex = i;
        }
        else if(L[i] < minRight) {
            minRight = L[i];
        }
    }
    if(minLeft == INF) {
        return 1 + solveSingleSet(L, begin+1, end);
    }
    else if(minRight == INF) {
        return 1 + solveSingleSet(L, begin, end-1);
    }
    else {
        int minSide = max(minLeft, minRight);
        int leftOpens = 0;
        int rightOpens = 0;
        for(int i=begin; i<maxIndex; i++) {
            if(L[i] >= minSide){
                leftOpens++;
            }
        }
        for(int i=maxIndex+1; i<end; i++) {
            if(L[i] >= minSide){
                rightOpens++;
            }
        }
        return solveSingleSet(L, begin, maxIndex) + solveSingleSet(L, maxIndex+1, end) + leftOpens + rightOpens + 2;
    }
}

int solve(int L[], const int size) {
    // create the list of runs
    list<pair<int, int> > runs;
    for (int i=0; i<size; i++) {
        runs.push_back(make_pair(i, i+1));
    }

    return solveRecursive(L, size, runs, size);
}

int solveRecursive(int L[], const int size, list<pair<int, int> >& runs, const int sizeRuns) {
    printArray(L, size);
    printRuns(runs);
    cout << endl;
    groupForward(L, size, runs);
    auto choice = groupBackward(L, size, runs);

    if(choice == runs.end()) {
        cout << "There are no choices" << endl;
        int sum = 0;
        for(auto i = runs.begin(); i != runs.end(); i++) {
            int val = solveSingleSet(L, i->first, i->second);
            printArray(L, size);
            printRuns(runs);
            cout << "   " << val << endl;
            sum += val;
        }
        return sum;
    }
    else {
        cout << "There is a choice" << endl;
        auto rightRuns = runs;
        auto rightChoice = rightRuns.begin();
        while(*rightChoice != *choice) {
            rightChoice++;
        }
        auto right = rightChoice;
        right++;
        joinRun(rightRuns, rightChoice, right);
        printArray(L, size);
        printRuns(rightRuns);
        int rightVal = solveRecursive(L, size, rightRuns, sizeRuns-1);

        auto leftRuns = runs;
        auto leftChoice = leftRuns.begin();
        while(*leftChoice != *choice) {
            leftChoice++;
        }
        auto left = leftChoice;
        left--;
        joinRun(leftRuns, left, leftChoice);
        printArray(L, size);
        printRuns(leftRuns);
        int leftVal = solveRecursive(L, size, leftRuns, sizeRuns-1);

        return min(rightVal, leftVal);
    }
}

iter groupForward(int L[], const int size, list<pair<int, int> >& runs) {
    // start is the element before the first element
    auto beforeBegin = --runs.begin();
    auto index = runs.begin();
    auto beforeEnd = --runs.end();
    auto choice = runs.end();

    while(index != runs.end()) {
        printArray(L, size);
        printRuns(runs);
        printCurrentIndex(runs, index);

        // the indexes we can continue to
        auto rightIndex = index;
        auto leftIndex = index;

        // continue to the right until we reach the end or are not not finished yet
        while (rightIndex != runs.end() and canMakeRun(getList(L, *index, *rightIndex)) < 0) {
            rightIndex++;
        }
        // if we reached the end or have too many elements
        if (rightIndex == runs.end() or canMakeRun(getList(L, *index, *rightIndex)) != 0) {
            rightIndex = index;
        }

        // continue to the left until we reach the beginning or are not not finished yet
        while (leftIndex != beforeBegin and canMakeRun(getList(L, *leftIndex, *index)) < 0) {
            leftIndex--;
        }
        // if we reached the beginning or have too many elements
        if (leftIndex == beforeBegin or canMakeRun(getList(L, *leftIndex, *index)) != 0) {
            leftIndex = index;
        }

        // if we can continue right
        if (rightIndex != index) {
            // if we can continue left
            if (leftIndex != index) {
                // this can extend both ways
                //bothWays = index;
                choice = index;
                index++;
            }
            else {
                // join right
                auto old_index = index;
                index++;
                joinRun(runs, old_index, index);
            }
        }
        else {
            // if we can continue left
            if (leftIndex != index) {
                // join left
                auto old_index = index;
                index--;
                joinRun(runs, index, old_index);
            }
            else {
                // if the current run is complete
                if (canMakeRun(getList(L, *index, *index)) == 0) {
                    index++;
                }
                // if the first run or last run cannot complete
                else if (index == runs.begin() or index == beforeEnd) {
                    throw "Not Solvable";
               }
                else {
                    auto right_of_index = index;
                    right_of_index++;
                    index--;
                    int canMakeRunVal = canMakeRun(getList(L, *index, *right_of_index));
                    // if the extended run has too many elements
                    if (canMakeRunVal == 1) {
                        throw "Not Solvable";
                    }
                    // if the extended run does not enough elements or is complete
                    else {
                        joinRun(runs, index, right_of_index);
                    }
                }
            }
        }
        printRuns(runs);
        cout << endl;
    }
    return choice;
}

iter groupBackward(int L[], const int size, list<pair<int, int> >& runs) {
    auto beforeBegin = --runs.begin();
    auto index = --runs.end();
    auto beforeEnd = --runs.end();
    auto choice = --runs.begin();

    while(index != beforeBegin) {
        printArray(L, size);
        printRuns(runs);
        printCurrentIndex(runs, index);

        // the indexes we can continue to
        auto rightIndex = index;
        auto leftIndex = index;

        // continue to the right until we reach the end or are not not finished yet
        while (rightIndex != runs.end() and canMakeRun(getList(L, *index, *rightIndex)) < 0) {
            rightIndex++;
        }
        // if we reached the end or have too many elements
        if (rightIndex == runs.end() or canMakeRun(getList(L, *index, *rightIndex)) != 0) {
            rightIndex = index;
        }

        // continue to the left until we reach the beginning or are not not finished yet
        while (leftIndex != beforeBegin and canMakeRun(getList(L, *leftIndex, *index)) < 0) {
            leftIndex--;
        }
        // if we reached the beginning or have too many elements
        if (leftIndex == beforeBegin or canMakeRun(getList(L, *leftIndex, *index)) != 0) {
            leftIndex = index;
        }

        // if we can continue right
        if (rightIndex != index) {
            // if we can continue left
            if (leftIndex != index) {
                // this can extend both ways
                choice = index;
                index--;
            }
            else {
                // join right
                auto old_index = index;
                index++;
                joinRun(runs, old_index, index);
            }
        }
        else {
            // if we can continue left
            if (leftIndex != index) {
                // join left
                auto old_index = index;
                index--;
                joinRun(runs, index, old_index);
            }
            else {
                // if the current run is complete
                if (canMakeRun(getList(L, *index, *index)) == 0) {
                    index--;
                }
                // if the first run or last run cannot complete
                else if (index == runs.begin() or index == beforeEnd) {
                    throw "Not Solvable";
                }
                else {
                    auto left_of_index = index;
                    left_of_index--;
                    index++;
                    int canMakeRunVal = canMakeRun(getList(L, *left_of_index, *index));
                    // if the extended run has too many elements
                    if (canMakeRunVal == 1) {
                        throw "Not Solvable";
                    }
                    // if the extended run does not enough elements
                    else {
                        joinRun(runs, left_of_index, index);
                    }
                }
            }
        }
        printRuns(runs);
        cout << endl;
    }
    return choice;
}

int main() {

    //int size;
    //cin >> size;
    //int* L = new int[size];
    //for(int i=0; i < size; i++) {
        //cin >> L[i];
    //}
    
    srand(time(NULL));
    for(int count=0; count<100; count++) {
        int size = rand()%16 + 15;
        int* L = new int[size];

        int index = 0;
        while(index < size) {
            int new_size = min(rand()%3 + 3, size - index);
            for(int i=0; i < new_size; i++) {
                L[index+i] = i+1;
            }
            random_shuffle(L+index, L+index+new_size);
            index += new_size;
        }

        cout << "Dolls: ";
        printArray(L, size);

        try {
            cout << solve(L, size) << endl;
        }
        catch (const char* error) {
            cout << "impossible" << endl;
        }

        delete [] L;
        
        cin.get();
    }

    return 0;
}
