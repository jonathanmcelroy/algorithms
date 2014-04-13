/**
 * Filename:    f.cpp
 * Author:      Jonathan McElroy
 * Description: Solves the Low Power problem on the ICPC 2013 Final Contest
 */
#include <iostream>
#include <algorithm>
#include <set>
using namespace std;

const int INF = 2000000000;

int main() {
    int machines, batteryPower;
    while(cin >> machines) {
        // get the number of batteries on each chip
        int battPerChip;
        cin >> battPerChip;

        // calculate the total number of batteries
        int numBatt;
        numBatt = 2*machines*battPerChip;

        // gather the power of each battery available and sort the powers
        int* batteries = new int[numBatt];
        for(int i=0; i<numBatt; i++) {
            cin >> batteries[i];
        }
        sort(batteries, batteries+numBatt);

        // gather the differences between the power of each battery and its
        // successor
        int* differences = new int[numBatt-1];
        for(int i=0; i<numBatt-1; i++) {
            differences[i] = batteries[i+1] - batteries[i];
        }

        // to store the indexes of the minimal batteries already found
        set<int> indexes;

        // The maximum difference between minimum batteries in each chip
        int maxDiff;

        // this is ensured to be minimized if one of the chip contains the first
        // two batteries
        maxDiff = differences[0];
        indexes.insert(0);

        // for each machine
        for(int i=1; i<machines; i++) {
            // the best found difference for this machine
            int minDiff = INF;
            // the index of the battery found
            int minIndex = -1;

            // for each battery in the current run of batteries
            for(int index=2; index<1+2*battPerChip*i; index++) {
                // if this battery and the battery before have not been found
                if (indexes.find(index) == indexes.end() and indexes.find(index-1) == indexes.end()) {
                    // if the difference between this battery and the next <= maxDiff, the battery is good enough
                    if(differences[index] <= maxDiff) {
                        //if the next battery has not been found yet
                        if (indexes.find(index+1) != indexes.end()) {
                            minIndex = index;
                            minDiff = differences[index];
                            break;
                        }
                        // otherwise, check all subsequent batteries
                        else {
                            int tempIndex = index + 1;

                            //while tempBattery has been found and the difference between tempBattery and the next one <= maxDiff
                            while(indexes.find(tempIndex) != indexes.end() and differences[tempIndex] <= maxDiff) {
                                // check the next pair
                                tempIndex+=2;
                            }
                            //if tempBattery has not been found, all the batteries pass the test
                            if (indexes.find(tempIndex) == indexes.end()) {
                                minIndex = index;
                                minDiff = differences[index];
                                break;
                            }
                        }
                    }
                    // if the difference between this battery and the next < minDiff, we have found the best battery so far
                    if (differences[index] < minDiff) {

                        //if the next battery has not been found yet, the battery is a potential new battery
                        if (indexes.find(index+1) == indexes.end()) {
                            minIndex = index;
                            minDiff = differences[index];
                        }
                        // otherwise, check all subsequent batteries
                        else {
                            int tempIndex = index + 1;

                            //while tempBattery has been found and the difference between tempBattery and the next one <= minDiff
                            while(indexes.find(tempIndex) != indexes.end() and differences[tempIndex] < minDiff) {
                                // check the next pair
                                tempIndex+=2;
                            }
                            //if tempBattery has not been found, all the batteries pass the test
                            if (indexes.find(tempIndex) == indexes.end()) {
                                minIndex = index;
                                minDiff = differences[index];
                            }
                        }
                    }
                }

            }

            // add the batteries to found batteries
            indexes.insert(minIndex);

            // update all batteries that must move
            int tempIndex = minIndex + 1;
            //while tempBattery is found
            while (indexes.find(tempIndex) != indexes.end()) {
                indexes.insert(tempIndex+1);
                indexes.erase(tempIndex);
                tempIndex+=2;
            }
            if (minDiff > maxDiff) {
                maxDiff = minDiff;
            }
        }

        cout << maxDiff << endl;
    }

    return 0;
}

/* Pseudocode 

maxDiff = 0
for machine in each machine
    minDiff = INF;
    bool found = fale;
    for each battery in the run of batteries and while not found
        if this battery and the last battery have not been found
            if the difference for this battery and the next <= maxDiff
                // good enough battery
                if the next battery has not been found yet
                    bestBattery = battery
                    break
                else
                    // check all subsequent batteries
                    tempBattery = the next battery
                    while tempBattery has been found and the difference between tempBattery and the next one <= maxDiff
                        tempBattery = the battery two after tempBattery
                    if tempBattery has not been found
                        // all the batteries pass the test
                        // do I need to check we have gone off the array?
                        bestBattery = battery
                        break
            if the difference between this battery and the next < minDiff
                // best battery found so far
                if the next battery has not been found yet
                    // the battery is a potential new battery
                    bestBattery = battery
                    minDiff = the difference between this battery and the next
                else
                    // check all subsequent batteries
                    tempBattery = the next battery
                    while tempBattery has been found and the difference between tempBattery and the next one <= maxDiff
                        tempBattery = the battery two after tempBattery
                    if tempBattery has not been found
                        // all the batteries pass the test
                        bestBattery = battery
                        minDiff = the difference between this battery and the next
    add bestBattery to found batteries
    tempBattery = the battery after bestBattery
    while tempBattery is found
        move temp battery to the next spot
        tempBattery = the battery two after tempBattery
    if the difference between bestBattery and the next battery > maxDiff
        update maxDiff
 */
