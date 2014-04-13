#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
using namespace std;

const int MAGIC = 200 + (200 << 8) + (200 << 16) + (200 << 24);

class Huffnode {
    public:
        virtual ~Huffnode();
        virtual ostream& print(ostream&) const =0;
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

    ostream& print(ostream& out) const;
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
        ostream& print(ostream& out) const;
        bool greaterThan(const Huffnode* h) const {return this->compValue() > h->compValue();}
        char compValue() const {return left->compValue();}
        map<char, string> getHuffmanEncoding(const string& prefix = "") const;
        Huffinner* copy() const {return new Huffinner(left->copy(), right->copy());}
        bool isLeaf() const {return false;}

        Huffnode* getLeft() const {return left;}
        Huffnode* getRight() const {return right;}
};
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
ostream& operator<<(ostream&, const Huffnode&);
ostream& operator<<(ostream&, const Huffinner&);
ostream& operator<<(ostream&, const Huffleaf&);
ostream& operator<<(ostream&, const Hufftree&);
bool encrypt(string source, string destination);
bool decrypt(string source, string destination);
bool heapComp(pair<int, Huffnode*>, pair<int, Huffnode*>);

#endif // HUFFMAN_H
