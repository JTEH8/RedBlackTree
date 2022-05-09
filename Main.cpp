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
//void treeRemove(Tree* current, Tree* &head, Tree* parent, int data);
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
cout << "Welcome to the Red Black Tree! You can add numbers to the tree (ADD) and print the tree (PRINT). To quit type QUIT." << endl; 
cout << "As an aside, make sure you're not entering duplicate numbers (Ones you've already entered or have added via file). Valid entries include anything in the range from 1-999." << endl;    
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
        /*Prevents duplicates
	    if(search(tree, input3) == true){
	      cout << "No duplicate numbers in the tree! Please enter another number." << endl;
	    }
	    else{
            */
	    Tree* newTree = new Tree(input3);
        Tree* current = tree;
        Tree* previous = NULL;
	    treeAdd(newTree, current, previous, tree);
        if(current != tree){
            treeBalance(tree,current);
        }
        cout << endl << input3 << " added to tree." << endl << endl;
	    //}
	    }
        else if(strcmp(input2, "FILE") == 0){
        fileAdd(tree);
        }
    }
    //If the user wants to delete
    /*else if(strcmp(input, "DELETE") == 0){
        cout << "Enter the number (In the tree) that you want to delete." << endl;
        cin >> input3;
        //Remove the number from the tree, if it's there
        treeRemove(tree, tree, tree, input3);
    } 
    */
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
      cout << "Thanks for using BST!" << endl;
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
        //Does this 10 times
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


/*

This isn't necessary for RBT at the moment (Just a holdover from BST)

Search for a number (Also helps to prevent duplicates)
bool search(Tree* head, int input){
    //If the tree is empty
    if(head == NULL){
        cout << "Number isn't the tree. It's thus valid to add." << endl;
        return false;
    }
    //If it isn't the head, and head exists
    else if(head->getData() != input && head != NULL){
        //If the input is smaller than the head, go down the left
        if(head->getData() > input){
            search(head->getLeft(), input);
            }
        //Otherwise if the input is larger than the head, go down the right
        else if(head->getData() < input){
            search(head->getRight(), input);
            }
        }
    //If the head has the searched for number
    else if(head->getData() == input){
        cout << "Your number is present in the tree!" << endl;
        return true;
    }
    else{
        //Otherwise if you get to the end (If it's not caught by the top), then the number isn't there
        cout << "Number isn't in the tree. It's thus valid to add." << endl;
        return false;
    }
    return false;
}
*/
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
    while(current != head && current->getColor() != 0 && current->getParent()->getColor() == 1){
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