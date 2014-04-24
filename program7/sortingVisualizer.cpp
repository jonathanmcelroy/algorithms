#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <GL/glut.h>
#include "GLUT_Plotter.h"
using namespace std;

void drawfunc(void);
void setpixel(char* buf, int x, int y, int r, int g, int b);

void keyfunc(unsigned char, int, int);

GLUT_Plotter* screen;
int* data;

void printData();
void RandomizeData();
void BubbleSort();
void SelectionSort();
void InsertionSort();
void MergeSort();
void HeapSort();
void QuickSort();
void _QuickSort(int start, int end);
void _MergeSort(int start, int end);

int main(int argc, char **argv) {
    screen = new GLUT_Plotter();
    screen->DisplayFunc(drawfunc);
    screen->KeyboardFunc(keyfunc);

    data = new int[screen->getWidth()];

    for(int i = 0; i < screen->getWidth(); i++) {
        data[i] = rand() % screen->getWidth();
    }

    screen->Draw(data);
    screen->MainLoop();
}

void keyfunc(unsigned char k, int x, int y) {
    int i,j;

    switch(toupper(k)) {
    case 27:
        // escape key
        exit(0);
        break;

    case GLUT_KEY_UP:
        // up arrow
        break;

    case 'C':
        screen->Clear();
        break;

    case 'P':
        screen->ShowHistory();
        break;

    case 'B':
        // show bubble sort
        BubbleSort();
        break;

    case 'S':
        // show selection sort
        SelectionSort();
        break;

    case 'H':
        // show Heap sort
        HeapSort();
        break;

    case 'I':
        // show Insertion sort
        InsertionSort();
        break;

    case 'Q':
        // show Quick sort
        QuickSort();
        break;

    case 'M':
        // show Merge  sort
        MergeSort();
        break;

    case 'R':
        // Randomize Data
        RandomizeData();
        break;
    }
}

// Bubble Sort
void BubbleSort() {
    for(int i = 0; i < screen->getWidth() - 1; i++) {
        for(int j = 0; j < screen->getWidth() - 1 - i; j++) {
            if(data[j] > data[j + 1]) {
                swap(data[j], data[j + 1]);
            }
        }

        screen->Draw(data);
    }
}

// Selection Sort
void SelectionSort() {
    for(int i = 0; i < screen->getWidth(); i++) {
        int best = data[i];
        int bestI = i;

        for(int j = i + 1; j < screen->getWidth(); j++) {
            if(best > data[j]) {
                best = data[j];
                bestI = j;
            }
        }

        swap(data[i], data[bestI]);
        screen->Draw(data);
    }
}

// Insertion Sort
void InsertionSort() {
    for(int i = 0; i < screen->getWidth(); i++) {
        int j = i;

        while(j > 0 and data[j] < data[j - 1]) {
            swap(data[j], data[j - 1]);
            j--;
        }

        screen->Draw(data);
    }
}

// Merge Sort
void MergeSort() {
    _MergeSort(0, screen->getWidth());
}
void _MergeSort(int start, int end) {
    // if the run size is 1, it is sorted
    if(start + 1 >= end) {
        return;
    }

    int mid = (start + end) / 2;
    // sort left half
    _MergeSort(start, mid);
    // sort right half
    _MergeSort(mid, end);

    // memory to merge to
    int* merge = new int[end - start];
    int leftTape = start;
    int rightTape = mid;
    for(int i = 0; i < end - start; i++) {
        if(rightTape == end or (leftTape < mid and data[leftTape] <= data[rightTape])) {
            merge[i] = data[leftTape++];
        }
        else {
            merge[i] = data[rightTape++];
        }
    }
    // move copy back to original array
    for(int i = 0; i < end - start; i++) {
        data[start + i] = merge[i];
    }
    screen->Draw(data);

}

// Heap Sort
void HeapSort() {
    make_heap(data, data + screen->getWidth());
    screen->Draw(data);
    for(int index = screen->getWidth(); index > 0; index--) {
        pop_heap(data, data + index);
        screen->Draw(data);
    }
}

// Quick Sort
void QuickSort() {
    _QuickSort(0, screen->getWidth());
}
void _QuickSort(int start, int end) {
    if(start + 1 >= end) {
        return;
    }
    int last = end - 1;

    int mid = (end + start) / 2;
    if(data[last] < data[start]) {
        swap(data[last], data[start]);
    }
    if(data[mid] < data[start]) {
        swap(data[mid], data[start]);
    }
    if(data[last] < data[mid]) {
        swap(data[last], data[mid]);
    }

    int pivot = data[mid];
    swap(data[mid], data[last]);
    int index = start;
    for(int i = start; i < last; i++) {
        if(data[i] <= pivot) {
            swap(data[i], data[index]);
            index++;
        }
    }
    swap(data[index], data[last]);
    screen->Draw(data);

    _QuickSort(start, index);
    _QuickSort(index, end);

}

// Select Random Data
void RandomizeData() {
    for(int i = 0; i < screen->getWidth(); i++) {
        data[i] = rand() % screen->getWidth();
    }

    screen->Draw(data);

}

// main draw function, gets called over and over, as fast as possible
void drawfunc(void) {
}

void setpixel(char* buf, int x, int y, int r, int g, int b) {
}

void printData() {
    for(int i = 0; i < screen->getWidth(); i++) {
        cout << data[i] << " ";
    }

    cout << endl;
}
