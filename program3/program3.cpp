/*******************************************************************************
* Filename:      program3.cpp
* Author:        Jonathan McElroy
* Description:   TODO: Finish Description

* Date Modified: 02/20/2014
*   - Created
*
* Date Modified: 02/23/2014
*   - Modified algorithm to use class closures instead of copying the list of
*     points to sort
*******************************************************************************/
#include <iostream>
#include <algorithm>
#include <utility>
//#include "program3.h"
using namespace std;

class orderByX {
    private:
        pair<float, float>* L;
    public:
        orderByX(pair<float, float> L[]) {this->L = L;}
        bool operator()(int x1, int x2){return this->L[x1].first < this->L[x2].first;}
};
class orderByY {
    private:
        pair<float, float>* L;
    public:
        orderByY(pair<float, float> L[]) {this->L = L;}
        bool operator()(int y1, int y2){return this->L[y1].second < this->L[y2].second;}
};
pair<int, int> closestPairOfPoints(pair<float, float> pairs[], int size);
pair<int, int> closestPairOfPoints(pair<float, float> pairs[], int x[], int start, int end);
float distSqr(pair<float, float> p1, pair<float, float> p2) {
    float x = p1.first - p2.first;
    float y = p1.second - p2.second;
    return x*x + y*y;
}

pair<int, int> closestPairOfPoints(pair<float, float> pairs[], int size) {
    int* x = new int[size];
    for(int i=0; i < size; i++) {
        x[i] = i;
    }

    sort(x, x+size, orderByX(pairs));
    pair<int, int> ret = closestPairOfPoints(pairs, x, 0, size);

    delete [] x;
    return ret;
}

pair<int, int> closestPairOfPoints(pair<float, float> pairs[], int x[], int start, int end) {
    int size = end-start;
    int middle = (start+end)/2;
    //      _ _ _ _
    //      ^   ^   ^
    //      s   m   e
    //      [  |   ]
    //      _ _ _ _ _
    //      ^   ^     ^
    //      s   m     e
    //      [  |     ]

    // this should not happen
    if (size < 2) {
        return make_pair(-1,-1);
    }
    // just returns the two points given
    else if (size == 2) {
        return make_pair(x[start], x[start+1]);
    }
    // of the triple, returns the pair that are closest
    else if (size == 3) {
        float dist01 = distSqr(pairs[x[start]], pairs[x[start+1]]);
        float dist02 = distSqr(pairs[x[start]], pairs[x[start+2]]);
        float dist12 = distSqr(pairs[x[start+1]], pairs[x[start+2]]);
        if (dist01 < dist02) {
            if (dist01 < dist12) {
                return make_pair(x[start], x[start+1]);
            }
            else {
                return make_pair(x[start+1], x[start+2]);
            }
        }
        else {
            if (dist02 < dist12) {
                return make_pair(x[start], x[start+2]);
            }
            else {
                return make_pair(x[start+1], x[start+2]);
            }
        }
    }

    // find the closest pair of points to the left and their distance
    pair<int, int> leftPair = closestPairOfPoints(pairs, x, start, middle);
    float leftDist = distSqr(pairs[leftPair.first], pairs[leftPair.second]);

    // find the closest pair of points to the right and their distance
    pair<int, int> rightPair = closestPairOfPoints(pairs, x, middle, end);
    float rightDist = distSqr(pairs[rightPair.first], pairs[rightPair.second]);

    // choose the side that is closer
    float dist;
    pair<int, int> bestPair;
    if (leftDist <= rightDist) {
        bestPair = leftPair;
        dist = leftDist;
    }
    else {
        bestPair = rightPair;
        dist = rightDist;
    }

    // collect all the points that are closer to the center than the best distance on the left
    int* leftY = new int[size];
    int leftYSize=0;
    // the point that is directly to the right of the center
    pair<float, float> otherSide = pairs[x[middle]];

    for(int pos=middle-1; pos >= start and otherSide.first - pairs[x[pos]].first < dist; pos--) {
        leftY[leftYSize] = x[pos];
        leftYSize++;
    }

    // collect all the points that are closer to the center than the best distance on the right
    int* rightY = new int[size];
    int rightYSize=0;

    // the point that is directly to the left of the center
    otherSide = pairs[x[middle-1]];

    for(int pos = middle; pos < end and pairs[x[pos]].first - otherSide.first < dist;  pos++) {
        rightY[rightYSize] = x[pos];
        rightYSize++;
    }

    // sort the points
    sort(leftY, leftY+leftYSize, orderByY(pairs));
    sort(rightY, rightY+rightYSize, orderByY(pairs));

    int leftI=0, rightI=0;
    pair<float, float> leftValue, rightValue;
    if (leftI < leftYSize and rightI < rightYSize) {
        leftValue=pairs[leftY[leftI]];
        rightValue=pairs[rightY[rightI]];
    }
    else {
        leftValue.second = 0;
        rightValue.second = 0;
    }
    // iterate left and right sides to find the closest pair over the boundary
    while(leftI < leftYSize and rightI < rightYSize) {
        // while the left side is not at the top and the left side is below the right side
        while(leftI < leftYSize and leftValue.second < rightValue.second) {
            float tempDist = distSqr(leftValue, rightValue);
            if (tempDist < dist) {
                bestPair = make_pair(leftY[leftI], rightY[rightI]);
                dist = tempDist;
            }
            leftI++;
            if (leftI < leftYSize) { 
                leftValue = pairs[leftY[leftI]];
            }
        }
        // while the right side is not at the top and the right side is below the left side
        while(rightI < rightYSize and rightValue.second < leftValue.second) {
            float tempDist = distSqr(leftValue, rightValue);
            if (tempDist < dist) {
                bestPair = make_pair(leftY[leftI], rightY[rightI]);
                dist = tempDist;
            }
            rightI++;
            if (rightI < leftYSize) {
                rightValue = pairs[rightY[rightI]];
            }
        }
    }
    // check the next one
    if (leftValue.second < rightValue.second) {
        if (leftI+1 < leftYSize) {
            float tempDist = distSqr(pairs[leftY[leftI+1]], rightValue);
            if (tempDist < dist) {
                bestPair = make_pair(leftY[leftI+1], rightY[rightI]);
                dist = tempDist;
            }
        }
    }
    else {
        if (rightI+1 < rightYSize) {
            float tempDist = distSqr(leftValue, pairs[rightY[rightI+1]]);
            if (tempDist < dist) {
                bestPair = make_pair(leftY[leftI], rightY[rightI+1]);
                dist = tempDist;
            }
        }
    }
    /*
               *|
        *       |
                | *
                |
                | *
                |
             *  |

     */
    // need to translate leftI and rightI to actual indices

    delete [] leftY;
    delete [] rightY;

    return bestPair;
}

int main() {
    int size = 10;
    float x, y;
    cin >> size;
    pair<float, float>* points = new pair<float, float>[size];
    for(int i=0; i<size; i++) {
        cin >> x >> y;
        points[i] = make_pair(x, y);
    }
    //points[0]=make_pair(3.2138, 20);
    //points[1]=make_pair(3.3384, 120);
    //points[2]=make_pair(4.178, 180);
    //points[3]=make_pair(5.0867, 80);
    //points[4]=make_pair(5.9391, 40);
    //points[5]=make_pair(6.6414, 160);
    //points[6]=make_pair(6.7779, 60);
    //points[7]=make_pair(8.9255, 100);
    //points[8]=make_pair(9.9998, 140);
    //points[9]=make_pair(11.0909, 200);
    pair<int, int> close = closestPairOfPoints(points, size);
    cout << close.first << " " << close.second << endl;
    delete [] points;
    return 0;
}

/*
Pseudo Code:
points = array of all points
sort points by x coord
index1, index2 = findBestPair(points, 0, points.size())

def findBestPair(points, left, right)
    if points.size() == 2
        return points[1], points[2]
    if points.size() == 3
        dist01 = dist(points[0], points[1])
        dist02 = dist(points[0], points[2])
        dist12 = dist(points[1], points[2])
        if dist01 < dist02
            if dist01 < dist03
                return 0, 1
            else
                return 0, 3
        else
            if dist02 < dist03
                return 0, 2
            else
                return 0, 3
    else
        // the index to the right of the dividing line
        middle = (left + right)/2

        leftInd = findBestPair(points, left, middle)
        leftDist = dist(leftInd[0], leftInd[1])
        rightInd = findBestPair(points, middle, right)
        rightDist = dist(rightInd[0], rightInd[1])

        if leftDist < rightDist:
            indices = leftInd
            bestDist = leftDist
        else
            indices = rightInd
            bestDist = rightDist

        leftPoints = []
*/
