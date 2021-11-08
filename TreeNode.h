#ifndef TreeNode_h
#define TreeNode_h

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <math.h>
using namespace std;

class TreeNode {
    public:
        vector<vector<int>> puzzleNode;
        int gN;
        double hN;
        double fN;
        int depth;
        TreeNode* parent;

        TreeNode();
        TreeNode(vector<vector<int>> puzzle);
        void display();
        double getUniqueKey();
        void setFn();
        void setHnMisplaced(vector<vector<int>> goal);
        void setHnDistance(vector<vector<int>> goal);
        TreeNode* move_up();
        TreeNode* move_down();
        TreeNode* move_left();
        TreeNode* move_right();
        vector<TreeNode*> CreateChildren();
};



#endif