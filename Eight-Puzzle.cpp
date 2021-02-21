// Eight-Puzzle.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include <cmath>

using namespace std;
static int goal[3][3] = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 0 } };

struct state { // state(node) structure
    int parameter[3][3] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
    int g = 0;
    int h = 0;
    int max = 1;
};

vector<state> expand(vector<state> nodes, state node) {

    vector<state> expandedQueue = nodes;
    int row, col;
    for (int i = 0; i < 3; i++) { //find where the empty tile is
        for (int j = 0; j < 3; j++) {
            if (node.parameter[i][j] == 0) {
                row = i;
                col = j;
            }
        }
    }
    //move left
    if (col != 0) {
        state tempNode = node;
        int temp = tempNode.parameter[row][col];
        tempNode.parameter[row][col] = tempNode.parameter[row][col - 1];
        tempNode.parameter[row][col - 1] = temp;
        tempNode.g += 1;
        expandedQueue.push_back(tempNode);
    }
    //move right
    if (col < 2) {
        state tempNode2 = node;
        int temp2 = tempNode2.parameter[row][col];
        tempNode2.parameter[row][col] = tempNode2.parameter[row][col + 1];
        tempNode2.parameter[row][col + 1] = temp2;
        tempNode2.g += 1;
        expandedQueue.push_back(tempNode2);
    }
    //move up
    if (row != 0) {
        state tempNode3 = node;
        int temp3 = tempNode3.parameter[row][col];
        tempNode3.parameter[row][col] = tempNode3.parameter[row - 1][col];
        tempNode3.parameter[row - 1][col] = temp3;
        tempNode3.g += 1;
        expandedQueue.push_back(tempNode3);
    }
    //move down
    if (row < 2) {
        state tempNode4 = node;
        int temp4 = tempNode4.parameter[row][col];
        tempNode4.parameter[row][col] = tempNode4.parameter[row + 1][col];
        tempNode4.parameter[row + 1][col] = temp4;
        tempNode4.g += 1;
        expandedQueue.push_back(tempNode4);
    }
    return expandedQueue; //return queue with new expanded nodes
}

vector<state> uniform(vector<state> nodes, state node) { // Uniform Cost Search h(n) = 0
    state nextExpand = node;
    vector<state> uniformNodes = expand(nodes, node);

    if (nodes.size() > node.max) {
        uniformNodes.front().max = nodes.size();
    }

    return uniformNodes;

}
vector<state> misplaced(vector<state> nodes, state node) { // Misplaced Tile Heuristic
    vector<state> misplacedNodes = nodes;
    int lowest = node.g + node.h;
    int lowestIndex = 0;
    int temp;
    for (unsigned int x = 0; x < misplacedNodes.size(); x++) {
        temp = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (misplacedNodes.front().parameter[i][j] != goal[i][j]) { // if the numbers of the problem do not match goal increase h by 1
                    temp += 1;
                }
            }
        }
        misplacedNodes.at(x).h = temp;
    }
    for (unsigned int y = 0; y < misplacedNodes.size(); y++) {
        if (misplacedNodes.at(y).g + misplacedNodes.at(y).h < lowest) { // find the node with the lowest h(n) + g(n) value
            lowest = misplacedNodes.at(y).g + misplacedNodes.at(y).h;
            lowestIndex = y;
        }
    }
    state nextExpand = misplacedNodes.at(lowestIndex);
    misplacedNodes = expand(nodes, nextExpand);

    if (nodes.size() > node.max) {
        nextExpand.max = nodes.size();
    }


    return misplacedNodes;
}

vector<state> manhattan(vector<state> nodes, state node) {
    vector<state> manhattanNodes = nodes;
    int lowest = node.g + node.h;
    int lowestIndex = 0;

    int temp;
    for (unsigned int x = 0; x < manhattanNodes.size(); x++) {
        temp = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) { //find h by finding distance between each goal tile and problem tile
                if (manhattanNodes.at(x).parameter[i][j] == 1) {
                    temp += abs(0 - i) + abs(0 - j);
                }
                if (manhattanNodes.at(x).parameter[i][j] == 2) {
                    temp += abs(0 - i) + abs(1 - j);
                }
                if (manhattanNodes.at(x).parameter[i][j] == 3) {
                    temp += abs(0 - i) + abs(2 - j);
                }
                if (manhattanNodes.at(x).parameter[i][j] == 4) {
                    temp += abs(1 - i) + abs(0 - j);
                }
                if (manhattanNodes.at(x).parameter[i][j] == 5) {
                    temp += abs(1 - i) + abs(1 - j);
                }
                if (manhattanNodes.at(x).parameter[i][j] == 6) {
                    temp += abs(1 - i) + abs(2 - j);
                }
                if (manhattanNodes.at(x).parameter[i][j] == 7) {
                    temp += abs(2 - i) + abs(0 - j);
                }
                if (manhattanNodes.at(x).parameter[i][j] == 8) {
                    temp += abs(2 - i) + abs(2 - j);
                }
            }
        }
        manhattanNodes.at(x).h = temp;
    }
    for (unsigned int y = 0; y < manhattanNodes.size(); y++) { // find the node with the lowest h(n) + g(n) value
        if (manhattanNodes.at(y).g + manhattanNodes.at(y).h < lowest) {
            lowest = manhattanNodes.at(y).g + manhattanNodes.at(y).h;
            lowestIndex = y;
        }
    }

    state nextExpand = manhattanNodes.at(lowestIndex);
    manhattanNodes = expand(nodes, nextExpand);
    if (nodes.size() > node.max) {
        nextExpand.max = nodes.size();
    }


    return manhattanNodes;

}

state searchAlgorithm(state problem, int function) {
    vector<state> nodes;
    state node;
    int isSolved = true;
    nodes.push_back(problem);
    nodes = expand(nodes, nodes.front());
    int x = 0;
    while (1) {
        node = nodes.front();
        isSolved = true;
        for (int i = 0; i < 3; i++) { // check if the goal state has been reached
            for (int j = 0; j < 3; j++) {
                if (node.parameter[i][j] != goal[i][j]) {
                    isSolved = false;
                 
                }
            }
        }
        cout << "The best state to expand with a g(n) = " << node.g << " and h(n) = " << node.h << " is" << endl;
        cout << "[" << node.parameter[0][0] << ", " << node.parameter[0][1] << ", " << node.parameter[0][2] << "]" << endl;
        cout << "[" << node.parameter[1][0] << ", " << node.parameter[1][1] << ", " << node.parameter[1][2] << "]" << endl;
        cout << "[" << node.parameter[2][0] << ", " << node.parameter[2][1] << ", " << node.parameter[2][2] << "]" << endl;

        if (isSolved) { //goal state reached
            cout << "goal" << endl;
            cout << "Solution depth: " << nodes.front().g << endl;
            cout << "Nodes expanded: " << x << endl;
            cout << "Max queue size: " << nodes.front().max << endl;
            return nodes.front();
            break;
        }

        nodes.erase(nodes.begin()); //pop out front node
       
        if (function == 1) { // Uniform Cost Search
            nodes = uniform(nodes, node);
        }
        else if (function == 2) { //Misplaced Tile Heuristic
            nodes = misplaced(nodes, node);
        }
        else if (function == 3) { // Manhattan Distance Heuristic
            nodes = manhattan(nodes, node);
        }
        x++;
    }

    return node;
}

int main()
{
    int type = 0;
    int num[9];
    state test;

    int function;
 


    cout << "Welcome to Tommy's Eight-Puzzle Solver. Type '1' to use a default puzzle or '2' to create your own." << endl;
    cin >> type;
    if (type == 1) {
        test.parameter[0][0] = 0;
        test.parameter[0][1] = 1;
        test.parameter[0][2] = 2;
        test.parameter[1][0] = 4;
        test.parameter[1][1] = 5;
        test.parameter[1][2] = 3;
        test.parameter[2][0] = 7;
        test.parameter[2][1] = 8;
        test.parameter[2][2] = 6;
    }
    if (type == 2) {
        cout << "Please enter 9 numbers:" << endl;
        for (int i = 0; i < 9; i++) {
            cin >> num[i];
        }
        test.parameter[0][0] = num[0];
        test.parameter[0][1] = num[1];
        test.parameter[0][2] = num[2];
        test.parameter[1][0] = num[3];
        test.parameter[1][1] = num[4];
        test.parameter[1][2] = num[5];
        test.parameter[2][0] = num[6];
        test.parameter[2][1] = num[7];
        test.parameter[2][2] = num[8];
    }
    cout << "Please enter a number for the algorithm: 1 for Uniform Cost Search 2 for Misplaced Tile Heuristic 3 for Manhattan Distance Heuristic." << endl;
    cin >> function;

    test = searchAlgorithm(test, function);




}

