#!/usr/bin/env python

from random import randint

if __name__ == "__main__":
    machines = 3
    print machines, "computers"
    battPerChip = 2
    #batteries = sorted(randint(1, 8*machines*battPerChip) for _ in range(2*machines*battPerChip))
    batteries = [1, 1, 1, 3, 4, 6, 9, 12, 15, 18, 21, 24]
    print batteries

    diff = [batteries[i+1]-batteries[i] for i in range(len(batteries)-1)]
    print diff

    maxDiff = diff[0]
    indexes = [0]

    print "Using batteries 1 and 2"

    for i in range(1, machines):
        print "Getting computer", i+1
        minDiff = float('inf')
        for index in range(2+2*battPerChip*i):
            print "Testing batteries", index+1, "and", index+2, ":", batteries[index], batteries[index+1]
            if diff[index] < minDiff:
                print "The difference is the best found so far",
                found = False
                for pastIndex in indexes:
                    if pastIndex == index + 1:
                        # we may be able to push it back
                        pass
                    elif abs(pastIndex - index) <= 1:
                        print "; however, it has already been taken",
                        found = True
                        break
                print
                if not found:
                    minIndex = index
                    minDiff = diff[index]
        print "Using batteries", minIndex+1, "and", minIndex+2
        print "Values:", batteries[minIndex], batteries[minIndex+1]
        print "Power of batteries:", batteries[minIndex], ",", batteries[minIndex+1]
        indexes.append(minIndex)
        if minDiff > maxDiff:
            print "New maximum difference"
            maxDiff = minDiff

    print "Maximum minimum difference:", maxDiff

'''
2 3
1 2 3 4 5 6 7 8 9 10 11 12
2 2
3 1 3 3 3 3 3 3
5 3
7 9 12 13 20 21 23 23 24 28 29 29 33 33 34 34 35 35 42 45 45 47 48 50 50 52 53 57 59 59
5 3
7 7 9 16 17 21 27 27 28 30 35 42 43 51 53 59 65 66 74 76 78 82 93 95 97 106 109 115 117 118
3 2
1 1 1 3 4 6 9 12 15 18 21 24
'''
