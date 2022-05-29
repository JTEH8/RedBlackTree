//Header guard
#ifndef TREE_H
#define TREE_H
#include <cstring>

//Tree Clas
class Tree{
public:
    //Vairables
    int data;
    Tree* left;
    Tree* right;
    Tree* parent;
    //Color (False is black, True is Red)
    int color;
    //Constructor and Destructor
    Tree(int newVal);
    ~Tree();
//Function prototypes
void setValue(int newVal);
void setLeft(Tree* newLeft);
void setRight(Tree* newRight);
void setParent(Tree* newParent);
void setColor(int newColor);
int getData();
Tree* getRight();
Tree* getLeft();
Tree* getParent();
int getColor();
};

#endif