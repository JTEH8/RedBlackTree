#include <iostream>
#include <cstring>
#include <fstream>
#include "Tree.h"

/*
Author: Jeffrey Teh
Date Completed: 5/8/2022
This is an implementation of a Red Black Tree
Help for this project came from Nathan Zou and Jayden Huang
*/

//Colors
#define RESET "\033[0m"
#define RED   "\033[31m"
#define BLUE  "\033[34m"

using namespace std;
//Struct for Printing (Thanks to Nathan for the idea)
struct Trunk{
    Trunk* previous;
    char* str;
    Trunk(Trunk* previous, char* str){
        this->previous = previous;
        this->str = str;
    }
};

void showTrunks(Trunk* p){
    if(p == NULL){
      return;  
    }
    showTrunks(p->previous);
    cout << p->str;
}
//Function Prototypes
bool search(Tree* head, int input);
void printTree(Tree* start, Trunk* previous, bool isLeft);
void treeAdd(Tree* newTree, Tree* & current, Tree* & previous ,Tree* & head);
void rbtADD(Tree* newTree, Tree* current, Tree* & head);
void fileAdd(Tree* &head);
void leftRotate(Tree* & head, Tree* & target);                                         
void rightRotate(Tree* & head, Tree* & target);
void treeRemove(Tree* current, Tree* &head, Tree* parent, int data);
void treeBalance(Tree* &head, Tree* &current);

//Main method
int main(){
//Initializing variables
Tree* tree = NULL; 
char input[100];
char input2[100];
int input3 = 0;
int spacing = 3;
char input4[100];
bool running = true;
//While the program is running
while(running == true){
//Prompt text
cout << "Welcome to the Red Black Tree! You can add numbers to the tree (ADD), search for numbers in the tree (SEARCH), delete numbers from the tree (DELETE), and print the tree (PRINT). To quit type QUIT." << endl; 
cout << "Valid entries include anything in the range from 1-999." << endl;    
//Read in input
cin >> input;
//When adding numbers to the RBT
    if(strcmp(input, "ADD") == 0){
        cout << "Do you want to add via direct input (INPUT) or via file (FILE)" << endl;
        //If the user wants to add directly or by file
        cin >> input2;
        //Direct input
        if(strcmp(input2, "INPUT") == 0){
            cout << "Enter the number that you want to add to the tree." << endl;
            cin >> input3;
        
	    Tree* newTree = new Tree(input3);
        Tree* current = tree;
        Tree* previous = NULL;
	    treeAdd(newTree, current, previous, tree);
        if(current != tree){
            treeBalance(tree,current);
        }
        cout << endl << input3 << " added to tree." << endl << endl;
	    }
        else if(strcmp(input2, "FILE") == 0){
        fileAdd(tree);
        }
    }
    //If the user wants to delete
    else if(strcmp(input, "DELETE") == 0){
        cout << "Enter the number (In the tree) that you want to delete." << endl;
        cin >> input3;
        //Remove the number from the tree, if it's there
        treeRemove(tree, tree,tree->getParent(), input3);
    } 
   //Printing
    else if(strcmp(input, "PRINT") == 0){
        cout << endl;
        printTree(tree, NULL, false);
        cout << endl;
    }
    //Search the tree
    else if(strcmp(input, "SEARCH") == 0){
     cout << "What's the number that you want to search for?" << endl;
        cin >> input3;
        search(tree, input3);
        }
    //Quit RBT
    else if(strcmp(input, "QUIT") == 0){
      cout << "Thanks for using RBT!" << endl;
      running = false;
    }
    }
}
//Add a number to the tree
void treeAdd(Tree* newTree, Tree*& current, Tree*& previous, Tree* & head){
    //If the tree is empty, make the new one head
    if(head == NULL){
        head = newTree;
        current = head;
        head->setColor(0); //Head is black
    }
    //Otherwise if the new Tree's data is bigger
    else if(newTree->getData() > current->getData()){ 
        previous = current;
        current = current->getLeft();
        //And if the current Tree's right is NULL
        if(current == NULL){
            current = newTree;
            previous->setLeft(current);
            current->setParent(previous);
	    current->setColor(1);
            treeBalance(head, current);
            return;
            }
        //Otherwise go onto the right side of the tree
        else{
            treeAdd(newTree, current, previous, head);
            }
        }
        else{
        previous = current;
        current = current->getRight();
        if(current == NULL){
            current = newTree;
            previous->setRight(current);
            current->setParent(previous);
	    current->setColor(1);
	    treeBalance(head,current);
            return;
        }
        else{
          treeAdd(newTree, current, previous, head);
            }
        }

    }
//Adding numbers to the tree via file input (Buggy, could use some work)
void fileAdd(Tree* &head){
        char input[100];
        int temp = 0;
        int* arr;
        cout << "Enter a file name: (e.g. filename.txt)" << endl;
        cin >> input;
        //Opens the file and adds the numbers one by one to the tree
	    fstream nums;
        nums.open(input);
        //Does this 2 times
        for(int i = 0; i < 10; i++){
            nums >> temp;
            Tree* newTree = new Tree(temp);
            Tree* current = head;
            Tree* previous = NULL;
            treeAdd(newTree, current, previous, head);
            if(current != head){
                treeBalance(head,current);
	    }
	    }
}
//https://algorithmtutor.com/Data-Structures/Tree/Red-Black-Trees/
//Print function, with trunk structs
void printTree(Tree* start, Trunk* previous, bool isLeft){
if(start == NULL){
        return;
    }
    //This creates space between levels of the tree
    char* oldStr = (char*)("   ");

    Trunk* trunk = new Trunk(previous, oldStr);
    printTree(start->getLeft(), trunk, true); 
    //Formatting
    if(!previous){
        //Diffrentiating for head
        trunk->str = (char*)("---"); 
    }
    else if(isLeft == true){
        //If it's a left child
        trunk->str = (char*)(".---");
        oldStr = (char*)("  |");     
    }
    else{
        //If it's a right child
        trunk->str = (char*)("'---");
        previous->str = oldStr;
    }
    showTrunks(trunk);
    //If black
    if(start->getColor() == 0){
        cout << BLUE << start->getData() << RESET << endl;
    }
    //If red
    else{
        cout << RED << start->getData() << RESET << endl;
    }
    if(previous){
        previous->str = oldStr;
    }
    trunk->str = (char*)("   |");
    //GO right
    printTree(start->getRight(), trunk, false);
}

//Search for a number (Also helps to prevent duplicates)
bool search(Tree* head, int input){
  //If the tree is empty
    if(head == NULL){
        cout << "Number isn't the tree." << endl;
        return false;
    }
    else if(head->getData() == input){
      cout << "Your number is present in the tree!" << endl;
      return true;
    }
    //If the data is larger than head, try going right
    else if(head->getData() > input){
       search(head->getRight(),input);
    }
    //If the data is smaller than head, go left
    else if(head->getData() < input){
      search(head->getLeft(), input);
    }
    return false;
}


//Left rotatation
void leftRotate(Tree* & head, Tree* & target){
    //Rotate from the right, and move it down to the left
    Tree* rightPtr = target->getRight();
    target->setRight(rightPtr->getLeft());
    if(target->getRight() != NULL){
        (target->getRight()->setParent(target));
    }
    rightPtr->setParent(target->getParent());
    //If you're rotating head
    if(target->getParent() == NULL){
        head = rightPtr;
    }
    //If the target is the parent's left child
    else if(target == (target->getParent()->getLeft())){
        target->getParent()->setLeft(rightPtr);
    }
    //iF the target is the parent's right child
    else{
        target->getParent()->setRight(rightPtr);
    }
    rightPtr->setLeft(target);
    target->setParent(rightPtr);
}   
//Rotatate down to the right
void rightRotate(Tree* & head, Tree* & target){
    //Start on the left, rotate to the right and down
    Tree* leftPtr = target->getLeft();
    target->setLeft(leftPtr ->getRight());
    if(target->getLeft() != NULL){
        (target->getLeft()->setParent(target));
    }
    leftPtr->setParent(target->getParent());
    //If you're rotating head
    if(target->getParent() == NULL){
        head = leftPtr;
    }
    else if(target == (target->getParent()->getLeft())){
        target->getParent()->setLeft(leftPtr);
    }
    else{
        target->getParent()->setRight(leftPtr);
    }
    leftPtr->setRight(target);
    target->setParent(leftPtr);
} 
//Balances the Red Black Tree
void treeBalance(Tree* &head, Tree* &current){
    Tree* parent = NULL;
    Tree* grandParent = NULL;
    //While current isn't root, not black, and parent is red.
    while((current != head) && (current->getColor() != 0) && (current->getParent()->getColor() == 1)){
        parent = current->getParent();
        grandParent = parent->getParent();
        //Case A (Where the parent is the left child of the grandparent)
        if(parent == grandParent->getLeft()){
            //The uncle must be the Grandparent's other child
            Tree* uncle = grandParent->getRight();
            //Case 1A: Red Uncle, only recolor
            if(uncle != NULL && uncle->getColor() != 0){
                grandParent->setColor(1); //Red
                parent->setColor(0); //Black
                uncle->setColor(0); //Black
                current = grandParent;
            }
            else{
                //Case 2A: Current = right child of parent (Left Rotate)
                if(current == parent->getRight()){
                    leftRotate(head, parent);
                    current = parent;
                    parent = current->getParent();
                }
                //Case 3A: Current is the left child of parent (Right Rotate)
                rightRotate(head, grandParent);
                //swap the parent and grandparent's colors
                int color = parent->getColor();
                parent->setColor(grandParent->getColor());
                grandParent->setColor(color);
                current = parent;
            }
        }
        //Case B: Parent is the right child of grandparent
        else{
            Tree* uncle = grandParent->getLeft();
            //Case 1B: Uncle is red, and recolor
            if(uncle != NULL && uncle->getColor() != 0){
                grandParent->setColor(1); //Red
                parent->setColor(0); //Black
                uncle->setColor(0); //Black
                current = grandParent;
            }
            //Case 2B: Where the current is the left child of the parent (Right Rotate)
            else{
                if(current == parent->getLeft()){
                    rightRotate(head, parent);
                    current = parent;
                    parent = current->getParent();
                }
                //Case 3B: Where the current is the right child of the parent (Left Rotate)
                leftRotate(head, grandParent);
                //Swap the color of the parent and the grandParent
                int color = parent->getColor();
                parent->setColor(grandParent->getColor());
                grandParent->setColor(color);
                current = parent;
            }
        }
    }
    //Reset head to black
    head->setColor(0);    
}

void treeRemove(Tree* current, Tree* &head, Tree* parent, int data){
    //If there's nothing in the tree to delete
    if(head == NULL){
      cout << "Nothing in the tree to delete!" << endl;
    }
    //Otherwise
    if(current != NULL){
        //If the current node's data is equal to the input
        if(current->getData() == data){
            //If the current has no children
            if(current->getLeft() == NULL && current->getRight() == NULL){
                //If current is head (the only node in the tree)
                if(current == head){
                    delete head;
                    head = NULL;
                }
                //If the current is the left child
                else if(parent->getLeft() == current){
                    parent->setLeft(NULL);
                    delete current;
                }
                //Otherwise if the current is the right child
                else{
                    parent->setRight(NULL);
                    delete current;
                }
            }
            //Only right child present
            else if(current->getLeft() == NULL){
                //If current is head (the only node in the tree)
                if(current == head){
                    Tree* temp = head;
                    head = head->getRight();
                    delete temp;
                }
                //If the current is the parent's right child
                else if(parent->getRight() == current){
                    Tree* temp = current;
                    parent->setRight(current->getRight());
                    delete temp;
                }
                //If the current is the parent's left child
                else{
                    Tree* temp = current;
                    parent->setLeft(current->getRight());
                    delete temp;
                }
            }
            //If only the left child is present
            else if(current->getRight() == NULL){
                if(current == head){
                    Tree* temp = head;
                    head = head->getLeft();
                    delete temp;
                }
                //If the current is the parent's right child
                else if(parent->getRight() == current){
                    Tree* temp = current;
                    parent->setRight(current->getLeft());
                    delete temp;
                }
                //If the current is the parent's left child
                else{
                    Tree* temp = current;
                    parent->setLeft(current->getLeft());
                    delete temp;
                }
            }
            //If both children are present
            else{
                //Replace it with the next smallest node, so go left once and right as far as you can
                //Go left
                Tree* temp2 = current->getLeft();
                Tree* temp3 = current;
                //Go right as far as possible
                while(temp2->getRight() != NULL){
                    temp3 = temp2;
                    temp2 = temp2->getRight();
                }
                //Swap data
                int newCurrent = temp2->getData();
                int deleteData = current->getData();
                temp2->setValue(deleteData);
                current->setValue(newCurrent);
                //If the node's left and right are NULL
                if(temp2->getLeft() == NULL && temp2->getRight() == NULL){
                    //If the node is it's parent's left
                    if(temp3->getLeft() == temp2){
                        temp3->setLeft(NULL);
                    }
                    //If the node is it's parent's right                    
                    else{
                        temp3->setRight(NULL);
                    }
                    //Delete the node
                    Tree* temp4 = temp2;
                    delete temp4;
                }
                //If the right is NULL (Only left exists)
                else if(temp2->getRight() == NULL){
                    Tree* temp4 = temp2;
                    temp3->setLeft(temp2->getLeft());
                    delete temp4;
                    
                }
                //Only the left exists (Right is NULL)
                else{
                    Tree* temp4 = temp2;
                    temp3->setRight(temp2->getRight());
                    delete temp4;
                }
            }
        }
        //If the current's data is larger than the inputted data, go left
        else if(current->getData() > data){
            parent = current;
            treeRemove(current->getLeft(),head, parent, data);
        }
        //Otherwise go right
        else{
            parent = current;
            treeRemove(current->getRight(),head, parent, data);
        }
    }
}
