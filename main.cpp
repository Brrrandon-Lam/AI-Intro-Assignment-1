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

void readFromFiles(GameState&, GameState&, std::ifstream&, std::ifstream&);
Node* moveOneEach(Node* initialState);
Node* moveTwoChickens(Node* initialState);
Node* moveTwoWolves(Node* initialState);
Node* moveOneChicken(Node* initialState);
Node* moveOneWolf(Node* initialState);
Node* depthFirstSearch(GameState& state, GameState& goalState, int& nodesExpanded);

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
		std::cout << "Write code for Breadth-First Search" << std::endl;
	}
	else if (mode.compare("dfs") == 0) {
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
	//issue with writing to output file
	if (goalPath == NULL) {
		outputFOUT << "No solution found after expanding nodes\n" << std::endl;
	}
	else {
		std::cout << "Reached file output" << std::endl;
		outputFOUT << "Nodes expanded: " << nodesExpanded << std::endl;
		outputFOUT << "Goal state found after searching to depth: " << goalPath->depth << " with the following path." << std::endl;
		outputFOUT << "Format: Initial state is at the bottom and the goal state is at the top?" << std::endl;
		while (goalPath != NULL) {
			//because i feel nice this is split into multiple lines :)
			outputFOUT << goalPath->currentState.leftBank.getNumChickens() << ","
				<< goalPath->currentState.leftBank.getNumWolves() << ","
				<< goalPath->currentState.leftBank.hasBoat() << '\n'
				<< goalPath->currentState.rightBank.getNumChickens() << ","
				<< goalPath->currentState.rightBank.getNumWolves() << ","
				<< goalPath->currentState.rightBank.hasBoat() << std::endl;
			goalPath = goalPath->previous;
			std::cout << "one state written to path" << std::endl;
		}
	}
	//clean up objects

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
	
	//Create our initial node that contains the initial game state.
	Node* initialState = new Node(); //create new node 
	initialState->currentState = state; //store the passed game state (the initial state)
	initialState->previous = NULL; //the initial game state has no previous game state, so set to null.

	//initialize the explored set as empty
	std::vector<Node*> exploredSet; //push_back()
	//create the path to hold nodes that we will expand on (the frontier)
	std::stack<Node*> frontier; //push()
	//push the initial state onto the frontier
	frontier.push(initialState);
	while (!frontier.empty()) {
		//get the last-in value of the frontier.
		initialState = frontier.top();
		//remove the value from the frontier
		frontier.pop();
		std::cout << "Initial State Left Bank: " << initialState->currentState.leftBank.getNumChickens() << "," << initialState->currentState.leftBank.getNumWolves() << "," << initialState->currentState.leftBank.hasBoat() << std::endl;
		std::cout << "Initial State Right Bank: " << initialState->currentState.rightBank.getNumChickens() << "," << initialState->currentState.rightBank.getNumWolves() << "," << initialState->currentState.rightBank.hasBoat() << std::endl;
		//BASE CASE: If the initial state is the goal state, then we have already found the shortest path to the goal and can simply return the initial state.
		//std::cout << "Iteration" << std::endl;
		if (initialState->currentState.leftBank.getNumChickens() == goalState.leftBank.getNumChickens()) {
			std::cout << "Matching left chickens" << std::endl;
			if (initialState->currentState.leftBank.getNumWolves() == goalState.leftBank.getNumWolves()) {
				std::cout << "Matching left wolves" << std::endl;
				if (initialState->currentState.leftBank.hasBoat() == goalState.leftBank.hasBoat()) {
					std::cout << "Matching left boat" << std::endl;
					if (initialState->currentState.rightBank.getNumChickens() == goalState.rightBank.getNumChickens()) {
						std::cout << "Matching right chickens" << std::endl;
						if (initialState->currentState.rightBank.getNumWolves() == goalState.rightBank.getNumWolves()) {
							std::cout << "Matching right wolves" << std::endl;
							if (initialState->currentState.rightBank.hasBoat() == goalState.rightBank.hasBoat()) {
								std::cout << "Matching right boat" << std::endl;
								std::cout << "GOAL FOUND" << std::endl;
								return initialState;
							}
						}
					}
				}
			}
		}

		//NEED TO KEEP TRACK OF EXPANDED NODES (NUMBER) UPDATE BEFORE EXPANSION, BUT AFTER POPPING FROM STACK
		//DONT EXPAND IF IT IS A GOAL NODE

		nodesExpanded++;
		Node* successorState4 = moveOneWolf(initialState);
		Node* successorState3 = moveOneChicken(initialState);
		Node* successorState2 = moveTwoWolves(initialState);
		Node* successorState1 = moveTwoChickens(initialState);
		Node* successorState0 = moveOneEach(initialState);

		std::vector<Node*>::iterator temp;
		//push fifth successor onto the stack
		bool explored = false;
		if ((successorState4 != initialState) && (successorState4 != NULL)) { //if the successor state is not the initial state, and if the successor state exists
			for (temp = exploredSet.begin(); temp != exploredSet.end(); temp++) { //validate the successor state against the explored set
				if ((*temp)->currentState == successorState4->currentState) {
					explored = true;
					//std::cout << "explored 4" << std::endl;
				}
			}
			if (explored == false) { //if it is unexplored, push to frontier
				frontier.push(successorState4);
				exploredSet.push_back(successorState4);
				//std::cout << "push 4" << std::endl;
			}
		}
		//push fourth successor onto stack
		explored = false;
		if ((successorState3 != initialState) && (successorState3 != NULL)) {
			for (temp = exploredSet.begin(); temp != exploredSet.end(); temp++) {
				if ((*temp)->currentState == successorState3->currentState) {
					explored = true;
					//std::cout << "explored 3" << std::endl;
				}
			}
			if (explored == false) {
				frontier.push(successorState3);
				exploredSet.push_back(successorState3);
				//std::cout << "push 3" << std::endl;
			}
		}
		//push third successor onto the stack
		explored = false;
		if ((successorState2 != initialState) && (successorState2 != NULL)) {
			for (temp = exploredSet.begin(); temp != exploredSet.end(); temp++) {
				if ((*temp)->currentState == successorState2->currentState) {
					explored = true;
					//std::cout << "explored 2" << std::endl;
				}
			}
			if (explored == false) {
				frontier.push(successorState2);
				exploredSet.push_back(successorState2);
				//std::cout << "push 2" << std::endl;
			}
		}

		//push second successor onto stack
		explored = false;
		if ((successorState1 != initialState) && (successorState1 != NULL)) {
			for (temp = exploredSet.begin(); temp != exploredSet.end(); temp++) {
				if ((*temp)->currentState == successorState1->currentState) {
					explored = true;
					//std::cout << "explored 1" << std::endl;
				}
			}
			if (explored == false) {
				//std::cout << "push 1" << std::endl;
				frontier.push(successorState1);
				exploredSet.push_back(successorState1);
			}
		}
		//push first successor onto the stack
		explored = false;
		if ((successorState0 != initialState) && (successorState0 != NULL)) {
			for (temp = exploredSet.begin(); temp != exploredSet.end(); temp++) {
				if ((*temp)->currentState == successorState0->currentState) {
					explored = true;
					//std::cout << "already explored 0" << std::endl;
				}
			}
			if (explored == false) {
				//std::cout << "push 0" << std::endl;
				frontier.push(successorState0);
				exploredSet.push_back(successorState0);
			}
		}
		//if a successor state is in the explored state, we do not push it onto the frontier
		//else we push it onto the frontier.
	}

	std::cout << "Frontier empty" << std::endl;

	return 0; //if we exit the while loop with nothing then there was no solution
}

/* At each step we must:
*	Add one wolf and one chicken to the opposite bank
*	Remove one wolf and one chicken from the initial bank
*	Remove the boat from the initial bank
*	Add the boat to the opposite bank
*	Set the resulting state's parent as the initial state
*	Return the resulting state
*/

Node* moveOneEach(Node* initialState) {
	Node* resultingState = new Node();
	resultingState->currentState = initialState->currentState;
	resultingState->depth = initialState->depth + 1;
	if (initialState->currentState.leftBank.hasBoat()) { //boat is on the left bank. We want to move one wolf and one chicken to the right bank
		//if the number of wolves on the left bank is greater than 0, and the number of wolves on the right bank is greater than 1
		if ((initialState->currentState.leftBank.getNumWolves() >= 1) && (initialState->currentState.leftBank.getNumChickens() >= 1)) {
			//if the number of wolves on the right bank + 1 is less than or equal to the number of chickens on the right bank + 1
			if ((initialState->currentState.rightBank.getNumWolves() + 1) <= (initialState->currentState.rightBank.getNumChickens() + 1)) { 
				//Add one wolf and one chicken to the right bank
				resultingState->currentState.rightBank.setNumChickens(initialState->currentState.rightBank.getNumChickens() + 1);
				resultingState->currentState.rightBank.setNumWolves(initialState->currentState.rightBank.getNumWolves() + 1);
				//Remove one wolf and one chicken from the right bank
				resultingState->currentState.leftBank.setNumChickens(initialState->currentState.leftBank.getNumChickens() - 1);
				resultingState->currentState.leftBank.setNumWolves(initialState->currentState.leftBank.getNumWolves() - 1);
				//Remove boat from the left bank
				resultingState->currentState.leftBank.setBoat(0);
				//Add boat to the right bank
				resultingState->currentState.rightBank.setBoat(1);
				//set the resulting state's parent as the initial sate.
				resultingState->previous = initialState;
				//do not allow negative states
				if (resultingState->currentState.leftBank.getNumChickens() >= 0 && resultingState->currentState.rightBank.getNumChickens() >= 0) {
					if (resultingState->currentState.leftBank.getNumWolves() >= 0 && resultingState->currentState.rightBank.getNumWolves() >= 0) {
						return resultingState;
					}
				}
			}
		}
	}
	else if(initialState->currentState.rightBank.hasBoat()){ //boat is on the right bank
		//if the number of wolves on the right bank is greater than or equal to 1, and the number of chickens on the right bank is greater than or equal to 1.
		if ((initialState->currentState.rightBank.getNumWolves() >= 1) && (initialState->currentState.rightBank.getNumChickens() >= 1)) { 
			//if the number of wolves on the left bank after a move is still valid, and the number of chickens after a move on the left bank is still valid
			if ((initialState->currentState.leftBank.getNumWolves() + 1) <= (initialState->currentState.leftBank.getNumChickens() + 1)) {  
				//and the lord said this was a valid move. Move one chicken and one wolf to the left bank
				//Add 1 to the left bank's current chickens by adding 1 to it.
				resultingState->currentState.leftBank.setNumChickens(initialState->currentState.leftBank.getNumChickens() + 1);
				//Add 1 to the left bank's current wolves by adding 1 to it.
				resultingState->currentState.leftBank.setNumWolves(initialState->currentState.leftBank.getNumWolves() + 1);
				//Remove one chicken and one wolf from the right bank
				//Remove one chicken from the current right bank by subtracting 1 from it
				resultingState->currentState.rightBank.setNumChickens(initialState->currentState.rightBank.getNumChickens() - 1);
				//Remove 1 wolf from the current right bank by subtracting 1 from it.
				resultingState->currentState.rightBank.setNumWolves(initialState->currentState.rightBank.getNumWolves() - 1);
				//remove the boat from the right bank by setting boat to 0
				resultingState->currentState.rightBank.setBoat(0);
				//add the boat to the left bank by setting boat to 1
				resultingState->currentState.leftBank.setBoat(1);
				//set the resulting state's parent as the initial state
				resultingState->previous = initialState;
				//do not allow negative states
				if (resultingState->currentState.leftBank.getNumChickens() >= 0 && resultingState->currentState.rightBank.getNumChickens() >= 0) {
					if (resultingState->currentState.leftBank.getNumWolves() >= 0 && resultingState->currentState.rightBank.getNumWolves() >= 0) {
						if (resultingState->previous->previous != NULL) {
							if (resultingState != resultingState->previous->previous) {
								return resultingState;
							}
						}
						else if (resultingState->previous == NULL) {
							return resultingState;
						}
					}
				}
			}
		}
	}
	return NULL;
}

Node* moveTwoChickens(Node* initialState) {
	Node* resultingState = new Node();
	resultingState->currentState = initialState->currentState;
	resultingState->depth = initialState->depth + 1;
	if (initialState->currentState.leftBank.hasBoat()) { //boat is on the left bank
		//if the left bank has atleast two chickens we can move, and if the number of wolves + 2 is less than or equal to the number of chickens, then we can move two chickens.
		if (initialState->currentState.leftBank.getNumChickens() >= 2 && (initialState->currentState.leftBank.getNumWolves() <= initialState->currentState.leftBank.getNumChickens() - 2)) {
			//verifies the right side. If the number of wolves on the right side is less than or equal to the number of chickens after we add two chickens, it is valid.
			if ((initialState->currentState.rightBank.getNumWolves()) <= (initialState->currentState.rightBank.getNumChickens() + 2)) { //if numChickens + 2 >= numWolves
				//let my animals go. Add two chickens to the right bank by getting the current number of chickens on the right bank and adding two to the chickens on the right bank
				resultingState->currentState.rightBank.setNumChickens(initialState->currentState.rightBank.getNumChickens() + 2);
				//Remove two chickens from the right bank by setting the chickens equal to the left bank's current number of chickens minus 2.
				resultingState->currentState.leftBank.setNumChickens(initialState->currentState.leftBank.getNumChickens() - 2);
				//Remove boat from the left bank
				resultingState->currentState.leftBank.setBoat(0);
				//Add boat to the right bank
				resultingState->currentState.rightBank.setBoat(1);
				//set the current state's previous state as the initial state.
				resultingState->previous = initialState;
				//std::cout << "move 2 c l to r" << std::endl;
				if (resultingState->currentState.leftBank.getNumChickens() >= 0 && resultingState->currentState.rightBank.getNumChickens() >= 0) {
					if (resultingState->currentState.leftBank.getNumWolves() >= 0 && resultingState->currentState.rightBank.getNumWolves() >= 0) {
						return resultingState;
					}
				}
			}
		}
	}
	else if (initialState->currentState.rightBank.hasBoat()) { //the boat is currently on the right bank
		//validate the right side. If the number of chickens >= 2 and if the number of wolves is <= the number of chickens - 2 then it is a valid lefthand state
		if ((initialState->currentState.rightBank.getNumChickens() >= 2) && (initialState->currentState.rightBank.getNumWolves() <= initialState->currentState.rightBank.getNumChickens() -2)) {
			//if the number of wovles on the left side is less than or equal to the number of chickens + 2 which accounts for the post-move value
			if ((initialState->currentState.leftBank.getNumWolves()) <= (initialState->currentState.leftBank.getNumChickens() + 2)) {
				//and the lord said this was a valid move. Set num chickens on right bank -2
				resultingState->currentState.rightBank.setNumChickens(initialState->currentState.leftBank.getNumChickens() - 2);
				//set num chickens on left bank as +2
				resultingState->currentState.leftBank.setNumChickens(initialState->currentState.leftBank.getNumChickens() + 2);
				//set current boat position to left bank and remove from right bank
				resultingState->currentState.leftBank.setBoat(1);
				resultingState->currentState.rightBank.setBoat(0);
				//set previous as parent
				resultingState->previous = initialState;
				//std::cout << "move 2 c right to left" << std::endl;
				if (resultingState->currentState.leftBank.getNumChickens() >= 0 && resultingState->currentState.rightBank.getNumChickens() >= 0) {
					if (resultingState->currentState.leftBank.getNumWolves() >= 0 && resultingState->currentState.rightBank.getNumWolves() >= 0) {
						if (resultingState->previous->previous != NULL) {
							if (resultingState != resultingState->previous->previous) {
								return resultingState;
							}
						}
						else if (resultingState->previous == NULL) {
							return resultingState;
						}
					}
				}
			}
		}
	}
	return NULL;
}

Node* moveTwoWolves(Node* initialState) {
	Node* resultingState = new Node();
	resultingState->currentState = initialState->currentState;
	resultingState->depth = initialState->depth + 1;
	if (initialState->currentState.leftBank.hasBoat()) { //boat is on the left bank
		//if the left bank has atleast two wolves, and if moving two wolves would not invalidate the left bank
		if (initialState->currentState.leftBank.getNumWolves() >= 2 && (initialState->currentState.leftBank.getNumChickens() >= initialState->currentState.leftBank.getNumWolves() - 2)) {
			//if adding two wolves to the right bank doesn't invalidate the state against the number of chickens on the right bank, then make the move
			if ((initialState->currentState.rightBank.getNumWolves() + 2) <= (initialState->currentState.rightBank.getNumChickens())) { //if numChickens >= numWolves + 2
				//unleash the hounds. Add two wolves to the right bank. Take the current value of the right bank and add 2.
				resultingState->currentState.rightBank.setNumWolves(initialState->currentState.rightBank.getNumWolves() + 2); //add two wolves to the right bank
				//Remove two wolves from the left bank. Take the current value of the left bank and subtract 2.
				resultingState->currentState.leftBank.setNumWolves(initialState->currentState.leftBank.getNumWolves() - 2); 
				//Set the left bank's boat val as false
				//Set the right bank boat val as true
				resultingState->currentState.leftBank.setBoat(0);
				resultingState->currentState.rightBank.setBoat(1);
				//set parent to the initial state
				resultingState->previous = initialState;
				//std::cout << "move 2 w l to r" << std::endl;
				if (resultingState->currentState.leftBank.getNumChickens() >= 0 && resultingState->currentState.rightBank.getNumChickens() >= 0) {
					if (resultingState->currentState.leftBank.getNumWolves() >= 0 && resultingState->currentState.rightBank.getNumWolves() >= 0) {
						return resultingState;
					}
				}
			}
		}
	}
	else if (initialState->currentState.rightBank.hasBoat()) {
		//if the initial state's num wolves on the right bank is atleast 2, and if moving two wolves would not invalidate the right bank state
		if (initialState->currentState.rightBank.getNumWolves() >= 2) { //if there 2 wolves we can move
			//if the number of wolves on the left bank + 2 is less than or equal to the number of chickens on the left bank, make the move
			if ((initialState->currentState.leftBank.getNumWolves() + 2) <= (initialState->currentState.leftBank.getNumChickens())) {  //if numChickens >= numWolves + 2;
			//and the lord said this was a valid move
				//remove two wolves from the right bank by subtracting 2 from the rightbank's num wolves
				resultingState->currentState.rightBank.setNumWolves(initialState->currentState.rightBank.getNumWolves() - 2);
				//add two wolves to the left bank by adding two wolves to the left bank's current value.
				resultingState->currentState.leftBank.setNumWolves(initialState->currentState.leftBank.getNumWolves() + 2);
				//remove the boat from the right bank
				resultingState->currentState.rightBank.setBoat(0);
				//add boat to the left bank
				resultingState->currentState.leftBank.setBoat(1);
				//set parent of resulting state as the initial state
				resultingState->previous = initialState;
				//std::cout << "move 2 w r to l" << std::endl;
				if (resultingState->currentState.leftBank.getNumChickens() >= 0 && resultingState->currentState.rightBank.getNumChickens() >= 0) {
					if (resultingState->currentState.leftBank.getNumWolves() >= 0 && resultingState->currentState.rightBank.getNumWolves() >= 0) {
						if (resultingState->previous->previous != NULL) {
							if (resultingState != resultingState->previous->previous) {
								return resultingState;
							}
						}
						else if (resultingState->previous == NULL) {
							return resultingState;
						}
					}
				}
			}
		}
	}
	return NULL;
}

Node* moveOneWolf(Node* initialState) {
	Node* resultingState = new Node();
	resultingState->currentState = initialState->currentState;
	resultingState->depth = initialState->depth + 1;
	if (initialState->currentState.leftBank.hasBoat()) { //boat is on the left bank. Move a wolf from the left bank to the right bank
		if (initialState->currentState.leftBank.getNumWolves() >= 1) { 
			if ((initialState->currentState.rightBank.getNumWolves() + 1) <= (initialState->currentState.rightBank.getNumChickens())) { //if numChickens >= numWolves + 1
				//unleash the hounds. add one wolf to the right bank
				resultingState->currentState.rightBank.setNumWolves(initialState->currentState.rightBank.getNumWolves() + 1);
				//remove one wolf from the left bank.
				resultingState->currentState.leftBank.setNumWolves(initialState->currentState.leftBank.getNumWolves() - 1); 
				//set boat on the left bank to false
				resultingState->currentState.leftBank.setBoat(0);
				//set right bank boat to true
				resultingState->currentState.rightBank.setBoat(1);
				//set current as child of initial state (successor)
				resultingState->previous = initialState;
				//std::cout << "move 1 w l to r" << std::endl;
				if (resultingState->currentState.leftBank.getNumChickens() >= 0 && resultingState->currentState.rightBank.getNumChickens() >= 0) {
					if (resultingState->currentState.leftBank.getNumWolves() >= 0 && resultingState->currentState.rightBank.getNumWolves() >= 0) {
						return resultingState;
					}
				}

			}
		}
	}
	else if (initialState->currentState.rightBank.hasBoat()) { //move wolf from right bank to left bank
		if (initialState->currentState.rightBank.getNumWolves() >= 1) { //if there 2 wolves we can move
			if ((initialState->currentState.leftBank.getNumWolves() + 1) <= (initialState->currentState.leftBank.getNumChickens())) {  //if numChickens >= numWolves + 1;
			//and the lord said this was a valid move
				//set the right bank's num wolves to numWolves - 1
				resultingState->currentState.rightBank.setNumWolves(initialState->currentState.rightBank.getNumWolves() - 1);
				//set the left bank's num wolves as +1
				resultingState->currentState.leftBank.setNumWolves(initialState->currentState.leftBank.getNumWolves() + 1);
				//set boat on the left bank to true
				resultingState->currentState.leftBank.setBoat(1);
				//set right bank boat to false
				resultingState->currentState.rightBank.setBoat(0);
				//set current state's parent to the initial state
				resultingState->previous = initialState;
				//std::cout << "move 1 w r to l" << std::endl;
				if (resultingState->currentState.leftBank.getNumChickens() >= 0 && resultingState->currentState.rightBank.getNumChickens() >= 0) {
					if (resultingState->currentState.leftBank.getNumWolves() >= 0 && resultingState->currentState.rightBank.getNumWolves() >= 0) {
						if (resultingState->previous->previous != NULL) {
							if (resultingState != resultingState->previous->previous) {
								return resultingState;
							}
						}
						else if (resultingState->previous == NULL) {
							return resultingState;
						}
					}
				}
			}
		}
	}
	return NULL;
}

Node* moveOneChicken(Node* initialState) {
	Node* resultingState = new Node();
	resultingState->currentState = initialState->currentState;
	resultingState->depth = initialState->depth + 1;
	if (initialState->currentState.leftBank.hasBoat()) { //boat is on the left bank. We want to move one chicken to the right bank
		//if there is one chicken we can move, and it does not invalidate the left side state
		if (initialState->currentState.leftBank.getNumChickens() >= 1 && initialState->currentState.leftBank.getNumChickens() - 1 >= initialState->currentState.leftBank.getNumWolves()) {
			//if the right bank wolves is less than or equalt o adding a chicken 
			if ((initialState->currentState.rightBank.getNumWolves()) <= (initialState->currentState.rightBank.getNumChickens() + 1)) { //if numChickens + 2 >= numWolves
				//let my animals go. Add one chicken to the right bank
				resultingState->currentState.rightBank.setNumChickens(initialState->currentState.rightBank.getNumChickens() + 1);
				//Remove one chicken from the left bank
				resultingState->currentState.leftBank.setNumChickens(initialState->currentState.leftBank.getNumChickens() - 1);
				//set boat on the left bank to false
				resultingState->currentState.leftBank.setBoat(0);
				//set right bank boat to true
				resultingState->currentState.rightBank.setBoat(1);
				//set current state's parent to the initial state
				resultingState->previous = initialState;
				//std::cout << "move 1 c l to r" << std::endl;
				if (resultingState->currentState.leftBank.getNumChickens() >= 0 && resultingState->currentState.rightBank.getNumChickens() >= 0) {
					if (resultingState->currentState.leftBank.getNumWolves() >= 0 && resultingState->currentState.rightBank.getNumWolves() >= 0) {
						return resultingState;
					}
				}
			}
		}
	}
	else if (initialState->currentState.rightBank.hasBoat()) { //move the boat from the right bank to the left bank
		if (initialState->currentState.rightBank.getNumChickens() >= 1 && (initialState->currentState.rightBank.getNumChickens() - 1 >= initialState->currentState.rightBank.getNumWolves())) { //if we can move a chicken from the right
			if ((initialState->currentState.leftBank.getNumWolves()) <= (initialState->currentState.leftBank.getNumChickens() + 1)) {  //if moving them doesnt invalidate the left bank
				//and the lord said this was a valid move. Remove one chicken from the right bank
				resultingState->currentState.rightBank.setNumChickens(initialState->currentState.rightBank.getNumChickens() - 1);
				//Add one chicken to the left bank
				resultingState->currentState.leftBank.setNumChickens(initialState->currentState.leftBank.getNumChickens() + 1);
				//set boat on the left bank to true
				resultingState->currentState.leftBank.setBoat(1);
				//set right bank boat to false
				resultingState->currentState.rightBank.setBoat(0);
				//set current state's parent to the initial state
				resultingState->previous = initialState;
				if (resultingState->currentState.leftBank.getNumChickens() >= 0 && resultingState->currentState.rightBank.getNumChickens() >= 0) {
					if (resultingState->currentState.leftBank.getNumWolves() >= 0 && resultingState->currentState.rightBank.getNumWolves() >= 0) {
						if (resultingState->previous->previous != NULL) {
							if (resultingState != resultingState->previous->previous) {
								return resultingState;
							}
						}
						else if (resultingState->previous == NULL) {
							return resultingState;
						}
					}
				}
			}
		}
	}
	return NULL;
}

/*
void insertDFS(Node* initialState, Node* successorState, vector<Node*> exploredSet, stack<Node*> frontier) {
	currentNode = new Node();
	currentNode->state = successorState;
	currentNode->previous = initialState;
	currentNode->depth = initialState->depth + 1;
	frontier.push(currentNode);
	//exploredSet.push_back(currentNode);
}
*/

/*******************************************************************************************************************
* Function Name: successor
* Description: returns possible states that are a result of an action and the state provided
* Parameters: 2 node objects
* Output: list of nodes to add to the frontier list
******************************************************************************************************************/

Node successor(Node& initialState, Node& successorNode) {
	//should return the successors of a given node (successorNode) from the given state (initialState)
	//the successors are the potential states that could result from one action being taken from the current state
	//this list of successors should then be added to the frontier in the dfs


	if (initialState.currentState.leftBank.hasBoat() == 0) { //boat is on the right bank
		//CONDITIONS FOR MOVING:
			//Must move one animal, up to 2, but as long as you are not leaving more wolves than chiks behind
			//Keep track of num of overal animals transported over 
			//if there is only one animal left on the bank, immediately transport it and follow the resulting action


		int animalsRem = initialState.currentState.rightBank.numObjects() - 1; //animals left to be transported
	}
	else { //bote in left bank
		//move boat over to the right bank for transportation of animals
		initialState.currentState.leftBank.setBoat(0); //moves boat over from left bank
		initialState.currentState.rightBank.setBoat(1); //boat now in right

		//now move animals within the constraints
		//preferably with a function, so that the if and else can just call it
	}

}


/* Iterative Deepening DFS
* DFS that searches based off the depth of the tree/graph
* starts off with depth = 0, expands those nodes according to dfs
* determines if the node is the goal node, if not then add 1 to the depth search
* continue until a solution has been found
*/


/* Breadth First Search
*  Return solution or failure
* Initialize the frontier using the initial state of hte problem
*  Initialize the explored set to be empty
*  Loop:
*	If the frontier is empty, return failure
*   Choose a leaf node, remove it from the frontier
*   If the node contains a goal state, return corresponding solution
*   Add the node to the explored set
*   Expand chosen node, add the resulting node to the frontier
*      Only if not in the forntier or explored set
*
*   Implement using a FIFO Queue <-This makes the generic graph search a Breadth First Search
*/


//theoretically we should be able to use what we have written for the dfs for all the other uninformed search algos
//just change the specs of the algorithm, FIFO, itteratively deepening LIFO, priority queue, etc.

