/*
* Author: Jonathan McElroy
* Assignment Title: Fin Max Value
* Assignment Description: This programs defines a function which will return the
*            maximum value of an array
* Due Date: 1/21/2014
* Date Created: 1/20/2014
* Date Last Modified: 1/20/2014
*
*
* Data Abstraction:
*      A variable, max, is used to store the maximum value
* Input:
*      The function is given an array of values and the length of the array
* Implementation:
*      The max value is set to the first value in the array
*      The program loops through the values in the array
*          If an value is greater that the max value, replace the max value with that value
* Output:
*      The maximum value in the array is returned
* Assumptions:
*      It is assumed that the array has at least one element
*      It is assumed that the given length is the same size as the number of
*           elements in the array
*/

#ifndef __MAXVALUE_H
#define __MAXVALUE_H

//************************************************************
// description: calculate the maximum value in a array       *
// return: same type as elements in array                    *
// pre: array has at least one value, given length is correct*
// post: returns the maximum                                 *
//                                                           *
//************************************************************
template <typename T>
T maxValue( T a[ ] , int S ) {
    // Data Abstraction
    T max;  // The maximum value in the array

    // Data Processing
    // Calculate the maximum by getting the first value, then comparing item
    // against all the others. If another happens to be larger, replace the
    // value in max and continue.
    max = a[0];
    for(int i=1; i<S; i++) {
        if (a[i] > max) {
            max = a[i];
        }
    }

    // Output
    return max;
}

#endif // define __MAXVALUE_H
