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
//Function Prototypes
bool search(Tree* head, int input);
void printTree(Tree* head, int space);
void treeAdd(Tree* newNode, Tree* current, Tree* & head);
void rbtADD(Tree* newNode, Tree* current, Tree* & head);
void leftRotate(Node* &head, Node* target);
void rightRotate(Node* &head, Node* target);
void treeRemove(Tree* current, Tree* &head, Tree* parent, int data);

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
cout << "Welcome to the Binary Search Tree! You can add numbers to the tree (ADD), print the tree (PRINT), delete numbers from the tree (DELETE), and search numbers in the tree (SEARCH). To quit type QUIT." << endl; 
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
        //Prevents duplicates
	    if(search(tree, input3) == true){
	      cout << "No duplicate numbers in the tree! Please enter another number." << endl;
	    }
	    else{
	    Tree* newNode = new Tree(input3);
	    treeAdd(newNode, tree, tree);
	    }
	    }
        else if(strcmp(input2, "FILE") == 0){
        int temp = 0;
        cout << "Enter a file name: (e.g. filename.txt)" << endl;
        cin >> input4;
        //Opens the file and adds the numbers one by one to the tree
	    fstream nums;
        nums.open(input4);
        //Does this 10 times
        for(int i = 0; i < 10; i++){
            nums >> temp;
            //Adds the node to the tree
            Tree* newNode = new Tree(temp);
            treeAdd(newNode, tree, tree);
	        }
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
        //Print the tree with spacing 3
        printTree(tree, spacing);
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
void treeAdd(Tree* newNode, Tree* current, Tree* & head){
    //If the tree is empty, make the new one head
    if(head == NULL){
        head = newNode;
        return;
    }
    //Otherwise if the new node's data is bigger
    else if(newNode->getData() > current->getData()){ 
        //And if the current node's right is NULL
        if(current->getRight() == NULL){
            //Add it to the right
            current->setRight(newNode);
            return;
            }
        //Otherwise go onto the right side of the tree
        else{
            treeAdd(newNode, current->getRight(), head);
            }
        }
    //If the node's data is less than the current node's
    else if(newNode->getData() < current->getData()){
        //IF the left space is empty
        if(current->getLeft() == NULL){
            //Add it to the left
            current->setLeft(newNode);
            return;
            }
        //Otherwise go down the left side
        else{
            treeAdd(newNode, current->getLeft(), head);
        }
    }
    //Otherwise (Used for duplicates)
    else {
        //If the left is empty
        if(current->getLeft() == NULL){
            //Add it there
            current->setLeft(newNode);
            return;
        }
        //Otherwise
        else if(current->getRight() == NULL){
            //Add it to the right
            current->setRight(newNode);
            return;
        }
        return;
    }
}
//https://algorithmtutor.com/Data-Structures/Tree/Red-Black-Trees/
void rbtAdd(Tree* newNode, Tree* current, Tree* & head){
    treeAdd(newNode, current, head);
    while(newNode->getParent()->getColor() == 1){
        if(newNode->getParent() == newNode->getParent()->getRight()){
            Tree* uncle = newNode->getParent()->getParent()->getLeft();
            if(uncle->getColor() == 1){ //Parent is red and Uncle is red too.
                uncle->setColor(0);
                newNode->getParent()->setColor(0);
                newNode->getParent()->getParent()->setColor(1);
                newNode = newNode->getParent()->getParent();
            }
            /*
            Parent is right child of Grandparent and NewNode is right child of Parent

            or

            Parent is right child of Grandparent and NewNode is left child of Parent
            */
            else if(newNode = newNode->getParent()->getLeft()){ 
                newNode = newNode->getParent();
                leftRotate(newNode, current, head);
                newNode->getParent()->setColor(0);
                newNode->getParent()->getParent()->setColor(1);
                rightRotate(newNode->getParent()->getParent(), current, head);
            }
        }
        else{
            head->setColor(0);  
        }
    }
}

//Print function from heap
void printTree(Tree* head, int space){
if(head == NULL){
        return;
    }
    //This creates space between levels of the tree
    space = space + 5;

    //Start from the right
    if(head->getRight() != NULL){
    printTree(head->getRight(), space);
    }
    //Print the current node after a new line
    cout << endl;
    for(int i = 0; i < space; i++){
        cout << " ";
    }
    cout << head->getData() << '\n';
    //Onto the left
    cout << endl;
    if(head->getLeft() != NULL){
    printTree(head->getLeft(), space);
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