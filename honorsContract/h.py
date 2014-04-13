#!/usr/bin/env python

from random import randint, shuffle, sample

INF = float('inf')

# input: a list
# output:   1 if there are any duplicates in the list
#           0 if the list contain exactly one of each element from 1 to len(list)
#           -1 otherwise
def canMakeRun(L):
    #print "CanMakeRun:", L, ":",
    setL = set(L)
    if len(L) > len(setL):
        #print "Too many elements"
        return 1
    return (setL == set(range(1, len(L)+1))) - 1

# L should be a range out of order. This will tell us the least amount of moves
# to combine the dolls
def solveSingleSet(L, debug = False):
    if len(L) == 1:
        if debug: print L, ":", 0
        return 0
    minLeft = minRight = float('inf')
    maximum = L[0]
    maxIndex = 0
    for index, num in enumerate(L[1:]):
        if num > maximum:
            minLeft = min(minLeft, maximum, minRight)
            minRight = float('inf')
            maximum = num
            maxIndex = index+1
        elif num < minRight:
            minRight = num

    if minLeft == float('inf') or minRight == float('inf'):
        val = 1 + solveSingleSet(L[:maxIndex]+L[maxIndex+1:], debug)
        print L, ":", val
        return val
    else:
        minSide = max(minLeft, minRight)
        leftOpens = rightOpens = 0
        for num in L[:maxIndex]:
            if num >= minSide:
                leftOpens+=1
        for num in L[maxIndex+1:]:
            if num >= minSide:
                rightOpens+=1
        val = solveSingleSet(L[:maxIndex], debug) + solveSingleSet(L[maxIndex+1:], debug) + leftOpens + rightOpens + 2
        print L, ":", val
        return val
        # [(5, 10, 6, 17, 15, <1>, 4, 11, 16, 18,  7), 19,(12, 9,  3,  8, 14, <2>,  13)]
        # [(5, 10, 6, 17, 15, <1>, 4, 11, 16),18,(<7>)]  [(12, 9, <3>, 8,)14,(<2>,  13)]

def solve(L, debug = False):
    runs = [[i, i+1] for i in range(len(L))]
    return solveRecurse(L, runs, debug)

def solveRecurse(L, runs, debug = False):
    # group going right
    groupDirection(L, runs, 1, debug)
    # group going left
    choice = groupDirection(L, runs, -1, debug)

    if isinstance(choice, str):
        return choice
    if choice == -1:
        print "There are no choices"
        return sum(map(solveSingleSet, (getList(L, i, i) for i in runs), [True]*len(runs)))
    else:
        print "Choice at index", choice
        rightRuns = [run[:] for run in runs]
        val = rightRuns.pop(choice)
        rightRuns[choice][0] = val[0]
        right = solveRecurse(L, rightRuns, debug)
        print "Right:", right

        #if choice > 0 and not canMakeRun(runs[choice-1]):
        leftRuns = [run[:] for run in runs]
        val = leftRuns.pop(choice)
        leftRuns[choice-1][1] = val[1]
        left = solveRecurse(L, leftRuns, debug)
        print "Left:", left
        #else:
            #left = INF
            #print "Skipping Left"

        return min(left, right)

def groupDirection(L, runs, direction, debug = False):
    if direction == 1:
        start = 0
        end = lambda : len(runs)
    elif direction == -1:
        start = len(runs)-1
        end = lambda: -1
    else:
        raise Exception("Direction must be one of 1 and -1: " + str(direction))

    bothWays = -1

    print "Starting in the ", direction, " direction"
    printList(L, runs)
    index = start
    while index != end():
        #if debug: printList(L, runs)

        # the indexes we can continue to
        rightIndex = leftIndex = index

        # continue to the right until we reach the end or are not not finished yet
        while rightIndex < len(runs) and canMakeRun(getList(L, runs[index], runs[rightIndex])) < 0:
            rightIndex += 1
        # if we reached the end or have too many elements
        if rightIndex == len(runs) or canMakeRun(getList(L, runs[index], runs[rightIndex])) != 0:
            rightIndex = index

        # continue to the left until we reach the beginning or are not not finished yet
        while leftIndex >= 0 and canMakeRun(getList(L, runs[leftIndex], runs[index])) < 0:
            leftIndex -= 1
        # if we reached the beginning or have too many elements
        if leftIndex == -1 or canMakeRun(getList(L, runs[leftIndex], runs[index])) != 0:
            #print "Changed left index"
            leftIndex = index

        #if debug:
            #print "Current Run:"
            #printRun(L, *runs[index])
            #print "Right run:"
            #printRun(L, runs[index][0], runs[rightIndex][1])
            #print "Left run:"
            #printRun(L, runs[leftIndex][0], runs[index][1])

        # if we can continue right
        if rightIndex > index:
            # if we can continue left
            if leftIndex < index:
                # this can extend both ways
                bothWays = index
                index += direction
            else:
                # join right
                val = runs.pop(index)
                runs[index][0] = val[0]
                if bothWays > index: bothWays-=1
        else:
            # if we can continue left
            if leftIndex < index:
                # join left
                val = runs.pop(index)
                index -= 1
                runs[index][1] = val[1]
                if bothWays > index: bothWays-=1
            else:
                # if the current run is complete
                if canMakeRun(getList(L, runs[index], runs[index])) == 0:
                    index += direction
                # if the first run or last run cannot complete
                elif index == 0 or index == len(runs)-1:
                    return "Not Solvable"
                else:
                    canMakeRunVal = canMakeRun(getList(L, runs[index-1], runs[index+1]))
                    # if the extended run has too many elements
                    if canMakeRunVal == 1:
                        return "Not Solvable"
                    # if the extended run does not enough elements or is complete
                    else:
                        runs.pop(index)
                        val = runs.pop(index)
                        index -= 1
                        runs[index][1] = val[1]
                        if bothWays > index: bothWays -= 2
        #if debug:
            #printList(L, runs)
            #raw_input()
            #print
            #print

    printList(L, runs)
    print

    # if the list is solvable, return the last instance there is a choice
    return bothWays

def printList(L, runs):
    print L
    print '[' + '><'.join(' '*(1+3*(i[1]-i[0]-1)) for i in runs) + ']'

def printRun(L, left, right):
    print L
    print ' '*3*left + '<' + ' '*(3*(right-left)-2) + '>'

def getList(L, left, right):
    return L[left[0]:right[1]]

if __name__ == "__main__":
    debug = True

    L = []
    if debug:
        num =  randint(2, 10)
        print num, "sets"
        for i in range(num):
            A = list(range(1,randint(4, 6)))
            shuffle(A)
            L.extend(A)
    else:
        pass
        #L = [1, 2, 4, 3, 1, 2, 5, 6, 4, 3, 5, 4, 1, 3, 2, 4, 1, 3, 2]
        #L = [1, 2, 3, 1, 3, 2, 1, 3, 2, 1, 4, 2, 3]
        #L = [3, 2, 1, 3, 1, 2, 1, 3, 4, 2, 3, 1, 2]
        #L = [1, 3, 2, 2, 3, 4, 1, 6, 5, 3, 4, 6, 1, 5, 2, 3, 1, 4, 2]
        #L = [1, 4, 5, 2, 3, 6, 4, 1, 5, 3, 2]
        #L = [3, 1, 6, 5, 4, 2, 1, 2, 6, 3, 4, 5, 5, 1, 4, 2, 3]
        #L = [3, 2, 1, 1, 3, 2, 4, 2, 3, 5, 6, 1]

        #L = [2, 4, 3]
        #L = [1, 2, 1, 2, 4, 3, 3]
        #L = [1, 2, 4]

        #L = [3, 2, 1, 4, 5, 3, 2, 1]
        L = [3, 2, 1, 4, 5, 6, 7, 4, 3, 2, 1, 5, 4, 3, 2, 1]

    print "Dolls:", L
    print "Sorted:"
    print solve(L, debug)

    #print solveSingleSet(sample(range(1, 20), 19), debug)

'''
3 2 1 3 1 2 1 3 4 2 3 1 2
<   > <   > <     > <   >

1 3 2 2 3 4 1 6 5 3 4 6 1 5 2 3 1 4 2
<   > <         > <         > <     >

1 4 5 2 3  6   4 1 5 3 2
<       > {<>} <       >

3 1 6 5 4 2 1 2 6 3 4 5 5 1 4 6 2 3
<         > <         > <         >

3, 2, 1, 1, 3, 2, 4, 2, 3, 5, 6, 1
<     >  <     >  <              >
'''
