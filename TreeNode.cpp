#include "TreeNode.h"
using namespace std;

vector<int> FindNumber(vector<vector<int>> puzzle, int NumberToFind);
int DistanceToGoal(vector<vector<int>> puzzle, vector<vector<int>> goal, int num);

TreeNode::TreeNode(){
    gN = 0;
    hN = 0;
    fN = 0;
    depth = 0;
    puzzleNode = {{1,2,3},{4,5,6},{7,8,0}};
    parent = NULL;
}

TreeNode::TreeNode(vector<vector<int>> puzzle){
    puzzleNode = puzzle;
    gN = 0;
    hN = 0;
    fN = 0;
    depth = 0;
    parent = NULL;
}

void TreeNode::display(){
    int i, j;
    vector<vector<int>> puzzle = puzzleNode;
    for(i = 0; i < puzzle.size(); ++i){
        vector<int> row = puzzle.at(i);
        cout << '[';
        for(j = 0; j < row.size()-1; ++j){
            cout << row.at(j) << ", ";
        }
        cout << row.at(j) << ']' << endl;
    }
    cout << endl;
}

double TreeNode::getUniqueKey(){
    double lock = 1;
    double key = 0;

    for(int i = 0; i < puzzleNode.size(); ++i){
        vector<int> row = puzzleNode.at(i);
        for(int j = 0; j < row.size(); ++j){
            key += abs(sqrt(row.at(j)) * lock);
            lock *= sqrt(key/2);
        }
    }
    return key;
}

void TreeNode::setFn(){
    fN = gN + hN;
}

void TreeNode::setHnMisplaced(vector<vector<int>> goal){
    int amount = 0;
    for(int i = 0; i < puzzleNode.size(); ++i){
        for(int j = 0; j < puzzleNode.at(i).size(); ++j){
            if(puzzleNode.at(i).at(j) != goal.at(i).at(j)){
                if(puzzleNode.at(i).at(j) != 0){
                    ++amount;
                }
            }
        }
    }
    hN = amount;
}

void TreeNode::setHnDistance(vector<vector<int>> goal){
    int sum = 0;

    for(int i = 0; i < 8; ++i){
        sum += DistanceToGoal(puzzleNode, goal, i+1);
    }
    cout << endl;
    hN = sum;
}

TreeNode* TreeNode::move_up(){
    TreeNode* up = new TreeNode(puzzleNode);
    up->parent = this;
    up->depth = this->depth + 1;
    up->gN = this->gN + 1;
    int row, column, fRow, fColumn;
    vector<int> rowAndColumn = FindNumber(puzzleNode, 0);

    row = rowAndColumn.at(0); column = rowAndColumn.at(1);
    fRow = row-1; fColumn = column;

    if(fRow < 0){delete up; return NULL;}
    swap(up->puzzleNode.at(row).at(column), up->puzzleNode.at(fRow).at(fColumn));
    return up;
}
TreeNode* TreeNode::move_down(){
    TreeNode* down = new TreeNode(puzzleNode);
    down->parent = this;
    down->depth = this->depth + 1;
    down->gN = this->gN + 1;
    int row, column, fRow, fColumn;
    vector<int> rowAndColumn = FindNumber(puzzleNode, 0);

    row = rowAndColumn.at(0); column = rowAndColumn.at(1);
    fRow = row+1; fColumn = column;

    if(fRow > 2){delete down; return NULL;} 
    swap(down->puzzleNode.at(row).at(column), down->puzzleNode.at(fRow).at(fColumn));
    return down;
}
TreeNode* TreeNode::move_left(){
    TreeNode* left = new TreeNode(puzzleNode);
    left->parent = this;
    left->depth = this->depth + 1;
    left->gN = this->gN + 1;
    int row, column, fRow, fColumn;
    vector<int> rowAndColumn = FindNumber(puzzleNode, 0);

    row = rowAndColumn.at(0); column = rowAndColumn.at(1);
    fRow = row; fColumn = column-1;

    if(fColumn < 0){delete left; return NULL;} 
    swap(left->puzzleNode.at(row).at(column), left->puzzleNode.at(fRow).at(fColumn));
    return left;
}
TreeNode* TreeNode::move_right(){
    TreeNode* right = new TreeNode(puzzleNode);
    right->parent = this;
    right->depth = this->depth + 1;
    right->gN = this->gN + 1;
    int row, column, fRow, fColumn;
    vector<int> rowAndColumn = FindNumber(puzzleNode, 0);

    row = rowAndColumn.at(0); column = rowAndColumn.at(1);
    fRow = row; fColumn = column+1;

    if(fColumn > 2){delete right; return NULL;}
    swap(right->puzzleNode.at(row).at(column), right->puzzleNode.at(fRow).at(fColumn));
    return right;
}

vector<TreeNode*> TreeNode::CreateChildren(){
    vector<TreeNode*> children;
    TreeNode* temp;
    temp = move_up();
    if(temp != NULL){children.push_back(temp);}
    temp = move_down();
    if(temp != NULL){children.push_back(temp);}
    temp = move_left();
    if(temp != NULL){children.push_back(temp);}
    temp = move_right();
    if(temp != NULL){children.push_back(temp);}
    
    return children;
}

vector<int> FindNumber(vector<vector<int>> puzzle, int NumberToFind){
    vector<int> RowAndColumn = {-1,-1};
    for(int i = 0; i < puzzle.size(); ++i){
        for(int j = 0; j < puzzle.at(i).size(); ++j){
            if (puzzle.at(i).at(j) == NumberToFind){
                RowAndColumn.at(0) = i; RowAndColumn.at(1) = j;
                break;
            }
        }
    }
    return RowAndColumn;
}

int DistanceToGoal(vector<vector<int>> puzzle, vector<vector<int>> goal, int num){
    vector<int> tempP, tempG;
    int distance = 0;

    tempP = FindNumber(puzzle, num);
    tempG = FindNumber(goal, num);

    if(tempP.at(0) != tempG.at(0)){
        distance += abs(tempG.at(0) - tempP.at(0));
    }
    if(tempP.at(1) != tempG.at(1)){
        distance += abs(tempG.at(1) - tempP.at(1));
    }

    return distance;
}