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
/* TXT FILE STRUCTURE: 
*  first line left: numChickens, numWolves, boat
*  second line right: numChickens, numWolves, boat
*/

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

void readFromFiles(River&, River&, River&, River&, std::ifstream&, std::ifstream&);

int main(int argc, char** argv) {
	if (argc != 5) { //check for correct number of arguments
		//arguments should be executable initial.txt goal.txt mode output.txt 
		std::cout << "Invalid number of arguments. argc != 5" << std::endl;
		return 1;
	}
	std::string initialStateFile = argv[1];
	std::string goalStateFile = argv[2];
	std::string mode = argv[3];
	std::string outputFile = argv[4];

	class River leftBank;
	class River rightBank;
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
	readFromFiles(leftBank, rightBank, leftGoal, rightGoal, initialFIN, goalFIN);

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


void readFromFiles(River& leftBank, River& rightBank, River& leftGoal, River& rightGoal, std::ifstream& initialFIN, std::ifstream& goalFIN) {
	int chickens = 0;
	int wolves = 0;
	bool bote;
	char temp69420;
	char temp;
	//get contents from first line of the initial.txt
	initialFIN >> chickens >> temp69420 >> wolves >> temp >> bote;
	//store them into the object (leftBank)
	leftBank.setNumChickens(chickens);
	leftBank.setNumWolves(wolves);
	leftBank.setBoat(bote);

	/*
	std::cout << "Left Bank Initial " << chickens << " " << wolves << " " << bote << std::endl;
	std::cout << "Left Bank Initial " << leftBank.getNumChickens() << " " << leftBank.getNumWolves() << " " << leftBank.hasBoat() << std::endl;
	*/

	//get contents from second line of initial.txt
	initialFIN >> chickens >> temp >> wolves >> temp69420 >> bote;
	//store in rightBank object
	rightBank.setNumChickens(chickens);
	rightBank.setNumWolves(wolves);
	rightBank.setBoat(bote);
	
	//std::cout << "Right Bank Initial " << chickens << " " << wolves << " " << bote << std::endl;

	//get contents from first line of goal.txt
	goalFIN >> chickens >> temp >> wolves >> temp69420 >> bote;
	//store them in leftGoal object
	leftGoal.setNumChickens(chickens);
	leftGoal.setNumWolves(wolves);
	leftGoal.setBoat(bote);
	//get contents from second line of goal.txt
	goalFIN >> chickens >> temp69420 >> wolves >> temp >> bote;
	//store them in rightGoal object
	rightGoal.setNumChickens(chickens);
	rightGoal.setNumWolves(wolves);
	rightGoal.setBoat(bote);
	//Our 4 objects now have all the data from the files so we are done

}