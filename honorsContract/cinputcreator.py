#!/usr/bin/env python
from random import choice, random, randrange

def main():
    graph = [0]
    nodes = 10000
    edges = 10000
    cars = 200
    with open("c.input2", 'w') as f:
        f.write('{0} {1} {2}\n'.format(nodes, edges, cars))
        for i in range(1, 10000):
            num = choice(graph)
            graph.append(i)
            if (randrange(10) == 0):
                graph.remove(num)
            f.write('{0} {1} {2}\n'.format(1+num, 1+i, 5+randrange(10)))
        for i in range(cars):
            f.write('{0} '.format(1+randrange(nodes)))
        f.write('\n')
    return 0;

if __name__ == "__main__":
    main();
