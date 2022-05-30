//This was done with reference tO: https://www.geeksforgeeks.org/binary-tree-set-1-introduction/
#include <iostream>
#include <cstring>
#include "Tree.h"

//Constructors
Tree::Tree(int newVal){
    parent = NULL;
    right = NULL;
    left = NULL;
    data = newVal;
}
//Setting methods
void Tree::setRight(Tree* newRight){
    right = newRight;
}

void Tree::setLeft(Tree* newLeft){
    left = newLeft;
}

void Tree::setValue(int newVal){
   data = newVal;
}

void Tree::setColor(int newColor){
    color = newColor;
}

void Tree::setParent(Tree* newParent){
    parent = newParent;
}

//Get methods
Tree* Tree::getParent(){
    return parent;
}

Tree* Tree::getRight(){
    return right;
}

Tree* Tree::getSuccessor(Tree* sameTree){
//Right as far as possible
Tree* temp;
while(sameTree->getRight() != NULL){
    temp = sameTree->getRight();     
    }
    return temp;
}

Tree* Tree::getReplacement(){
    //Left once and then right as far as possible
    if(left != NULL && right != NULL){
        return left->getSuccessor(left);
    }
    //If no children
    else if(right == NULL && left == NULL){
        return NULL;
    } 
    //If nod e has one child
    else{
        if(left != NULL){
            return left;
        }
        else{
            return right;
        }
    }
    return NULL;
}

Tree* Tree::getSibling(Tree* sameTree){
    if(parent == NULL){
        return NULL;
    }
    else if(parent->getLeft() == sameTree){
        return parent->getRight();
    }
    else{
        return parent->getLeft();
    }
}

int Tree::getData(){
    return data;
}

Tree* Tree::getLeft(){
    return left;
}

int Tree::getColor(){
    return color;
}

//Destructor
Tree::~Tree(){
    
}
