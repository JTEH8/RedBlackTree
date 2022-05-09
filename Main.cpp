#include <iostream>
#include <cstring>
#include <fstream>
#include "Tree.h"

/*
Author: Jeffrey Teh
Date Completed: 4/21/2022
This is an implementation of a Binary Search Tree via Right and Left child nodes.
Help for this project came from Jayden Huang
*/

//Colors
#define RESET "\033[0m"
#define RED   "\033[31m"
#define BLUE  "\033[34m"

using namespace std;

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
void PARSE(char* input, int* modify, int& count);
void printTree(Tree* start, Trunk* previous, bool isLeft);
void treeAdd(Tree* newNode, Tree* & current, Tree* & previous ,Tree* & head);
void rbtADD(Tree* newNode, Tree* current, Tree* & head);
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
cout << "Welcome to the Red Black Tree! You can add numbers to the tree (ADD) and print the tree (PRINT). To quit type QUIT." << endl; 
cout << "As an aside, make sure you're not entering duplicate numbers (Ones you've already entered or have added via file). Valid entries include anything in the range from 1-999." << endl;    
//Read in input
cin >> input;
//When adding numbers to the BST
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
	    Tree* newNode = new Tree(input3);
        Tree* current = tree;
        Tree* previous = NULL;
	    treeAdd(newNode, current, previous, tree);
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
    else if(strcmp(input, "DELETE") == 0){
        cout << "Enter the number (In the tree) that you want to delete." << endl;
        cin >> input3;
        //Remove the number from the tree, if it's there
        treeRemove(tree, tree, tree, input3);
    } 
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
    //Quit BST
    else if(strcmp(input, "QUIT") == 0){
      cout << "Thanks for using BST!" << endl;
      running = false;
    }
    }
}
//Add a number to the tree
void treeAdd(Tree* newNode, Tree*& current, Tree*& previous, Tree* & head){
    //If the tree is empty, make the new one head
    if(head == NULL){
        head = newNode;
        current = head;
        head->setColor(0); //Head is black
    }
    //Otherwise if the new node's data is bigger
    else if(newNode->getData() > current->getData()){ 
        previous = current;
        current = current->getLeft();
        //And if the current node's right is NULL
        if(current == NULL){
            current = newNode;
            previous->setLeft(current);
            current->setParent(previous);
            treeBalance(head, current);
            return;
            }
        //Otherwise go onto the right side of the tree
        else{
            treeAdd(newNode, current, previous, head);
            }
        }
        else{
        previous = current;
        current = current->getRight();
        if(current == NULL){
            current = newNode;
            previous->setRight(current);
            current->setParent(previous);
            treeBalance(head,current);
            return;
        }
        else{
          treeAdd(newNode, current, previous, head);
            }
        }
    }

void fileAdd(Tree* &head){
        char input[100];
        char input2[100];
        int temp = 0;
        int mods[100];
        int modSize = 0;
        int count = 0;
        cout << "Enter a file name: (e.g. filename.txt)" << endl;
        cin >> input;
        streampos size;
        ifstream file(input, ios::in | ios::binary| ios::ate);
        if(file.is_open() == true){
            size = file.tellg();
            file.seekg(0, ios::beg);
            file.read(input, size);
            file.close();
            PARSE(input, mods, count);
            cout << "Input: ";
            for(int i = 0; i < 100; i++){
                if(mods[i] != 0){
                    cout << mods[i] << " ";
                    modSize++;
                }
            }
            cout << endl << "Size: " << modSize << endl;
            Tree* current = NULL;
            Tree* previous = NULL;
            for(int i = 0; i <modSize; i++){
                if(mods[i] == 0){
                    break;
                }
                current = head;
                Tree* newNode = new Tree(mods[i]);
                treeAdd(newNode, current, previous, head);
            }
        }
	}

//https://algorithmtutor.com/Data-Structures/Tree/Red-Black-Trees/

//Print function from heap
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
        trunk->str = (char*)("---"); 
    }
    else if(isLeft == true){
        trunk->str = (char*)(".---");
        oldStr = (char*)("  |");     
    }
    else{
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
    printTree(start->getRight(), trunk, false);
}
//Parses input
void PARSE(char* input, int* modify, int &count){
    int x = 0; //# of chars before each space
    for(int i = 0; i < strlen(input); i ++){
        if(input[i] == ' '){
            if(x == 1){
                int temp = 0;
                temp = input[i-1] - '0';
                modify[count] = temp;
                count ++;
                x = 0;
            }
            else{
                int temp = 0;
                for(int j = 0; j < x; j++){
                    temp = 10*temp + (input[i-x+j] - '0');
                }
                modify[count] = temp;
                count++;
            }
        }
    }
}

//Search for a number (Also helps to prevent duplicates)
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
void leftRotate(Tree* & head, Tree* & target){
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
    else if(target == (target->getParent()->getLeft())){
        target->getParent()->setLeft(rightPtr);
    }
    else{
        target->getParent()->setRight(rightPtr);
    }
    rightPtr->setLeft(target);
    target->setParent(rightPtr);
}   

void rightRotate(Tree* & head, Tree* & target){
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
        //Case B: PArent is the right child of grandparent
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