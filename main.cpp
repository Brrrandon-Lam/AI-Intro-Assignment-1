#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>

/****************************************************************************************************************************
* Program: Intro to AI Assignment 1: Uninformed and Informed Search
* Programmers: Brandon Lam and Aedan Mills
* Description: Performs uninformed and informed search algorithms after receiving valid input for chicken & wolves game.
*			   Input includes the initial state, the goal state, the type of algorithm to use and the output file.
*			   The program will print the number of nodes expanded, the depth of the goal state found in the algorithm,
*			   and the path from the initial state to the goal state into the output file. See the output file for
*			   formatting.
*****************************************************************************************************************************/

//TEMPORARY GAME STRUCTURE
/* TXT FILE STRUCTURE: 
*  first line left: numChickens, numWolves, boat
*  second line right: numChickens, numWolves, boat
*/

class River {
public:
	int numObjects(); //return total number of objects on a river side
	int getNumWolves(); // return num wolves
	int getNumChickens(); //return num chickens
	bool hasBoat(); //return 1 or 0 for boat being present
	void setNumWolves(int wolfgang); //set num wolves.
	void setNumChickens(int chickengang); //set num chickens
	void setBoat(bool boatVal); //set boat to 1 or 0
private:
	int numWolves;
	int numChickens;
	bool boat;
};

class GameState {
	public:
		River leftBank;
		River rightBank;
		friend bool operator== (GameState& left, GameState& right);
		//if all of those line up for left and right, then return true
};

//operator overload to compare two GameState objects properly. 
bool operator== (GameState& left, GameState& right) {
	bool b = true;
	
	if (!(left.leftBank.getNumChickens() == right.leftBank.getNumChickens())) {
		b = false;
	}
	if (!(left.leftBank.getNumWolves() == right.leftBank.getNumWolves())) {
		b = false;
	}
	if (!(left.rightBank.getNumChickens() == right.rightBank.getNumChickens())) {
		b = false;
	}
	if (!(left.rightBank.getNumWolves() == right.rightBank.getNumWolves())) {
		b = false;
	}
	if (!(left.rightBank.hasBoat() == right.rightBank.hasBoat())) {
		b = false;
	}
	if (!(left.leftBank.hasBoat() == right.leftBank.hasBoat())) {
		b = false;
	}
	if (b == false) {
		//std::cout << "Objects are not the same" << std::endl;
	}
	else {
		//std::cout << "Objects are the same" << std::endl;
	}
	//if all of those line up for left and right, then return true
	return b;
}


struct Node { //keep track of the parent node, current node and depth.
	GameState currentState;
	Node* previous;
	int depth;
	int priority;
};

//used for calculating a given state's heuristic. gets the total number of animals on the respective side of the bank
int River::numObjects() {
	int BOATGANG = 0;
	if (hasBoat() == 1) {
		BOATGANG = 1;
	}
	int total = getNumWolves() + getNumChickens() + BOATGANG;
	return total;
}

// MUTATORS
void River::setNumWolves(int wolfgang) {
	numWolves = wolfgang;
}

void River::setNumChickens(int chickengang) {
	numChickens = chickengang;
}

void River::setBoat(bool boatVal) {
	boat = boatVal;
}

// ACCESSORS
int River::getNumChickens() {
	return numChickens;
}

int River::getNumWolves() {
	return numWolves;
}

bool River::hasBoat() {
	return boat;
}


//Function Declarations
bool checkExplored(std::vector<Node*> exploredSet, GameState successorState);
void readFromFiles(GameState&, GameState&, std::ifstream&, std::ifstream&);
bool validSuccessor(GameState* successorState);
void refreshStates(std::vector<Node*> exploredSet);
void move(GameState initialState, GameState* successorState, int numChickens, int numWolves);
Node* depthFirstSearch(GameState& state, GameState& goalState, int& nodesExpanded);
Node* breadthFirstSearch(GameState& state, GameState& goalState, int& nodesExpanded);
Node* iddfs(GameState& state, GameState& goalState, int& nodesExpanded);
Node* astar(GameState& state, GameState& goalState, int& nodesExpanded);
void heuristicFunc(Node* initialState, GameState& goalState);

int main(int argc, char** argv) {
	if (argc != 5) { //check for correct number of arguments
		//arguments should be executable initial.txt goal.txt mode output.txt 
		std::cout << "Invalid number of arguments. argc != 5" << std::endl;
		return 1;
	}
	//store the passed arguments in string variables to be used for later.
	std::string initialStateFile = argv[1]; 
	std::string goalStateFile = argv[2];
	std::string mode = argv[3];
	std::string outputFile = argv[4];
	//create the necessary game states
	class GameState state;
	class GameState goalState;

	//open the initial and goal state files for reading with filein, and open the output file with fileout.
	std::ofstream outputFOUT;
	std::ifstream initialFIN(initialStateFile.c_str());
	std::ifstream goalFIN(goalStateFile.c_str());
	outputFOUT.open(outputFile.c_str());

	//check that the initial state file descriptor and goal state file descriptors are open. Additionally, verify that the output file is open.
	if (!(initialFIN.is_open() && goalFIN.is_open()) && outputFOUT.is_open()) {
		std::cout << "Invalid file name provided. Double check initial state file or goal state file." << std::endl;
		return 1;
	}

	//fill initial and goal states with information from text files (see header)
	readFromFiles(state, goalState, initialFIN, goalFIN);

	//close files after we are done with them
	initialFIN.close();
	goalFIN.close();
	Node* goalPath;
	int nodesExpanded = 0;
	//Determine which graph algorithm to run by mode using the compare() function
	if (mode.compare("bfs") == 0) {
		std::cout << "Entering BFS" << std::endl;
		goalPath = breadthFirstSearch(state, goalState, nodesExpanded);
		std::cout << "breadth first search completed." << std::endl;
	}
	else if (mode.compare("dfs") == 0) {
		std::cout << "Entering DFS" << std::endl;
		goalPath = depthFirstSearch(state, goalState, nodesExpanded);
		std::cout << "Depth first search completed." << std::endl;
	}
	else if (mode.compare("iddfs") == 0) {
		std::cout << "Entering Iterative Deepening Depth-First Search" << std::endl;
		goalPath = iddfs(state, goalState, nodesExpanded);
		std::cout << "ID Depth first search completed." << std::endl;
	}
	else if (mode.compare("astar") == 0) {
		std::cout << "Entering astar search" << std::endl;
		goalPath = astar(state, goalState, nodesExpanded);
		std::cout << "A* search completed." << std::endl;
	}
	else { //bad case
		std::cout << "No valid mode entered. Program only supports bfs, dfs, iddfs and astar graph algorithms" << std::endl;
		return 1;
	}
	if (goalPath == NULL) {
		outputFOUT << "No solution found after expanding " << nodesExpanded << " nodes\n" << std::endl;
	}
	else {
		std::cout << "Writing contents to: " << outputFile << std::endl;
		outputFOUT << "Nodes expanded: " << nodesExpanded << std::endl;
		outputFOUT << "Goal state found after searching to depth: " << goalPath->depth << " with the following path." << std::endl;
		outputFOUT << "Format: Initial state is at the bottom and the goal state is at the top\n" << std::endl;

		//passes in the nodesExpanded as well as the depth the goal was found at
		while (goalPath != NULL) {
			//all the elements that explain the current state of the game
			outputFOUT << goalPath->currentState.leftBank.getNumChickens() << ","
				<< goalPath->currentState.leftBank.getNumWolves() << ","
				<< goalPath->currentState.leftBank.hasBoat() << '\n'
				<< goalPath->currentState.rightBank.getNumChickens() << ","
				<< goalPath->currentState.rightBank.getNumWolves() << ","
				<< goalPath->currentState.rightBank.hasBoat() << "\n" << std::endl;
			goalPath = goalPath->previous;
			//std::cout << "one state written to path" << std::endl;
		}
	}


	outputFOUT.close();
	return 0;
}

/*******************************************************************************************************************
* Function Name: readFromFiles
* Description: Reads shit from the initial and goal.txt files and stores them in our four objects using the infiles. 
* Parameters: 4 objects and two ifstream variables
* Output: nothing
******************************************************************************************************************/


void readFromFiles(GameState& state, GameState& goalState, std::ifstream& initialFIN, std::ifstream& goalFIN) {
	int chickens = 0;
	int wolves = 0;
	bool bote;
	char temp69420;
	char temp;
	//get contents from first line of the initial.txt
	initialFIN >> chickens >> temp69420 >> wolves >> temp >> bote;
	//store them into the object (leftBank)
	state.leftBank.setNumChickens(chickens);
	state.leftBank.setNumWolves(wolves);
	state.leftBank.setBoat(bote);

	/*
	std::cout << "Left Bank Initial " << chickens << " " << wolves << " " << bote << std::endl;
	std::cout << "Left Bank Initial " << leftBank.getNumChickens() << " " << leftBank.getNumWolves() << " " << leftBank.hasBoat() << std::endl;
	*/

	//get contents from second line of initial.txt
	initialFIN >> chickens >> temp >> wolves >> temp69420 >> bote;
	//store in rightBank object
	state.rightBank.setNumChickens(chickens);
	state.rightBank.setNumWolves(wolves);
	state.rightBank.setBoat(bote);
	
	//std::cout << "Right Bank Initial " << chickens << " " << wolves << " " << bote << std::endl;

	//get contents from first line of goal.txt
	goalFIN >> chickens >> temp >> wolves >> temp69420 >> bote;
	//store them in leftGoal object
	goalState.leftBank.setNumChickens(chickens);
	goalState.leftBank.setNumWolves(wolves);
	goalState.leftBank.setBoat(bote);
	//get contents from second line of goal.txt
	goalFIN >> chickens >> temp69420 >> wolves >> temp >> bote;
	//store them in rightGoal object
	goalState.rightBank.setNumChickens(chickens);
	goalState.rightBank.setNumWolves(wolves);
	goalState.rightBank.setBoat(bote);
	//Our 4 objects now have all the data from the files so we are done

}

/* Depth First Search 
*  Return solution or failure
* Initialize the frontier using the initial state of the problem
*  Initialize the explored set to be empty
*  Loop:
*	If the frontier is empty, return failure
*   Choose a leaf node, remove it from the frontier
*   If the node contains a goal state, return corresponding solution
*   Add the node to the explored set
*   Expand chosen node, add the resulting node to the frontier
*      Only if not in the forntier or explored set
* 
*   Implement using a LIFO queue (Stack) <-This makes the generic graph search a depth first search
*/

/*******************************************************************************************************************
* Function Name: dfs
* Description: runs through dfs version of the chicken and wolves problem, for additional comments: see astar
* Parameters: GameState& state, GameState& goalState, int& nodesExpanded
* Output: path of nodes leading to goal or no solution.
******************************************************************************************************************/

Node* depthFirstSearch(GameState& state, GameState& goalState, int& nodesExpanded) {
	nodesExpanded = 0;
	std::stack<Node*> frontier; //the frontier. It is a stack of nodes that we will expand upon in LIFO order.
	std::vector<Node*> exploredSet; //contains the set of nodes we've expanded (the explored set).
	Node* initialState = new Node();
	initialState->currentState = state; //set the initial state to the initial state from the file
	initialState->previous = NULL; //there is no state before the initial state.
	initialState->depth = 0;
	frontier.push(initialState);
	exploredSet.push_back(initialState);

	Node* temp;
	while (!(frontier.empty())) { //while there are still nodes in the frontier
		initialState = frontier.top(); //store the last value in the frontier in the initial state
		frontier.pop(); //pop the last value from the frontier off (pop it off of the stack)
		//std::cout << "Chickens on left, right bank: " << initialState->currentState.leftBank.getNumChickens() << "," << initialState->currentState.rightBank.getNumChickens() << std::endl;
		//std::cout << "Wolves on left, right bank: " << initialState->currentState.leftBank.getNumWolves() << "," << initialState->currentState.rightBank.getNumWolves() << std::endl;
		//nodesExpanded++;
		if (initialState->currentState == goalState) { //compare the two objects using our overloaded operator to compare two GameStates.
			std::cout << "Goal state found" << std::endl;
			return initialState;
		}
		nodesExpanded++; //increment the number of nodes we expanded upon if we have failed to find the goal state.
		GameState successorState0;
		move(initialState->currentState, &successorState0, 1, 0);
		if (checkExplored(exploredSet, successorState0) == 0 && validSuccessor(&successorState0)) {
			temp = new Node();
			temp->currentState = successorState0;
			temp->previous = initialState;
			temp->depth = initialState->depth + 1;
			frontier.push(temp);
			exploredSet.push_back(temp);
			//std::cout << "state 3 pushed into frontier" << std::endl;
		}
		GameState successorState1;
		move(initialState->currentState, &successorState1, 2, 0);
		if (checkExplored(exploredSet, successorState1) == 0 && validSuccessor(&successorState1)) {
			temp = new Node();
			temp->currentState = successorState1;
			temp->previous = initialState;
			temp->depth = initialState->depth + 1;
			frontier.push(temp);
			exploredSet.push_back(temp);
			//std::cout << "state 1 pushed into frontier" << std::endl;
		}
		GameState successorState2;
		move(initialState->currentState, &successorState2, 0, 1);
		if (checkExplored(exploredSet, successorState2) == 0 && validSuccessor(&successorState2)) {
			temp = new Node();
			temp->currentState = successorState2;
			temp->previous = initialState;
			temp->depth = initialState->depth + 1;
			frontier.push(temp);
			exploredSet.push_back(temp);
			//std::cout << "state 4 pushed into frontier" << std::endl;
		}
		GameState successorState3;
		move(initialState->currentState, &successorState3, 1, 1);
		if (checkExplored(exploredSet, successorState3) == 0 && validSuccessor(&successorState3)) {
			temp = new Node();
			temp->currentState = successorState3;
			temp->previous = initialState;
			temp->depth = initialState->depth + 1;
			frontier.push(temp);
			exploredSet.push_back(temp);
			//std::cout << "state 0 pushed into frontier" << std::endl;
		}
		GameState successorState4;
		move(initialState->currentState, &successorState4, 0, 2);
		if (checkExplored(exploredSet, successorState4) == 0 && validSuccessor(&successorState4)) {
			temp = new Node();
			temp->currentState = successorState4;
			temp->previous = initialState;
			temp->depth = initialState->depth + 1;
			frontier.push(temp);
			exploredSet.push_back(temp);
			//std::cout << "state 2 pushed into frontier" << std::endl;
		}
	}
	return NULL;
		
}

/*******************************************************************************************************************
* Function Name: bfs
* Description: runs through bfs version of the chicken and wolves problem, for additional comments: see astar
* Parameters: GameState& state, GameState& goalState, int& nodesExpanded
* Output: path of nodes leading to goal or no solution.
******************************************************************************************************************/

Node* breadthFirstSearch(GameState& state, GameState& goalState, int& nodesExpanded) {
	nodesExpanded = 0;
	std::queue<Node*> frontier; //normal queue needed for bfs, uses a FIFO ordering to pick nodes for expansion
	std::vector<Node*> exploredSet;
	Node* initialState = new Node();
	initialState->currentState = state;
	initialState->previous = NULL;
	frontier.push(initialState);
	exploredSet.push_back(initialState);

	Node* temp;
	while (!(frontier.empty())) {
		initialState = frontier.front(); //store the value at the front of the queue into the initial state
		frontier.pop(); //remove the value at the front from the frontier
		//std::cout << "Chickens on left, right bank: " << initialState->currentState.leftBank.getNumChickens() << "," << initialState->currentState.rightBank.getNumChickens() << std::endl;
		//std::cout << "Wolves on left, right bank: " << initialState->currentState.leftBank.getNumWolves() << "," << initialState->currentState.rightBank.getNumWolves() << std::endl;
		//nodesExpanded++;
		if (initialState->currentState == goalState) { //compare the two objects using our overloaded operator to compare two GameStates.
			std::cout << "Goal state found" << std::endl;
			return initialState;
		}

		nodesExpanded++; //increment the number of nodes we expanded upon if we have failed to find the goal state.
		GameState successorState0; //see astar
		move(initialState->currentState, &successorState0, 1, 0);
		if (checkExplored(exploredSet, successorState0) == 0 && validSuccessor(&successorState0)) {
			temp = new Node();
			temp->currentState = successorState0;
			temp->previous = initialState;
			temp->depth = initialState->depth + 1;
			frontier.push(temp);
			exploredSet.push_back(temp);
			//std::cout << "state 3 pushed into frontier" << std::endl;
		}
		GameState successorState1;
		move(initialState->currentState, &successorState1, 2, 0);
		if (checkExplored(exploredSet, successorState1) == 0 && validSuccessor(&successorState1)) {
			temp = new Node();
			temp->currentState = successorState1;
			temp->previous = initialState;
			temp->depth = initialState->depth + 1;
			frontier.push(temp);
			exploredSet.push_back(temp);
			//std::cout << "state 1 pushed into frontier" << std::endl;
		}
		GameState successorState2;
		move(initialState->currentState, &successorState2, 0, 1);
		if (checkExplored(exploredSet, successorState2) == 0 && validSuccessor(&successorState2)) {
			temp = new Node();
			temp->currentState = successorState2;
			temp->previous = initialState;
			temp->depth = initialState->depth + 1;
			frontier.push(temp);
			exploredSet.push_back(temp);
			//std::cout << "state 4 pushed into frontier" << std::endl;
		}
		GameState successorState3;
		move(initialState->currentState, &successorState3, 1, 1);
		if (checkExplored(exploredSet, successorState3) == 0 && validSuccessor(&successorState3)) {
			temp = new Node();
			temp->currentState = successorState3;
			temp->previous = initialState;
			temp->depth = initialState->depth + 1;
			frontier.push(temp);
			exploredSet.push_back(temp);
			//std::cout << "state 0 pushed into frontier" << std::endl;
		}
		GameState successorState4;
		move(initialState->currentState, &successorState4, 0, 2);
		if (checkExplored(exploredSet, successorState4) == 0 && validSuccessor(&successorState4)) {
			temp = new Node();
			temp->currentState = successorState4;
			temp->previous = initialState;
			temp->depth = initialState->depth + 1;
			frontier.push(temp);
			exploredSet.push_back(temp);
			//std::cout << "state 2 pushed into frontier" << std::endl;
		}
	}
	return NULL;
}

/*******************************************************************************************************************
* Function Name: iddfs
* Description: runs through iddfs version of the chicken and wolves problem, for additional comments: see astar
* Parameters: GameState& state, GameState& goalState, int& nodesExpanded
* Output: path of nodes leading to goal or no solution.
******************************************************************************************************************/

Node* iddfs(GameState& state, GameState& goalState, int& nodesExpanded) {
	nodesExpanded = 0;
	int d = 0; //current depth counter that IDDFS uses to check for paths until
	int maxD = 100; //the max depth to check for goal paths
	std::stack<Node*> frontier; //the frontier. It is a stack of nodes that we will expand upon in LIFO order.
	std::vector<Node*> exploredSet; //contains the set of nodes we've expanded (the explored set).

	while (1) {
		Node* initialState = new Node();
		initialState->currentState = state; //set the initial state to the initial state from the file
		initialState->previous = NULL; //there is no state before the initial state.
		initialState->depth = 0;
		frontier.push(initialState);
		exploredSet.push_back(initialState);

		Node* temp;
		while (!(frontier.empty())) { //while there are still nodes in the frontier
			initialState = frontier.top(); //store the last value in the frontier in the initial state
			frontier.pop(); //pop the last value from the frontier off (pop it off of the stack)
			//std::cout << "Chickens on left, right bank: " << initialState->currentState.leftBank.getNumChickens() << "," << initialState->currentState.rightBank.getNumChickens() << std::endl;
			//std::cout << "Wolves on left, right bank: " << initialState->currentState.leftBank.getNumWolves() << "," << initialState->currentState.rightBank.getNumWolves() << std::endl;
			//nodesExpanded++;

			if (initialState->depth < maxD) { //if the node's depth is less than the max depth (chosen by programmer)
				d = initialState->depth; //update the depth iterator for iddfs
			}

			if (initialState->currentState == goalState) { //compare the two objects using our overloaded operator to compare two GameStates.
				std::cout << "Goal state found" << std::endl;
				return initialState;
			}

			if (initialState->depth == maxD) { //about to exceed the maxD
				continue; //skip through the rest of the while loop
			}

			nodesExpanded++; //increment the number of nodes we expanded upon if we have failed to find the goal state.
			GameState successorState0; //see astar
			move(initialState->currentState, &successorState0, 1, 0);
			if (checkExplored(exploredSet, successorState0) == 0 && validSuccessor(&successorState0)) {
				temp = new Node();
				temp->currentState = successorState0;
				temp->previous = initialState;
				temp->depth = initialState->depth + 1;
				frontier.push(temp);
				exploredSet.push_back(temp);
				//std::cout << "state 3 pushed into frontier" << std::endl;
			}
			GameState successorState1;
			move(initialState->currentState, &successorState1, 2, 0);
			if (checkExplored(exploredSet, successorState1) == 0 && validSuccessor(&successorState1)) {
				temp = new Node();
				temp->currentState = successorState1;
				temp->previous = initialState;
				temp->depth = initialState->depth + 1;
				frontier.push(temp);
				exploredSet.push_back(temp);
				//std::cout << "state 1 pushed into frontier" << std::endl;
			}
			GameState successorState2;
			move(initialState->currentState, &successorState2, 0, 1);
			if (checkExplored(exploredSet, successorState2) == 0 && validSuccessor(&successorState2)) {
				temp = new Node();
				temp->currentState = successorState2;
				temp->previous = initialState;
				temp->depth = initialState->depth + 1;
				frontier.push(temp);
				exploredSet.push_back(temp);
				//std::cout << "state 4 pushed into frontier" << std::endl;
			}
			GameState successorState3;
			move(initialState->currentState, &successorState3, 1, 1);
			if (checkExplored(exploredSet, successorState3) == 0 && validSuccessor(&successorState3)) {
				temp = new Node();
				temp->currentState = successorState3;
				temp->previous = initialState;
				temp->depth = initialState->depth + 1;
				frontier.push(temp);
				exploredSet.push_back(temp);
				//std::cout << "state 0 pushed into frontier" << std::endl;
			}
			GameState successorState4;
			move(initialState->currentState, &successorState4, 0, 2);
			if (checkExplored(exploredSet, successorState4) == 0 && validSuccessor(&successorState4)) {
				temp = new Node();
				temp->currentState = successorState4;
				temp->previous = initialState;
				temp->depth = initialState->depth + 1;
				frontier.push(temp);
				exploredSet.push_back(temp);
				//std::cout << "state 2 pushed into frontier" << std::endl;
			}
		}
		d++;

		if (d > maxD) {
			return NULL;
		}

		maxD++;

		d = 0;
		refreshStates(exploredSet); //clear all elements of the explored set
		exploredSet.clear(); //clears all memory within the vector
	}

	return NULL;
}

/*******************************************************************************************************************
* Function Name: astar
* Description: runs through astar version of the chicken and wolves problem
* Parameters: GameState& state, GameState& goalState, int& nodesExpanded
* Output: path of nodes leading to goal or no solution.
******************************************************************************************************************/

Node* astar(GameState& state, GameState& goalState, int& nodesExpanded) {
	nodesExpanded = 0;
	std::priority_queue<Node*> frontier; //the frontier. A priority queue as needed for astar.Determines the ordering of the nodes to be expanded
	std::vector<Node*> exploredSet; //contains the set of nodes we've expanded (the explored set).
	Node* initialState = new Node();
	initialState->currentState = state; //set the initial state to the initial state from the file
	initialState->previous = NULL; //there is no state before the initial state.
	initialState->depth = 0;
	initialState->priority = goalState.leftBank.numObjects() - state.leftBank.numObjects(); //how we will judge the priority value for the queue (should be 0 initially)
	frontier.push(initialState); //push the first node onto the pqueue
	exploredSet.push_back(initialState); //add it to the explored nodes

	Node* temp; //created for changing object values
	while (!(frontier.empty())) { //while there are still nodes in the frontier
		initialState = frontier.top(); //store the last value in the frontier in the initial state
		frontier.pop(); //pop the last value from the frontier off (pop it off of the stack)
		//std::cout << "Chickens on left, right bank: " << initialState->currentState.leftBank.getNumChickens() << "," << initialState->currentState.rightBank.getNumChickens() << std::endl;
		//std::cout << "Wolves on left, right bank: " << initialState->currentState.leftBank.getNumWolves() << "," << initialState->currentState.rightBank.getNumWolves() << std::endl;
		//nodesExpanded++;
		if (initialState->currentState == goalState) { //compare the two objects using our overloaded operator to compare two GameStates.
			//std::cout << "Goal state found" << std::endl;
			return initialState;
		}
		nodesExpanded++; //increment the number of nodes we expanded upon if we have failed to find the goal state.
		GameState successorState0; //create a potential gamestate
		move(initialState->currentState, &successorState0, 1, 0); //does the first move, as shown in the pdf (1 chicken)
		if (checkExplored(exploredSet, successorState0) == 0 && validSuccessor(&successorState0)) { //makes sure the move is valid
			temp = new Node(); //allocate memory
			//store contents
			temp->currentState = successorState0; 
			temp->previous = initialState;
			temp->depth = initialState->depth + 1;

			heuristicFunc(temp, goalState); //check for the heuristic and store that value into priority field
			frontier.push(temp); 
			exploredSet.push_back(temp);
			//std::cout << "state 3 pushed into frontier" << std::endl;
		}
		GameState successorState1; //continues but for the next move
		move(initialState->currentState, &successorState1, 2, 0);
		if (checkExplored(exploredSet, successorState1) == 0 && validSuccessor(&successorState1)) {
			temp = new Node();
			temp->currentState = successorState1;
			temp->previous = initialState;
			temp->depth = initialState->depth + 1;
			heuristicFunc(temp, goalState);
			frontier.push(temp);
			exploredSet.push_back(temp);
			//std::cout << "state 1 pushed into frontier" << std::endl;
		}
		GameState successorState2; //see above
		move(initialState->currentState, &successorState2, 0, 1);
		if (checkExplored(exploredSet, successorState2) == 0 && validSuccessor(&successorState2)) {
			temp = new Node();
			temp->currentState = successorState2;
			temp->previous = initialState;
			temp->depth = initialState->depth + 1;
			heuristicFunc(temp, goalState);
			frontier.push(temp);
			exploredSet.push_back(temp);
			//std::cout << "state 4 pushed into frontier" << std::endl;
		}
		GameState successorState3; //see above
		move(initialState->currentState, &successorState3, 1, 1);
		if (checkExplored(exploredSet, successorState3) == 0 && validSuccessor(&successorState3)) {
			temp = new Node();
			temp->currentState = successorState3;
			temp->previous = initialState;
			temp->depth = initialState->depth + 1;
			heuristicFunc(temp, goalState);
			frontier.push(temp);
			exploredSet.push_back(temp);
			//std::cout << "state 0 pushed into frontier" << std::endl;
		}
		GameState successorState4; //see above
		move(initialState->currentState, &successorState4, 0, 2);
		if (checkExplored(exploredSet, successorState4) == 0 && validSuccessor(&successorState4)) {
			temp = new Node();
			temp->currentState = successorState4;
			temp->previous = initialState;
			temp->depth = initialState->depth + 1;
			heuristicFunc(temp, goalState);
			frontier.push(temp);
			exploredSet.push_back(temp);
			//std::cout << "state 2 pushed into frontier" << std::endl;
		}
	}
	return NULL;
}

/*******************************************************************************************************************
* Function Name: heuristicFunc
* Description: gives a priority value to specific actions/states depending on how close the total number of animals are between the state and goalstate
* Parameters: potential successorNode, the goalState of the problem
* Output: none
******************************************************************************************************************/

void heuristicFunc(Node *successorNode, GameState& goalState) {
	int prio;
	//because prio queue in C++ is a max heap
	prio = (goalState.leftBank.numObjects() - successorNode->currentState.leftBank.numObjects()) * -1; 
	successorNode->priority = prio;
	//std::cout << "Item pushed to queue with with priority value of: " << successorNode->priority << std::endl;
	//depends on how close the state is to the actual goal state, the closer it is to 0, the better the action
}

/*******************************************************************************************************************
* Function Name: refreshStates
* Description: clears the memory from the explored nodes vector
* Parameters: the exploredSet
* Output: none
******************************************************************************************************************/

void refreshStates(std::vector<Node*> exploredSet) {
	std::vector <Node*>::iterator it;
	for (it = exploredSet.begin(); it != exploredSet.end(); it++) {
		delete (*it);
	}
}

/*******************************************************************************************************************
* Function Name: checkExplored
* Description: iterates thru exploredSet of nodes and returns a bool based on if the successor node is explored already
* Parameters: vector of explored NODES, potential successorstate
* Output: bool
******************************************************************************************************************/

bool checkExplored(std::vector<Node*> exploredSet, GameState successorState) {
	std::vector<Node*>::iterator temp;
	for (temp = exploredSet.begin(); temp != exploredSet.end(); temp++) { //iterate through explored set
		if ((*temp)->currentState == successorState) { //if there is a game state in the explored set equal to the successor's game state
			return true; //return true
		}
	}
	return false; //else return false
}

/*******************************************************************************************************************
* Function Name: move
* Description: does the logistics of a move, which is determined by the num of animals passed in
* Parameters: initialstate, potential successorstate, and the number of animals transported
* Output: valid state change
******************************************************************************************************************/

void move(GameState initialState, GameState* successorState, int numChickens, int numWolves) {

	if (initialState.leftBank.hasBoat() == 1) {
		if (initialState.leftBank.getNumChickens() >= numChickens) {
			//take numChickens away from the left bank
			successorState->leftBank.setNumChickens(initialState.leftBank.getNumChickens() - numChickens);
			//add numChickens to the right bank
			successorState->rightBank.setNumChickens(initialState.rightBank.getNumChickens() + numChickens);
		}
		if (initialState.leftBank.getNumWolves() >= numWolves) {
			successorState->leftBank.setNumWolves(initialState.leftBank.getNumWolves() - numWolves);
			successorState->rightBank.setNumWolves(initialState.rightBank.getNumWolves() + numWolves);
		}
		successorState->leftBank.setBoat(0);
		successorState->rightBank.setBoat(1);
	}
	else {
		if (initialState.rightBank.getNumChickens() >= numChickens) {
			successorState->rightBank.setNumChickens(initialState.rightBank.getNumChickens() - numChickens);
			successorState->leftBank.setNumChickens(initialState.leftBank.getNumChickens() + numChickens);
		}
		if (initialState.rightBank.getNumWolves() >= numWolves) {
			successorState->rightBank.setNumWolves(initialState.rightBank.getNumWolves() - numWolves);
			successorState->leftBank.setNumWolves(initialState.leftBank.getNumWolves() + numWolves);
		}
		successorState->rightBank.setBoat(0);
		successorState->leftBank.setBoat(1);
	}
}

/*******************************************************************************************************************
* Function Name: validSuccessor
* Description: makes sure the state is valid for an action
* Parameters: a potential successorstate
* Output: boolean value
******************************************************************************************************************/

bool validSuccessor(GameState* successorState) {
	//num chicks must be greater than or equal to num wolves, and there must be at least one thing to pilot boat back (for both banks)
	if ((successorState->leftBank.getNumChickens() >= successorState->leftBank.getNumWolves()) || (successorState->leftBank.getNumChickens() == 0)) { 
		if ((successorState->rightBank.getNumChickens() >= successorState->rightBank.getNumWolves()) || (successorState->rightBank.getNumChickens() == 0)) {
			return true;
		}
	}
	return false;
}

