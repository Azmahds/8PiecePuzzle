#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <queue>
#include "TreeNode.h"
#include <map>
using namespace std;

//ALL COMMENTS FOR FUNCTIONS ARE WHERE THE FUNCTION IS WRITTEN
TreeNode* Get_Default_Puzzle();
TreeNode* Create_Puzzle();
void searchAlgorithm(queue<TreeNode*> &, int);
void move_up(vector<vector<int>> &puzzle);
void move_down(vector<vector<int>> &puzzle);
void move_left(vector<vector<int>> &puzzle);
void move_right(vector<vector<int>> &puzzle);
bool isGoalState(TreeNode* puzzle);
void emptyQueue(queue<TreeNode*> &);
queue<TreeNode*> sortQueue(queue<TreeNode*>);


vector<vector<int>> goal_state = {{1,2,3}, {4,5,6}, {7,8,0}}; //Hardcoded Goal State for 8-piece puzzle
vector<vector<int>> original_state; //Keeps track of the Original 8-piece puzzle
int qSize = 0; //size of the Queue
int nodesExpanded = 0; //amount of nodes expanded for the valid moves attempted
map<double, vector<vector<int>>> checked; //The map to log all puzzles so we don't attempt to expand duplicates
int duplicates = 0; //The number of duplicates encountered

int main(){
    
    int puzzleSelection = 0, algorithm;
    TreeNode* puzzle;
    queue<TreeNode*> q;

    cout << "Hello! Welcome to the Puzzle Solver. Please enter '0' to create your own puzzle or enter '1' to use the default puzzle." << endl;
    cin >> puzzleSelection;
    cout << endl;

    if(puzzleSelection == 0){
        puzzle = Create_Puzzle(); //assigns puzzle to created puzzle
    } else{
        puzzle = Get_Default_Puzzle(); //assigns puzzle to a default puzzle
    }

    cout << "Select algorithm. (1) for Uniform Cost Search, (2) for the Misplaced Tile Heuristic, or (3) the Manhattan Distance Heuristic." << endl; 
    cin >> algorithm; //stores the chosen algorithm
    cout << endl;

    q.push(puzzle); //puts the chosen puzzle into the queue

    qSize = q.size(); //initializes Q size
    nodesExpanded = 0; //initializes nodesExpanded
    while(1){ //Continues looping to check all nodes in queue
        if(q.empty()){break;} //If the queue is empty it will break out of the loop
        if(qSize < q.size()){qSize = q.size();} //Assigns qSize if q.size() is ever greater

        TreeNode* node = q.front(); //Takes node from the front of the queue
        checked.insert(pair<double, vector<vector<int>>>(node->getUniqueKey(), node->puzzleNode)); //inserts node into the map
        
        if(isGoalState(node)){ //Check for if the initial state IS the goal state
            break;
        }
        if(algorithm == 1){         //Uniform Cost Search
            searchAlgorithm(q, 0);
        } else if(algorithm == 2){  //A* with the Misplaced Tile heuristic
            searchAlgorithm(q, 1);
        } else{                     //A* with the Manhattan Distance heuristic
            searchAlgorithm(q, 2);
        }
    }

    return 0;
}

void searchAlgorithm(queue<TreeNode*> &q, int heruistic){
    TreeNode* parent = q.front();  //Takes the Node out of the queue
    q.pop(); //removes the node from the queue

    cout << "Expanding Node with G(n) = " << parent->gN << " and H(n) = " << parent->hN << endl;
    parent->display();

    vector<TreeNode*> children = parent->CreateChildren(); //creates ALL VALID children of the parent and keeps them in a vector

    if(heruistic == 1){ //For Misplaced Algorithm
        for(int i = 0; i < children.size(); ++i){
            children.at(i)->setHnMisplaced(goal_state); //sets children h(n) according to the alorithm
        }
    } 
    else if(heruistic == 2){ //For Distance Algorithm
        for(int i = 0; i < children.size(); ++i){
            children.at(i)->setHnDistance(goal_state); //sets children h(n) according to the alorithm
        }
    }

    cout << "Child(ren): " << endl;
    for(int i = 0; i < children.size(); ++i){ //iterates through all the children
        if(checked.find(children.at(i)->getUniqueKey()) != checked.end()){ //checks if child has already been explored
            children.erase(children.begin() + i);
            --i;
            ++duplicates;
            continue;
        }
        ++nodesExpanded; //increases the valid nodes expanded by 1
        if(isGoalState(children.at(i))){ //checks if the child if the goal state
            emptyQueue(q); //if it is the goal state emptys the queue to break out of main while loop
            return;
        }
        children.at(i)->display();

        if(qSize < q.size()){qSize = q.size();} //reassigns qSize if q.size() is greater
    }

    for(int i = 0; i < children.size(); ++i){
        children.at(i)->setFn(); //sets all childrens f(n)
        q.push(children.at(i)); //pushs children into the queue
    }

    q = sortQueue(q); //makes sure queue is sorted by f(n)

    return;
}




//Create default puzzle
TreeNode* Get_Default_Puzzle(){
    TreeNode* puzzle = new TreeNode(); //Initializes puzzle to Trivial
    int level;
    
    cout << "Choose difficulty. (0) Trivial, (1) Very Easy, (2) Easy, (3) Doable, (4) Oh boy" << endl;
    cin >> level;

    if(level == 1){
        puzzle->puzzleNode = {{1,2,3}, {4,5,6}, {7,0,8}}; //sets puzzle to Very Easy
    } else if(level == 2){
        puzzle->puzzleNode = {{1,2,0}, {4,5,3}, {7,8,6}}; //sets puzzle to Easy
    } else if(level == 3){
        puzzle->puzzleNode = {{0,1,2}, {4,5,3}, {7,8,6}}; //sets puzzle to Doable
    } else if(level == 4){
        puzzle->puzzleNode = {{8,7,1}, {6,0,2}, {5,4,3}}; //sets puzzle to Oh boy
    }

    original_state = puzzle->puzzleNode; //sets original state to chosen puzzle
    return puzzle;
}

//Create new puzzle
TreeNode* Create_Puzzle(){
    vector<int> r1, r2, r3; //rows for puzzle
    int input, i = 0;

    // cout << "Enter your puzzle, using a zero to represent the blank. Please only enter valid 8-puzzles." << endl;
    // cout << "Enter the puzzle with spaces between each number. Type RETURN only when finished." << endl << endl;

    // cout << "Enter the First row: ";
    // while(i < 3){ //inputs values of the first row to r1
    //     cin >> input;
    //     r1.push_back(input);
    //     ++i;
    // }
    // cout << "Enter the Second row: ";
    // i = 0;
    // while(i < 3){ //inputs values of the second row to r2
    //     cin >> input;
    //     r2.push_back(input);
    //     ++i;
    // }
    // cout << "Enter the Third row: ";
    // i = 0;
    // while(i < 3){ //inputs values of the third row to r3
    //     cin >> input;
    //     r3.push_back(input);
    //     ++i;
    // }
    // cout << endl;
    cout << "choose 0 2 4 8 12 or 16";
    cin >> input;
    vector<vector<int>> test;
    if(input == 0){
        test = {{1,2,3}, {4,5,6},{7,8,0}};
    }
    else if(input == 2){
        test = {{1,2,3}, {4,5,6},{0,7,8}};
    }
    else if(input == 4){
        test = {{1,2,3}, {5,0,6},{4,7,8}};
    }
    else if(input == 8){
        test = {{1,3,6}, {5,0,2},{4,7,8}};
    }
    else if(input == 12){
        test = {{1,3,6}, {5,0,7},{4,8,2}};
    }
    else if(input == 16){
        test = {{1,6,7}, {5,0,3},{4,8,2}};
    }

    TreeNode* puzzle = new TreeNode(test); //creates a new puzzle with the inputted rows
    return puzzle;
}

//Returns True if puzzle is the Goal State
bool isGoalState(TreeNode* puzzle){
    if(puzzle->puzzleNode == goal_state){ //checks if puzzle is goal state
        cout << endl << "GOAL STATE FOUND!" << endl;
        cout  << "Solution Depth was " << puzzle->depth << endl //outputs the puzzle depth
              << "Number of nodes expanded (valid moves attempted): " << nodesExpanded << endl //outputs the amount of nodes expanded
              << "Max queue size at any moment: " << qSize << endl //outputs the max queue size.
              << "Duplicates encountered: " << duplicates << endl; //outputs the amount of duplicates encountered
        return true;
    }
    return false;
}

//Empties the Queue
void emptyQueue(queue<TreeNode*> &q){
    while(!q.empty()){//loops until queue is empty
        if(qSize < q.size()){ //final check reassign qSize if q.size() is greater
            qSize = q.size();
        } 
        q.pop(); //pops a node from the queue
    }
}

//Sorts the queue by the node f(n)
queue<TreeNode*> sortQueue(queue<TreeNode*> q){
    vector<TreeNode*> tempQ;
    TreeNode* temp;
    vector<double> allFn;

    if(q.empty() || q.size() < 2){return q;}

    while(!q.empty()){
        temp = q.front();
        tempQ.push_back(temp);
        allFn.push_back(temp->fN);
        q.pop();
    }
    for(int i = 0; i < allFn.size(); ++i){
       for(int j = i+1; j < allFn.size(); ++j){
           if(allFn.at(i) > allFn.at(j)){
               swap(allFn.at(i), allFn.at(j));
               swap(tempQ.at(i), tempQ.at(j));
           }
       }
    }

    for(int i = 0; i < tempQ.size(); ++i){
        q.push(tempQ.at(i));
    } cout << endl;
    return q;
}