#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>

//Create graph structure

struct Node { //keep track of the parent node, current node and depth.
	Node* parent;
	Node* current;
	int depth;
};

//There are two game states: the left river, and the right river, each of which contain W-n wolves, C-m chickens and 0 or 1 boats, where n, m <= W, C respectively.
//We will create two GameState objects, one called Left and one called Right, where each game object contains the number of items on the river.

//TEMPORARY GAME STRUCTURE
class River{
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

int River::numObjects() {
	int BOATGANG = 0;
	if (hasBoat == true) {
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

int main(int argc, char** argv) {
	if (argc != 5) { //check for correct number of arguments
		std::cout << "Invalid number of arguments. argc != 5" << std::endl;
		return 1;
	}
	std::string initialStateFile = argv[1];
	std::string goalStateFile = argv[2];
	std::string mode = argv[3];
	std::string outputFile = argv[4];

	class River leftInitial;
	class River rightInitial;
	class River leftGoal;
	class River rightGoal;

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

	//fill initial and goal states

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

	outputFOUT.close();
	return 0;
}