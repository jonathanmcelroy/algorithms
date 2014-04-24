/*******************************************************************************
Filename:           huffman.cpp
Author:             Jonathan McElroy
Assignment:         Huffman Encoding
Description:
    This program will either encode or decode a file using Huffman Encoding
    according to command line arguments.
Due Date:           03/23/2014
Date Created:       03/16/2014
Date Last Modified: 04/11/2014
*******************************************************************************/
/******************************************************************************
Data Abstraction:
    -huff:
        Input file stream
        Frequency distribution map for each character in the buffer, including
                an <EOF> character
        Huffman tree created from the frequency distribution map
        Char to string map representing the direction to take to reach each
                character in the Huffman tree
        Outputer object:
            Character buffer to store each bit until a character is built
            A count of how many bits are in the character buffer
            String stream buffer to store each character to write to file
            A count of how many bytes are in the string stream buffer
            Output file stream
    -unhuff:
        Inputer:
            Input file stream
            A character buffer containing the last byte read from the file 
                    stream
            A count of how many bits read from the character buffer
        Output file stream
        Frequency map built from the header in the encrypted file
        Huffman tree built from the frequency map
Input:
    Command line arguments:
        -huff: indicates huffman encoding
        -unhuff: indicates huffman decoding
        source file to decode/encode
        destination file to write to
    Input file:
        -huff: Ascii message to encode
        -unhuff: File that was encoded using this program
Implementation:
    -huff:
        The input file is opened.
        Each character is read from the input file to create a frequency 
            distribution.
        A min heap is built using the frequency distribution, where the value
            compared in the heap is the frequency.
        While the heap has more than one element, take the top two elements
            and make them the children of a node and place the next object on
            the heap, with a value of the sum of the two children's values.
        Make the last element in the heap the root of the Huffman tree.
        Create the outputer, giving it the frequency map
        Tell the outputer to write the header to the file. It will first write
            the magic number to the file, then write size of the frequency map
            to the file. Then, for each key and value in the frequency map, it
            will write the key to the file, then write the value
        Get the huffman encoding map from the hufftree, where each character
            maps to a string of '1's and '0's. The string represents the path
            from the root to the character in the tree, where each '0'
            represents a left child and each '1' represents a right child.
        For each character in the file, get the string the huffman encoding map
            maps it to and write that string to the outputer.
        For each character in that string, the outputer will write to
            corresponding bit to a character sized buffer. When the buffer is
            full, it will write the buffer to a string stream.
        When all the characters from the file have been iterated over, compare
            the original size of the file to the size of the string stream. If
            the string stream is smaller, write the contents of the string
            stream to the file. Otherwise do nothing and report to the user that
            notihng was done
    -unhuff
        Create the inputer with the input filename
        The inputer will check the file for the magic number. If it is not
            there, it will stop and report to the user that the file is not 
            Huffman encoded
        Otherwise, the inputer will get the frequency map from the file.
        Using the frequency map, create the Huffman tree, using the method
            described in the -huff part of the implementation
        Traverse the Huffman tree using the bits read from the file, '0' to go
            left, and '1' to go right. When a leaf is read, look at teh value in
            the leaf. If it is not the <EOF> char, write it to the output file.
            If it is, stop.
        The inputer contains a character buffer that stores each byte as it is
            read from the file. When a bit is asked for, give the next bit as
            either the character '0' or '1'. If the buffer is empty, grab the
            next byte from the file
Output:
    -huff:
        If the resulting file is smaller than the original file, the destination
        file will contain the encoded data. Otherwise will indicate on stdout
        that the it cannot be enoded
    -unhuff:
        Will try to unhuff the file and placing the result in the destination
        file. If this is impossible, indicate that on stdout.
Assumptions:
    Assume the user only wants to encode one file at a time
    Assume the user will not feed a file that is not a Huffman encoded file
        that contains the magic number at the beginning
******************************************************************************/

#include <fstream>
#include <iostream>
#include <string.h>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;

const char USAGE[] = "USAGE: rehuff (-huff | -unhuff) source destination";
const int MAGIC = 200 + (200 << 8) + (200 << 16) + (200 << 24);
const char MYEOF = '\0';

// Huffnode is a node in the Hufftree. It may be an inner node called
// huffinner or a leaf node called huffleaf.
class Huffnode {
    public:
        virtual ~Huffnode();
        virtual bool greaterThan(const Huffnode*) const =0;
        virtual char compValue() const =0;

        virtual map<char, string> getHuffmanEncoding(const string& prefix = "") const =0;
        virtual Huffnode* copy() const =0;

        virtual bool isLeaf() const =0;

        virtual Huffnode* getLeft() const =0;
        virtual Huffnode* getRight() const =0;
};
class Huffleaf : public Huffnode {
    private:
        char ch;
    public:

        Huffleaf(char c) :ch(c) {}

        bool greaterThan(const Huffnode* h) const {return this->ch > h->compValue();}
        char compValue() const {return ch;}
        map<char, string> getHuffmanEncoding(const string& prefix = "") const;
        Huffleaf* copy() const {return new Huffleaf(ch);}
        bool isLeaf() const {return true;}

        Huffnode* getLeft() const {return NULL;}
        Huffnode* getRight() const {return NULL;}
};
class Huffinner : public Huffnode {
    private:
        Huffnode* left;
        Huffnode* right;

    public:
        Huffinner(Huffnode* l, Huffnode* r) : left(l), right(r) {}
        ~Huffinner() {if(left) delete left; if(right) delete right;}
        bool greaterThan(const Huffnode* h) const {return this->compValue() > h->compValue();}
        char compValue() const {return left->compValue();}
        map<char, string> getHuffmanEncoding(const string& prefix = "") const;
        Huffinner* copy() const {return new Huffinner(left->copy(), right->copy());}
        bool isLeaf() const {return false;}

        Huffnode* getLeft() const {return left;}
        Huffnode* getRight() const {return right;}
};
// Hufftree represents an encryption
class Hufftree {
    private:
        Huffnode* root;
    public:
        Hufftree();
        ~Hufftree();
        Hufftree(const Hufftree& h) : root(h.root) {}
        Hufftree(map<char, int> m);
        const Huffnode* getRoot() const {return root;}
};
// The outputer eases output of encrypted data to a file
class Outputer {
    private:
        int size;

        int count;
        char buffer;

        map<char, int> freq;
        stringstream out;
        ofstream file;

        void writeHeader();
    public:
        Outputer(string destination, map<char, int> m);
        ~Outputer();
        int getSize() const;
        void write(string bytes);
        void flush();
};
// The inputer eases input of encrypted data from a file
class Inputer {
    private:
        short count;
        char buffer;
        ifstream in;
        map<char, int> m;
    public:
        Inputer(string source);
        map<char, int> getMap();
        Inputer& operator>>(char&);
};
bool encrypt(string source, string destination);
bool decrypt(string source, string destination);
bool heapComp(pair<int, Huffnode*>, pair<int, Huffnode*>);

Huffnode::~Huffnode(){}

// get the huffman encoding of a hufftree as a mapping from chars to strings
map<char, string> Huffleaf::getHuffmanEncoding(const string& prefix) const {
    map<char, string> m;
    m[this->ch] = prefix;
    return m;
}
map<char, string> Huffinner::getHuffmanEncoding(const string& prefix) const {
    map<char, string> m;
    m = this->left->getHuffmanEncoding(prefix + "0");
    map<char, string> m2 = this->right->getHuffmanEncoding(prefix + "1");
    for(map<char, string>::iterator index = m2.begin(); index != m2.end(); index++) {
        m[index->first] = index->second;
    }
    return m;
}

// build a hufftree using a frequency map
Hufftree::Hufftree(map<char, int> m) {
    // the heap to build the tree
    vector<pair<int, Huffnode*> > heap;

    // copy the values in the map to the heap
    for(map<char, int>::iterator map_value = m.begin(); map_value != m.end(); map_value++) {
        heap.push_back(make_pair(map_value->second, new Huffleaf(map_value->first)));
    }

    make_heap(heap.begin(), heap.end(), heapComp);
    while(heap.size() > 1) {
        // get the min element
        pair<int, Huffnode*> top = heap.front();
        pop_heap(heap.begin(), heap.end(), heapComp);
        heap.pop_back();

        // get the next to min element
        pair<int, Huffnode*> nextTop = heap.front();
        pop_heap(heap.begin(), heap.end(), heapComp);
        heap.pop_back();

        // add the union of the two elements
        pair<int, Huffinner*> newValue = make_pair(top.first + nextTop.first,
                new Huffinner(top.second, nextTop.second));
        heap.push_back(newValue);
        push_heap(heap.begin(), heap.end(), heapComp);
    }
    root = static_cast<Huffinner*>(heap.front().second);
}
Hufftree::~Hufftree() {
    delete root;
}

Outputer::Outputer(string destination, map<char, int> m) : count(0), buffer('\0'), freq(m), size(8+5*m.size()){
    file.open(destination.c_str(), ios::binary);
    if(!out) {
        cout << "Unable to open destination file" << endl;
        exit(1);
    }
    writeHeader();
}
Outputer::~Outputer() {
    file.close();
}
// write the header information containing the magic number and frequency map
// to the string buffer
void Outputer::writeHeader() {
    // write the magic number
    out.write((char*)(&MAGIC), sizeof(MAGIC));

    // write the frequency map
    int size = freq.size();
    out.write((char*)(&size), sizeof(size));
    for(map<char, int>::iterator index = freq.begin(); index != freq.end(); index++) {
        // output each character and its frequency
        out.write((char*)(&index->first), sizeof(index->first));
        out.write((char*)(&index->second), sizeof(index->second));
    }
}
int Outputer::getSize() const {
    // return the size, plus one if there is still stuff in the buffer
    return size + (count!=0 ? 1 : 0);
}
// write should receive a string containing '0's and '1's. Will encode the
// string in a series of bytes and write them to the stream when ready.
void Outputer::write(string bytes) {
    // for each '0' or '1' in the string
    for(int i=0; i<bytes.size(); i++) {
        // make room in the buffer for it and add it
        this->buffer<<=1;
        this->buffer+=bytes[i]-'0';

        // if the buffer is full, write it to the file and empty the buffer
        if (++this->count == 8) {
            size += sizeof(buffer);
            out.write((char*)(&buffer), sizeof(buffer));
            this->buffer = '\0';
            this->count = 0;
        }
    }
}
// write the buffer to the output file
void Outputer::flush() {
    // write the rest of the buffer if there is anything in it
    if (count != 0) {
        buffer <<= (8 - count);
        size += sizeof(buffer);
        out.write((char*)(&buffer), sizeof(buffer));
        buffer = '\0';
        count = 0;
    }
    file << out.rdbuf();
    out.str("");
    file.flush();
}

Inputer::Inputer(string source) : count(0) {
    // open the input file
    in.open(source.c_str(), ios::binary);
    if(!in) {
        cout << "Unable to open source file" << endl;
        exit(1);
    }

    // read in magic number. If it is not the correct number, this is not a
    // Huffman encoded file
    int magic;
    in.read((char*)(&magic), sizeof(magic));
    if (magic != MAGIC) {
        cout << "Input file was not Huffman Endoded" << endl;
        exit(1);
    }

    // Read in the frequency distribution map
    int size;
    in.read((char*)(&size), sizeof(size));

    char value;
    int number;
    for(int i=0; i<size; i++) {
        // read in each value in the map
        in.read((char*)(&value), sizeof(value));
        in.read((char*)(&number), sizeof(number));
        m[value] = number;
    }

    // pump the buffer
    in.read((char*)(&buffer), sizeof(buffer));
}
map<char, int> Inputer::getMap() {
    return m;
}
Inputer& Inputer::operator>>(char& ch) {
    // grab the next bit in the buffer and return '0' or '1'
    ch = '0' + ((buffer & (1<<7))>>7);
    buffer <<= 1;
    count++;

    // if there are not more bits in the buffer, grab the next character in the
    // input file
    if (count == 8) {
        in.read((char*)(&buffer), sizeof(buffer));
        count = 0;
    }
    return *this;
}

// encrypt the file given by a path and write the encryption to the other file
// given by a path
bool encrypt(string source, string destination) {
    // open the input file
    ifstream in;
    in.open(source.c_str());
    if (!in) {
        cout << "Unable to open source file" << endl;
        exit(1);
    }

    // create the frequency distribution for the file to encode. Add one <EOF>
    // char
    map<char, int> freq;
    char ch;
    int size = 0;
    while(in.get(ch)) {
        freq[ch]++;
        size++;
    }
    freq[MYEOF] = 1;

    // create a Hufftree and an outputer object to the encrypted file from the
    // frequency distribution
    Hufftree h(freq);
    Outputer out(destination, freq);

    // get the encoding map for each character
    const Huffnode* root = h.getRoot();
    map<char, string> m = root->getHuffmanEncoding();

    // write each character in the file to the outputer, ending with the <EOF>
    // char
    in.clear();
    in.seekg(0);
    while(in.get(ch)) {
        out.write(m[ch]);
    }
    out.write(m[MYEOF]);

    bool written = true;
    // if the new encoded size is smaller than the original size, write to
    // the file
    if (out.getSize() <= size) {
        out.flush();
    }
    else {
        // don't write to the file
        cout << "File will not compress" << endl;
        written = false;
    }

    in.close();
    return written;
}
// decrypt the file given by a path and write the decryption to the other file
// given by a path
bool decrypt(string source, string destination) {
    // create an inputer class with the encoded file
    Inputer in(source);

    // open the file to write the decrypted message to
    ofstream out;
    out.open(destination.c_str());
    if (!out) {
        cout << "Unable to open destination file" << endl;
        exit(1);
    }

    // get the frequency distribution from the file and create a Hufftree from
    // it
    map<char, int> freq = in.getMap();
    Hufftree h(freq);

    // read each bit from the file. For each '0', travel to the left node; for
    // each '1', travel to the right node. If the node is a leaf, write the
    // char in the node to the file and reset the node to the root of the tree.
    const Huffnode* node = h.getRoot();
    char bit;
    char lastCh = 1;
    while(lastCh != 0) {
        in >> bit;
        if (bit == '0') {
            node = node->getLeft();
        }
        else {
            node = node->getRight();
        }
        if (node->isLeaf()) {
            lastCh = node->compValue();
            if (lastCh != MYEOF) {
                out << lastCh;
                node = h.getRoot();
            }
        }
    }
    return true;
}
// helper function for the heap use to build a hufftree
bool heapComp(pair<int, Huffnode*> p1, pair<int, Huffnode*> p2) {
    return p1.first > p2.first or (p1.first == p2.first and p1.second->greaterThan(p2.second));
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << USAGE << endl;
        exit(1);
    }
    if (strcmp(argv[1], "-huff") == 0){
        encrypt(argv[2], argv[3]);
    }
    else if (strcmp(argv[1], "-unhuff") == 0) {
        decrypt(argv[2], argv[3]);
    }
    else {
        cout << USAGE << endl;
        exit(1);
    }
    return 0;
}
