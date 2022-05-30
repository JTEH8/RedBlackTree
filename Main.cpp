#include <iostream>
#include <cstring>
#include <fstream>
#include "Tree.h"

/*
Author: Jeffrey Teh
Date Completed: 5/29/2022
This is an implementation of a Red Black Tree (With deletion this time)
Help for this project came from Nathan Zou and Jayden Huang

*Some functions have been moved to class file of Tree

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
void treeRemove(Tree* &head, Tree* & deleted);
void treeBalance(Tree* &head, Tree* &current);
void RecolorDoubleBlack(Tree* &head, Tree* &changed);
void swapValues(Tree* &a, Tree* &b);

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
        int del = 0;
        cin >> del;
        //Remove the number from the tree, if it's there
        bool checkTree = search(tree, del);
        if(checkTree == true){
            //Go through the tree and delete.
            Tree* temp = tree;
            while(temp->getData() != del){
                if(del > temp->getData()){
                    temp = temp->getRight();
                }
                if(del < temp->getData()){
                    temp = temp->getLeft();
                }
            }
            treeRemove(tree, temp);
            cout << input3 << " Deleted from the tree" << endl;
        }
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
        int searched = 0;
        cin >> searched;
        search(tree, searched);
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
        current = current->getRight();
        //And if the current Tree's right is NULL
        if(current == NULL){
            current = newTree;
            previous->setRight(current);
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
        current = current->getLeft();
        if(current == NULL){
            current = newTree;
            previous->setLeft(current);
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

/*
void printTree(Tree* start, Trunk* previous, bool isLeft){
    if(start == NULL){
        return;
    }
    if(t )
}
*/
//https://algorithmtutor.com/Data-Structures/Tree/Red-Black-Trees/
//Print function, with trunk structs
void printTree(Tree* start, Trunk* previous, bool isRight){
if(start == NULL){
        return;
    }
    //This creates space between levels of the tree
    char* oldStr = (char*)("   ");

    Trunk* trunk = new Trunk(previous, oldStr);
    printTree(start->getRight(), trunk, true); 
    //Formatting
    if(!previous){
        //Diffrentiating for head
        trunk->str = (char*)("---"); 
    }
    else if(isRight == true){
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
    printTree(start->getLeft(), trunk, false);
}

//Search for a number 
bool search(Tree* head, int input){
  //If the tree is empty
    if(head == NULL){
        cout << "Number isn't the tree." << endl;
        return false;
    }
    if(head->getData() == input){
      cout << "Your number is present in the tree!" << endl;
      /*
        if(head->getRight() != NULL){
        cout << "Here is head's right child: (If applicable)" << head->getRight()->getData() << endl;
        }
        if(head->getLeft() != NULL){
        cout << "Here is head's left child: (If applicable)" << head->getLeft()->getData() << endl;
        }
        */
      return true; 
    }
    //If the data is larger than head, try going right
    else if(head->getData() < input){
       search(head->getRight(),input);
    }
    //If the data is smaller than head, go left
    else if(head->getData() > input){
      search(head->getLeft(), input);
    }
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
    head->setColor(0)   ;    
}
void treeRemove(Tree* &head, Tree* &deleted){
    Tree* replacement = deleted->getReplacement();
    Tree* parent = deleted->getParent();
    //If both are black (Use a boolean to store)
     bool bothBlack = false;
    if(((replacement == NULL) || (replacement->getColor())) && ((deleted == NULL || deleted->getColor() == 0))){
        bothBlack = true;
    }
    //No children
    if(replacement == NULL){
        if(deleted == head){
            head == NULL;
        }
        else{
            if(bothBlack == true){
                RecolorDoubleBlack(head, deleted);
            }
            else{
                //If one is red, make the siblings red
                if(deleted->getSibling(deleted) != NULL){
                    deleted->getSibling(deleted)->setColor(1);
                }
            }
            //Delete it from the tree
            //If it's the left child
            if(deleted == parent->getLeft()){
                parent->setLeft(NULL);
            }
            //If it's the right child
            else{
                parent->setRight(NULL);
            }
        }
        deleted->~Tree();
        return;
    }
    if(deleted->getRight() == NULL || deleted->getLeft() == NULL){
        if(deleted == head){
            //Value goes from replacement to deleted node.
            deleted->setValue(replacement->getData());
            deleted->setLeft(NULL);
            deleted->setRight(NULL);
            replacement->~Tree();
        }
        else{
            //Get the deleted from the tree and move the replacement up
            if(deleted == parent->getLeft()){
                parent->setLeft(replacement);
            }
            else{
                parent->setRight(replacement);
            }
        deleted->~Tree();
        replacement->setParent(parent);
        if(bothBlack == true){
            RecolorDoubleBlack(head, deleted);
        }
        else{
        //If one of them is red, set the color to be Black
            replacement->setColor(0);
                }
            }
        return;
        }
        swapValues(replacement, deleted);
        treeRemove(head, replacement);
    }
//Swaps values between the two nodes.
void swapValues(Tree* &a, Tree* &b){
    int temp = a->getData();
    a->setValue(b->getData());
    b->setValue(temp);
}

void RecolorDoubleBlack(Tree* &head, Tree* & changed){
    if(changed == head){
        return;
    }
    Tree* sibling = changed->getSibling(changed);
    Tree* parent = changed->getParent();
    //If no sibling, push the double black up one.
    if(sibling == NULL){
        RecolorDoubleBlack(head, parent);
    }
    else{
        //Red sibling
        if(sibling->getColor() == 1){
        parent->setColor(1);
        sibling->setColor(0);
        if(sibling == parent->getLeft()){
            rightRotate(head,parent);
        }
        else{
            leftRotate(head, parent);
        }
        RecolorDoubleBlack(head, changed);
        }
        //Sibling is black
        else{
            //If sibling has red child
            if((sibling->getLeft() != NULL && sibling->getLeft()->getColor() == 1) || (sibling->getRight() && sibling->getRight()->getColor() == 1)){
                //Left child is red 
                if(sibling->getLeft() != NULL && sibling->getLeft()->getColor() == 1){
                    if(sibling == parent->getLeft()){
                        //Sibling's left is red and sibling is parent's left
                        sibling->getLeft()->setColor(sibling->getColor());
                        sibling->setColor(parent->getColor());
                        rightRotate(head, parent);
                    }
                    else{
                        //Sibling's left is red and sibling is parent's right
                        sibling->getLeft()->setColor(parent->getColor());
                        rightRotate(head, sibling);
                        leftRotate(head, parent);
                    }
                }
                //Right Child is red
                else{
                    if(sibling == parent->getLeft()){
                        //Sibling's right is red and sibling is parent's left
                        sibling->getRight()->setColor(parent->getColor());
                        leftRotate(head, sibling);
                        rightRotate(head, parent);
                    }
                    else{
                        //Sibling's right is red and sibling is parent's right
                        sibling->getLeft()->setColor(sibling->getColor());
                        sibling->setColor(parent->getColor());
                        leftRotate(head, parent);
                    }
                }
            //Set parent to black
            parent->setColor(0);
            }
            //both children are black
            else{
                sibling->setColor(1);
                //If parent is black
                if(parent->getColor() == 0){
                    RecolorDoubleBlack(head, parent);
                }
                else{
                    //Set parent to black
                    parent->setColor(0);
                }
            }
        }
    }
}
