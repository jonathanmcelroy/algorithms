/*
* Author: Jonathan McElroy 
* Assignment Title: Block Voting 
* Assignment Description: Calculate the power index of each of the parties in the describes parliaments
* Due Date: 1/30/2014
* Date Created: 1/29/2014
* Date Last Modified: 1/29/2014
 */

/*
 * Data Abstracation:
 *	Vector of the power indexes
 *	Integer containing the total number of votes in the parliament
 *	For each combination of parties, a number counting the number of votes the coalition has
 * Input: 
 * 	A squence of parliaments ending with a -1. Each parliement described by an integer describing the number of parties followed a list of the number of votes in each party.
 * 	
 * Implementation:
 *	The vector of power indexes is initialized to zeros
 *	For every int from 1 to 2^n, with each set bit representing the party (this lists all combinations of the parties)
 *		Sum the combines votes of the parties included in the current coalition
 *		If the sum is greater than half the number of votes in the parliament, a majority
 *			For each party, if their leaving causes the coalition to become a minority
 *				Increment the power index of that party
 * 
 * Output: 
 *	Outputs the power index of each party for each parliament
 * 	
 * Assumptions: 
 * 	The input is in correct format
 *	No parties hvae negative votes
 */

#include <sstream>
#include <vector>
#include <iostream>
#include <cmath>
#include "Program2.h"
using namespace std;

//************************************************************
// description: calculate the power index of each party	     *
// return: void						     *
// pre: no parties have negative votes			     *
// post: outputs to a string stream the power index of each  *
//	 party						     *
//                                                           *
//************************************************************
void BlockVoting(vector<int> parties, stringstream& out) {
    // store the number of parties
    int length = parties.size();
 
    // initialize the power indices to 0
    vector<int> powerIndex(length, 0);
 
     // get the total number of members in the parliament
     int total=0;
     for(int i=0; i<length; i++){
         total+=parties[i];
     }
 
     // for each combination of parties, each coalition
     for(int i=1; i<pow(2,length); i++) {
         // get the number of votes in the current coalition
         int sum=0;
         for(int comb=i, party=0; comb > 0; comb>>=1, party++) {
             if(comb%2==1) {
                 sum+=parties[party];
             }
         }
         // if the coalition forms a majority
         if (sum*2 > total) {
             // increment the power index of each party that has a swing vote
             for(int comb=i, party=0; comb > 0; comb>>=1, party++) {
                 if(comb%2==1 and 2*parties[party] >= 2*sum-total) {
                     powerIndex[party]++;
                 }
             }
         }
     }
 
     for(int i=0; i<length; i++){
         out << "party " << i+1 << " has power index "
              << powerIndex[i] << endl;
     }
     out << endl;
 }
 
 
 int main() {
     stringstream out;
 
     int length;
     while(cin >> length and length != -1) {
         vector<int> parties;
         for(int i=0; i < length; i++) {
             int votes;
             cin >> votes;
             parties.push_back(votes);
         }
         BlockVoting(parties, out);
     }
 
     cout << out.str() << endl;
 
 
return 0;
}
