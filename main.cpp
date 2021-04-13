#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>

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
};

int River::numObjects() {
	int BOATGANG = 0;
	if (hasBoat() == 1) {
		BOATGANG = 1;
	}
	int total = getNumWolves() + getNumChickens() + BOATGANG;
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
void move(GameState initialState, GameState* successorState, int numChickens, int numWolves);
Node* depthFirstSearch(GameState& state, GameState& goalState, int& nodesExpanded);
Node* breadthFirstSearch(GameState& state, GameState& goalState, int& nodesExpanded);

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
		goalPath = breadthFirstSearch(state, goalState, nodesExpanded);
		std::cout << "Write code for Breadth-First Search" << std::endl;
	}
	else if (mode.compare("dfs") == 0) {
		std::cout << "Entering DFS" << std::endl;
		goalPath = depthFirstSearch(state, goalState, nodesExpanded);
		std::cout << "Depth first search completed." << std::endl;
	}
	else if (mode.compare("iddfs") == 0) {
		std::cout << "Write code for Iterative Deepening Depth-First Search" << std::endl;
	}
	else if (mode.compare("astar") == 0) {
		std::cout << "Write code for A* Search" << std::endl;
	}
	else { //bad case
		std::cout << "No valid mode entered. Program only supports bfs, dfs, iddfs and astar graph algorithms" << std::endl;
		return 1;
	}
	if (goalPath == NULL) {
		outputFOUT << "No solution found after expanding nodes\n" << std::endl;
	}
	else {
		std::cout << "Reached file output" << std::endl;
		outputFOUT << "Nodes expanded: " << nodesExpanded << std::endl;
		outputFOUT << "Goal state found after searching to depth: " << goalPath->depth << " with the following path." << std::endl;
		outputFOUT << "Format: Initial state is at the bottom and the goal state is at the top" << std::endl;
		while (goalPath != NULL) {
			//because i feel nice this is split into multiple lines :)
			outputFOUT << goalPath->currentState.leftBank.getNumChickens() << ","
				<< goalPath->currentState.leftBank.getNumWolves() << ","
				<< goalPath->currentState.leftBank.hasBoat() << '\n'
				<< goalPath->currentState.rightBank.getNumChickens() << ","
				<< goalPath->currentState.rightBank.getNumWolves() << ","
				<< goalPath->currentState.rightBank.hasBoat() << "\n" << std::endl;
			goalPath = goalPath->previous;
			std::cout << "one state written to path" << std::endl;
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

Node* depthFirstSearch(GameState& state, GameState& goalState, int& nodesExpanded) {
	
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
		std::cout << "Chickens on left, right bank: " << initialState->currentState.leftBank.getNumChickens() << "," << initialState->currentState.rightBank.getNumChickens() << std::endl;
		std::cout << "Wolves on left, right bank: " << initialState->currentState.leftBank.getNumWolves() << "," << initialState->currentState.rightBank.getNumWolves() << std::endl;
		if (initialState->currentState == goalState) { //compare the two objects using our overloaded operator to compare two GameStates.
			std::cout << "Goal state found" << std::endl;
			return initialState;
		}
		nodesExpanded++; //increment the number of nodes we expanded upon if we have failed to find the goal state.
		GameState successorState0;
		move(initialState->currentState, &successorState0, 1, 1);
		if (checkExplored(exploredSet, successorState0) == 0 && validSuccessor(&successorState0)) {
			temp = new Node();
			temp->currentState = successorState0;
			temp->previous = initialState;
			temp->depth = initialState->depth + 1;
			frontier.push(temp);
			exploredSet.push_back(temp);
			std::cout << "state 0 pushed into frontier" << std::endl;
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
			std::cout << "state 1 pushed into frontier" << std::endl;
		}
		GameState successorState2;
		move(initialState->currentState, &successorState2, 0, 2);
		if (checkExplored(exploredSet, successorState2) == 0 && validSuccessor(&successorState2)) {
			temp = new Node();
			temp->currentState = successorState2;
			temp->previous = initialState;
			temp->depth = initialState->depth + 1;
			frontier.push(temp);
			exploredSet.push_back(temp);
			std::cout << "state 2 pushed into frontier" << std::endl;
		}
		GameState successorState3;
		move(initialState->currentState, &successorState3, 1, 0);
		if (checkExplored(exploredSet, successorState3) == 0 && validSuccessor(&successorState3)) {
			temp = new Node();
			temp->currentState = successorState3;
			temp->previous = initialState;
			temp->depth = initialState->depth + 1;
			frontier.push(temp);
			exploredSet.push_back(temp);
			std::cout << "state 3 pushed into frontier" << std::endl;
		}
		GameState successorState4;
		move(initialState->currentState, &successorState4, 0, 1);
		if (checkExplored(exploredSet, successorState4) == 0 && validSuccessor(&successorState4)) {
			temp = new Node();
			temp->currentState = successorState4;
			temp->previous = initialState;
			temp->depth = initialState->depth + 1;
			frontier.push(temp);
			exploredSet.push_back(temp);
			std::cout << "state 4 pushed into frontier" << std::endl;
		}
	}
	return NULL;
		
}

Node* breadthFirstSearch(GameState& state, GameState& goalState, int& nodesExpanded) {
	std::queue<Node*> frontier;
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
		std::cout << "Chickens on left, right bank: " << initialState->currentState.leftBank.getNumChickens() << "," << initialState->currentState.rightBank.getNumChickens() << std::endl;
		std::cout << "Wolves on left, right bank: " << initialState->currentState.leftBank.getNumWolves() << "," << initialState->currentState.rightBank.getNumWolves() << std::endl;
		if (initialState->currentState == goalState) { //compare the two objects using our overloaded operator to compare two GameStates.
			std::cout << "Goal state found" << std::endl;
			return initialState;
		}

		nodesExpanded++; //increment the number of nodes we expanded upon if we have failed to find the goal state.
		GameState successorState0;
		move(initialState->currentState, &successorState0, 1, 1);
		if (checkExplored(exploredSet, successorState0) == 0 && validSuccessor(&successorState0)) {
			temp = new Node();
			temp->currentState = successorState0;
			temp->previous = initialState;
			temp->depth = initialState->depth + 1;
			frontier.push(temp);
			exploredSet.push_back(temp);
			std::cout << "state 0 pushed into frontier" << std::endl;
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
			std::cout << "state 1 pushed into frontier" << std::endl;
		}
		GameState successorState2;
		move(initialState->currentState, &successorState2, 0, 2);
		if (checkExplored(exploredSet, successorState2) == 0 && validSuccessor(&successorState2)) {
			temp = new Node();
			temp->currentState = successorState2;
			temp->previous = initialState;
			temp->depth = initialState->depth + 1;
			frontier.push(temp);
			exploredSet.push_back(temp);
			std::cout << "state 2 pushed into frontier" << std::endl;
		}
		GameState successorState3;
		move(initialState->currentState, &successorState3, 1, 0);
		if (checkExplored(exploredSet, successorState3) == 0 && validSuccessor(&successorState3)) {
			temp = new Node();
			temp->currentState = successorState3;
			temp->previous = initialState;
			temp->depth = initialState->depth + 1;
			frontier.push(temp);
			exploredSet.push_back(temp);
			std::cout << "state 3 pushed into frontier" << std::endl;
		}
		GameState successorState4;
		move(initialState->currentState, &successorState4, 0, 1);
		if (checkExplored(exploredSet, successorState4) == 0 && validSuccessor(&successorState4)) {
			temp = new Node();
			temp->currentState = successorState4;
			temp->previous = initialState;
			temp->depth = initialState->depth + 1;
			frontier.push(temp);
			exploredSet.push_back(temp);
			std::cout << "state 4 pushed into frontier" << std::endl;
		}
	}
	return NULL;
}

bool checkExplored(std::vector<Node*> exploredSet, GameState successorState) {
	std::vector<Node*>::iterator temp;
	for (temp = exploredSet.begin(); temp != exploredSet.end(); temp++) { //iterate through explored set
		if ((*temp)->currentState == successorState) { //if there is a game state in the explored set equal to the successor's game state
			return true; //return true
		}
	}
	return false; //else return false
}


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




bool validSuccessor(GameState* successorState) {
	if ((successorState->leftBank.getNumChickens() >= successorState->leftBank.getNumWolves()) || (successorState->leftBank.getNumChickens() == 0)) {
		if ((successorState->rightBank.getNumChickens() >= successorState->rightBank.getNumWolves()) || (successorState->rightBank.getNumChickens() == 0)) {
			return true;
		}
	}
}

