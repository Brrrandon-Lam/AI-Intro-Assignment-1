#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>

//There are two game states: the left river, and the right river, each of which contain W-n wolves, C-m chickens and 0 or 1 boats, where n, m <= W, C respectively.
//We will create two GameState objects, one called Left and one called Right, where each game object contains the number of items on the river.

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
		//friend bool operator== (GameState& left, GameState& right);
};

/*bool operator== (GameState& left, GameState& right) {
	bool b;
	//left.leftBank
	//left.rightBank
	//right.leftBank
	//right.rightBank

	//if all of those line up for left and right, then return true
	return b;
}*/


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


//IMPORTANT: Priority queue provided by <queue> in C++ is max by default, be sure to change this to a min priority queue.
//Potential solutions: Use negative values or use a custom struct


//create priority queue structure

//create structure for game states


//Function Declarations

void readFromFiles(GameState&, GameState&, std::ifstream&, std::ifstream&);

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

	
	//create the four objects for our stuff. leftBank/rightBank correspond to start.txt and the goal objects correspond to goal.txt
	/*class River leftBank;
	class River rightBank;
	class River leftGoal;
	class River rightGoal;*/

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

	/*
	std::cout << "left bank initial: " << leftBank.getNumChickens() << " " << leftBank.getNumWolves() << " " << leftBank.hasBoat() << std::endl;
	std::cout << "right bank initial: " << rightBank.getNumChickens() << " " << rightBank.getNumWolves() << " " << rightBank.hasBoat() << std::endl;
	std::cout << "left bank gole: " << leftGoal.getNumChickens() << " " << leftGoal.getNumWolves() << " " << leftGoal.hasBoat() << std::endl;
	std::cout << "right bank goul: " << rightGoal.getNumChickens() << " " << rightGoal.getNumWolves() << " " << rightGoal.hasBoat() << std::endl;
	*/

	//close files after we are done with them
	initialFIN.close();
	goalFIN.close();

	//Determine which graph algorithm to run by mode using the compare() function
	if (mode.compare("bfs") == 0) {
		std::cout << "Write code for Breadth-First Search" << std::endl;
	}
	else if (mode.compare("dfs") == 0) {
		std::cout << "Write code for Depth-First Search" << std::endl;
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

	//write solution to output file and print to the terminal, then close the output file
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

Node* depthFirstSearch(GameState& state, GameState& goalState) {
	
	int nodesExpanded = 0;
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
		//BASE CASE: If the initial state is the goal state, then we have already found the shortest path to the goal and can simply return the initial state.
		
		if (initialState->currentState == goalState) {
			return initialState; //may need to operator overload the == for object comparison.
		}

		//NEED TO KEEP TRACK OF EXPANDED NODES (NUMBER) UPDATE BEFORE EXPANSION, BUT AFTER POPPING FROM STACK
		//DONT EXPAND IF IT IS A GOAL NODE

		//nodesExpanded++;


		//expansion function()
			//expands the popped node

			//check and make sure the current node is not in the explored set
				//for runtime purposes
			//calls successor on the popped node
				//determines which states are valid to go to from the current
				//if successor finds one, place it in the frontier
					//then place the popped node into the exploredSet (initialState Node)
				



		Node* successorNode;
		

		//frontier.push_back(successor(initialState, successorNode))

		//all possible actions that can be done:

		//if we want to move one chicken and one wolf
		//if we want to move two chickens
		//if we want to move two wolves
		//if we want to move one chicken
		//if we want to move one wolf

		//if( we are at initial state )
		// then we can move 1 chik and 1 wolf --> resulting state is a successor (AND hasn't already been added to the frontier or explored set)
		// insert this state/node into the frontier for evaluation
		// same with 1 wolf and 2 wolves
		// same with 2 chickens and 1 chik
		// all of the true possibilities should be added to the frontier, then pop the top of the stack
		// continue until we find a successor state that is the goal state
		// return path or unsuccessful

		//if( successor state leads to an already visited state (exploredSet))
			//essentially only add a state that has not been in the frontier OR the explored set
			//only adds unique states
		// then ignore that option
		// do not include it into the frontier, as itis the graph version
		//else
		// add as indicated
		return;
	}

	return NULL; //if we exit the while loop with nothing then there was no solution
}

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

