/*******************************************************************************
 Filename:                  galacticBonding.cpp
 Author:                    Jonathan McElroy
 Assignment Title:          Galactic Bonding
 Assignment Description:    This will find the constellations given a list of
                            stars from the input
 Due Date: 04/06/2014
 Date Created: 04/02/2014
 Date Last Modified: 04/06/2014
*******************************************************************************/
/*
 * Data Abstraction:
 *      Variable numInputs is initialized with the number of inputs
 *      Variables numStars and dist are initialize with the number of stars and with the
 *          square of the max distance the stars in a constellation can be apart
 *          respectively for the current input
 *      Variable stars is a vector of pairs with each pair being the coordinates of the
 *          star
 *      Variable ds is a disjoint set containing the constellations
 * Input: 
 *      The program reads the number of inputs first. Then for each input, it reads the
 *          number of stars and the max distance for each star. Then it reads each pair of
 *          coordinates for the stars
 * Implementation:
 *      For each starIndex up to numInputs
 *          For each otherStarIndex less than the starIndex
 *              If the distance between the stars at the indexes is less than the max distance
 *                  Union the sets in ds
 * Output: 
 *      For each input, the input number is written to stdout followed by the number of
 *          constellations
 * Assumptions: 
 *      It is assumed that the number of inputs given matches the actual number of inputs
 *      It is assumed that the number of stars given matches the actual number of stars
 */
#include <iostream>
#include <vector>
#include "program5.h"

using namespace std;

class disjointSet{
    private:
        vector<int> pset;
    public:

        disjointSet(){
            pset.assign(MAX_STARS,0);
        }

        void initSet(int N ){
            for(size_t i = 0; i < N; i++)
                pset[i] = i;
        }

        int findSet(int i){
            if (pset[i] == i)
                return i;
            else
                return (pset[i]=findSet(pset[i]));
        }

        bool isSameSet(int i, int j){
            return findSet(i) == findSet(j);
        }

        void unionSet(int i, int j){
            pset[findSet(i)] = findSet(j);
        }

        int countSets(int N){
            int count = 0;

            for(size_t i = 0; i < N; i++){
                if(pset[i] == i) count++;
            }

            return count;
        }
};

double distSqr(pair<double, double> p1, pair<double, double> p2) {
    double xdiff = p1.first - p2.first;
    double ydiff = p1.second - p2.second;
    return xdiff*xdiff + ydiff*ydiff;
}

int main() {
    // get the number of input tests
    int numInputs;
    cin >> numInputs;

    // iterate through the inputs
    for(int inputNum=0; inputNum<numInputs; inputNum++) {
        // get the number of stars in the current input and the distance
        int numStars;
        double dist;
        cin >> numStars >> dist;

        // we don't want to call sqrt, so use the square of all the distances
        dist = dist*dist;

        // create the vector of stars and disjoint set of star indexes
        vector<pair<double, double> > stars(numStars);
        disjointSet ds;
        ds.initSet(numStars);

        // iterate through each star
        for(int starNum=0; starNum<numStars; starNum++) {
            // get the coordinates of this star
            double x, y;
            cin >> x >> y;
            stars[starNum] = make_pair(x, y);

            // for each of the other stars already found
            for(int i=0; i < starNum; i++) {
                // if the distance is less than the max distance
                if (distSqr(stars[starNum], stars[i]) <= dist) {
                    // combine the constellations
                    ds.unionSet(starNum, i);
                }
            }
        }

        cout << "Case " << inputNum+1 << ": " << ds.countSets(numStars) << endl;
    }

    return 0;
}
