/*
*Authors: Christopher Bourn & Ethan Puschell
*Creation Date: 11-24-20
*Modification Date: 12-1-20
*Purpose: Program utilizes an Epsilon-Greedy algorithm and Q-Learning in order to iterate through a maze in an attempt to find a goal node. 
*/
#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include<algorithm>
using namespace std;

struct Direction {
	float qValue = 0.0;
	int nValue = 0;
};

struct Node {
	float data = 0.0;
	Direction W, N, E, S;
	string key = "####";
	struct Node* next = NULL;
	struct Node* prev = NULL;
	struct Node* north = NULL;
	struct Node* south = NULL;
};

// AUTHOR: Christopher Bourn 
// CREATION DATE: 11-24-2020
// LAST MODIFIED: 12-1-20
// INPUT: Head node, new key and data, if it's a barrier or not
// OUTPUT: The doubly linked list becomes one larger, with the new node at the end.
// DESCRIPTION: takes in new data, and appends a node with that data to the end of a doubly linked list, starting with a head node
void append(struct Node** head_ref, float new_data, string new_key, bool block)
{
	struct Node* new_node = new Node;

	struct Node* last = *head_ref;

	new_node->data = new_data;

	if (block == true) {
		new_node->W.nValue = -1;
		new_node->W.qValue = -1;
		new_node->N.nValue = -1;
		new_node->N.qValue = -1;
		new_node->E.nValue = -1;
		new_node->E.qValue = -1;
		new_node->S.nValue = -1;
		new_node->S.qValue = -1;
	}

	new_node->key = new_key;

	new_node->next = NULL;
	new_node->north = new_node;
	new_node->south = new_node;

	if (*head_ref == NULL) {
		new_node->prev = NULL;
		*head_ref = new_node;
		return;
	}

	while (last->next != NULL)
		last = last->next;

	last->next = new_node;

	new_node->prev = last;

	return;
}

// AUTHOR: Christopher Bourn
// CREATION DATE: 11-24-2020
// LAST MODIFIED: 11-24-2020
// INPUT: the head node and the key of a node to find
// OUTPUT: the node with the key
// DESCRIPTION: this function takes in a key ie. "a1" or "f5," and outputs the node with that key. 
Node* locate(struct Node* node, string s) {
	struct Node* last{};
	while (node != NULL) {

		if (node->key == s)
			return node;

		last = node;
		node = node->next;
	}
}

// AUTHOR: Christopher Bourn
// CREATION DATE: 11-24-2020
// LAST MODIFIED: 11-24-2020
// INPUT: the head node, a key, and if you want to find the node above or below
// OUTPUT: the node that is above or below
// DESCRIPTION: this function finds the node above or below a node
Node* locate(struct Node* node, string s, bool a) {
	struct Node* last{};

	string tempS0, tempS1;
	if (a == true)
		tempS0 = static_cast<char>(s.at(0) - 1);
	else
		tempS0 = static_cast<char>(s.at(0) + 1);
	tempS1 = s.at(1);
	string sAbove = (tempS0 + tempS1);

	while (node != NULL) {

		if (node->key == sAbove) {
			return node;
		}

		last = node;
		node = node->next;
	}
	return NULL;
}

// AUTHOR: Christopher Bourn
// CREATION DATE: 11-24-2020
// LAST MODIFIED: 11/24/2020
// INPUT: the head node (twice)
// OUTPUT: the grid should now be quadrupaly linked
// DESCRIPTION: this function makes every node linked to the nodes around it, in all four directions.
void reConnectGrid(struct Node* head, struct Node* node) {
	struct Node* last{};
	while (node != NULL) {
		if (node->key.at(0) == 'a') {
			node->north = NULL;
		}
		if (node->key.at(0) == 'f') {
			node->south = NULL;
		}
		if (node->north != NULL) {
			node->north = locate(head, node->key, true);
		}
		if (node->south != NULL) {
			node->south = locate(head, node->key, false);
		}

		last = node;
		node = node->next;
	}
}

// AUTHOR: Christopher Bourn
// CREATION DATE: 12-1-20
// LAST MODIFIED: 12-1-20
// INPUT: the head node
// OUTPUT: a quadrupally linked list with all the nodes with corrrect data in place
// DESCRIPTION: this function creates a quadrupally linked list that contains all 30 nodes with their correct data. 
void fillGrid(struct Node** head) {

	struct Node* goal;
	string msg = "";
	int counter = 1;

	for (int i = 0; i < 30; i++) {

		if (i < 5)
			msg = "a";
		if (i >= 5 && i < 10)
			msg = "b";
		if (i >= 10 && i < 15)
			msg = "c";
		if (i >= 15 && i < 20)
			msg = "d";
		if (i >= 20 && i < 25)
			msg = "e";
		if (i >= 25 && i < 30)
			msg = "f";

		if (i == 6 || i == 7 || i == 11 || i == 16 || i == 17 || i == 21)
			append((head), -1, msg + to_string(counter), true);
		else
			append((head), 0.0, msg + to_string(counter), false);
		if (counter == 5)
			counter = 1;
		else
			counter++;
	}
	reConnectGrid(*head, *head);
	goal = locate(*head, "c3");
	goal->data = 100;
	goal->W.qValue = 100;
	goal->N.qValue = 100;
	goal->E.qValue = 100;
	goal->S.qValue = 100;
}

// AUTHORS: Christopher Bourn & Ethan Puschell
// CREATION DATE: 12-1-20
// LAST MODIFIED: 12-1-20
// INPUT: Pointer to the head node.
// OUTPUT: Q-values for the maze's tiles and directions.
// DESCRIPTION: Function prints out the Q-values given for each tile and direction in the maze.
void printQList(struct Node* node)
{
	cout << "Q-List" << endl;
	int counter = 0;
	int directionCounter = 0;
	struct Node* last;
	while (node != NULL) {

		last = node;

		for (int i = 0; i < 5; i++)
		{
			if (node->data == -1) {
				if (directionCounter == 0 || directionCounter == 2) {
					cout << "             ";
				}
				else {
					if (node->prev->data == -1)
						cout << right << setw(13) << "#### ";
					else
						cout << right << setw(8) << "#### ";
				}
			}
			else if (node->key == "c3")
			{
				if (directionCounter == 0 || directionCounter == 2)
					cout << "             ";
				else
					cout << right << setw(13) << "+100";
			}
			else {
				if (directionCounter == 0) {
					cout << right << setw(13) << fixed << setprecision(2) << node->N.qValue;
				}
				if (directionCounter == 1)
				{
					if (node->key == "a1" || node->key == "b1" || node->key == "c1" || node->key == "c4" || node->key == "d1" || node->key == "e1" || node->key == "f1" || node->prev->data == -1)
						cout << right << setw(10);
					else
						cout << right << setw(5);
					cout << node->W.qValue << " " << left << setw(7) << node->E.qValue;
				}
				if (directionCounter == 2) {
					cout << right << setw(13) << fixed << setprecision(2) << node->S.qValue;
				}
			}

			node = node->next;

			if (i == 4) {
				cout << endl;
			}
		}
		if (directionCounter == 2) {
			directionCounter = 0;
			cout << endl;
		}
		else {
			node = last;
			counter++;
			directionCounter++;
		}
	}
	cout << endl;
}

// AUTHORS: Christopher Bourn & Ethan Puschell
// CREATION DATE: 12-1-20
// LAST MODIFIED: 12-1-20
// INPUT: Pointer to the head node.
// OUTPUT: N-values for the maze's tiles and directions.
// DESCRIPTION: Function prints out the N-values given for each tile and direction in the maze.
void printNList(struct Node* node)
{
	cout << "N-List" << endl;
	int counter = 0;
	int directionCounter = 0;
	struct Node* last;
	while (node != NULL) {

		last = node;

		for (int i = 0; i < 5; i++)
		{
			if (node->data == -1) {
				if (directionCounter == 0 || directionCounter == 2) {
					cout << "                 ";
				}
				else {
					if (node->prev->data == -1)
						cout << right << setw(14) << "####";
					else
						cout << right << setw(8) << "####";
				}
			}
			else if (node->key == "c3")
			{
				if (directionCounter == 0 || directionCounter == 2)
					cout << "           ";
				else
					cout << right << setw(14) << "+100";
			}
			else {
				if (directionCounter == 0) {
					if (node->key == "b4" || node->key == "d4")
						cout << right << setw(8);
					else if (node->key == "c3" || node->key == "e3")
						cout << right << setw(11);
					else
						cout << right << setw(14);
					cout << fixed << setprecision(2) << node->N.nValue;
				}
				if (directionCounter == 1)
				{
					if (node->key == "a1" || node->key == "b1" || node->key == "c1" || node->key == "d1" || node->key == "e1" || node->key == "f1")
						cout << right << setw(13);
					else if (node->key == "c4" || node->prev->data == -1)
						cout << right << setw(12);
					else
						cout << right << setw(6);
					cout << node->W.nValue << " " << left << setw(7) << node->E.nValue;
				}
				if (directionCounter == 2) {
					if (node->key == "b4" || node->key == "d4")
						cout << right << setw(8);
					else if (node->key == "c3" || node->key == "e3")
						cout << right << setw(11);
					else
						cout << right << setw(14);
					cout << fixed << setprecision(2) << node->S.nValue;
				}
			}

			node = node->next;

			if (i == 4) {
				cout << endl;
			}
		}
		if (directionCounter == 2) {
			directionCounter = 0;
			cout << endl;
		}
		else {
			node = last;
			counter++;
			directionCounter++;
		}
	}
	cout << endl;
}

// AUTHOR: Christopher Bourn
// CREATION DATE: 11-24-2020
// LAST MODIFIED: 11-24-2020
// INPUT: the head node
// OUTPUT: the keys of all the nodes
// DESCRIPTION: simple print function that prints the keys of all the nodes.
void printKey(struct Node* node) {
	cout << "Key" << endl << left;
	int counter = 0;
	struct Node* last{};
	while (node != NULL) {
		if (counter == 5 || counter == 10 || counter == 15 || counter == 20 || counter == 25) {
			cout << endl;
		}
		if (node->data == -1) {
			cout << setw(8) << "####" << " ";
		}
		else
			cout << setw(8) << node->key << " ";
		last = node;
		node = node->next;
		counter++;
	}
}

// AUTHOR: Ethan Puschell
// CREATION DATE: 12-1-20
// LAST MODIFIED: 12-1-20
// INPUT: Pointer to the head node
// OUTPUT: Randomly selected node
// DESCRIPTION: Function returns a random node in the quad-linked list to start the calculations.
Node* RandomStart(Node* head)
{
	srand((int)time(0));						//Seeds the random function.
	struct Node* temp = locate(head, "b2");		//Set the temporary node equal to b2, which is an obstacle and therefore its data value is equal to -1
	while (temp->data == -1)					//While loop that iterates until a Node that is not an obstacle is found
	{
		int letter = rand() % 6;				//Initialize letter to a value between 0-5
		int column = rand() % 5 + 1;			//Initialize column to a value between 1 - 5
		string row, startKey;					//Strings that will hold the row and the complete start key.
		switch (letter)							//Switch for letter that will determine which row the function will start in (from a - e).
		{
		case 0:
			row = "a";
			break;
		case 1:
			row = "b";
			break;
		case 2:
			row = "c";
			break;
		case 3:
			row = "d";
			break;
		case 4:
			row = "e";
			break;
		case 5:
			row = "f";
			break;
		}
		temp = locate(head, row + to_string(column));	//Once a non-obstacle Node has been chosen, set the temp equal to that Node.
	}
	return temp;										//Returns the temp node to start the E-Greedy algorithm.
}

// AUTHOR: Ethan Puschell
// CREATION DATE: 12-1-20
// LAST MODIFIED: 12-2-20
// INPUT: The state Node and the char for the action.
// OUTPUT: Determines the action and next state for E-Greedy.
// DESCRIPTION: Function randomly chooses an action and new state for the E-Greedy algorithm.
Node* RandomTile(Node* state, char& action)
{
	srand((int)time(0));										//Seed the random function.
	int d4 = rand() % 4;										//Roll a d4 to see what direction will be chosen.
	switch (d4)													//Switch that will determine the action and next state for E-Greedy.
	{
	case(0):
		action = 'W';
		return state->prev;
		break;
	case(1):
		action = 'E';
		return state->next;
		break;
	case(2):
		action = 'N';
		return state->north;
		break;
	case(3):
		action = 'S';
		return state->south;
		break;
	default:													//If the d4 fails to roll, give an error.
		cerr << "ERROR: Tile was not randomly chosen." << endl;
		system("pause");
		exit(0);
	}
}

// AUTHOR: Ethan Puschell
// CREATION DATE: 12-2-20
// LAST MODIFIED: 12-2-20
// INPUT: The state Node and the action char.
// OUTPUT: Returns the next state and action.
// DESCRIPTION: Function randomly determines if the action and next state is West, North, or South.
Node* RandomWNS(Node* state, char& action)
{
	srand((int)time(0));					//Seed the random function
	int d3 = rand() % 3;					//Roll a d3 (doesn't exist, but for the sake of computer science pretend it does).
	switch (d3)								//Switch that checks the result of the roll and whether the next state and action is West, North, or South.
	{
	case(0):
		action = 'W';
		return state->prev;
		break;
	case(1):
		action = 'N';
		return state->north;
		break;
	case(2):
		action = 'S';
		return state->south;
		break;
	}
}

// AUTHOR: Ethan Puschell
// CREATION DATE: 12-2-20
// LAST MODIFIED: 12-2-20
// INPUT: The state Node and the action char.
// OUTPUT: Returns the next state and action.
// DESCRIPTION: Function randomly determines if the action and next state is West, North, or East.
Node* RandomWNE(Node* state, char& action)
{
	srand((int)time(0));				//Seed the random function
	int d3 = rand() % 3;				//Roll a d3
	switch (d3)							//Switch that checks the result of the roll and whether the next state and action is West, North, or East.
	{
	case(0):
		action = 'W';
		return state->prev;
		break;
	case(1):
		action = 'N';
		return state->north;
		break;
	case(2):
		action = 'E';
		return state->next;
		break;
	}
}

// AUTHOR: Ethan Puschell
// CREATION DATE: 12-2-20
// LAST MODIFIED: 12-2-20
// INPUT: The state Node and the action char.
// OUTPUT: Returns the next state and action.
// DESCRIPTION: Function randomly determines if the action and next state is North, East, or South.
Node* RandomNES(Node* state, char& action)
{
	srand((int)time(0));				//Seed the random function
	int d3 = rand() % 3;				//Roll a d3
	switch (d3)							//Switch that checks the result of the roll and whether the next state and action is North, East, or South.
	{
	case(0):
		action = 'N';
		return state->north;
		break;
	case(1):
		action = 'E';
		return state->next;
		break;
	case(2):
		action = 'S';
		return state->south;
		break;
	}
}

// AUTHOR: Ethan Puschell
// CREATION DATE: 12-2-20
// LAST MODIFIED: 12-2-20
// INPUT: The state Node and the action char.
// OUTPUT: Returns the next state and action.
// DESCRIPTION: Function randomly determines if the action and next state is West, East, or South.
Node* RandomWES(Node* state, char& action)
{
	srand((int)time(0));				//Seed the random function
	int d3 = rand() % 3;				//Roll a d3
	switch (d3)							//Switch that checks the result of the roll and whether the next state and action is North, East, or South.
	{
	case(0):
		action = 'W';
		return state->prev;
		break;
	case(1):
		action = 'E';
		return state->next;
		break;
	case(2):
		action = 'S';
		return state->south;
		break;
	}
}

// AUTHOR: Ethan Puschell
// CREATION DATE: 12-2-20
// LAST MODIFIED: 12-2-20
// INPUT: The state Node and the action char.
// OUTPUT: Returns the next state and action.
// DESCRIPTION: Function randomly determines if the action and next state is West or North.
Node* RandomWN(Node* state, char& action)
{
	srand((int)time(0));				//Seed the random function
	int coin = rand() % 2;				//Flip a coin
	if (coin == 0)						//Switch that checks the result of the coin flip and whether the next state and action is West or North.
	{
		action = 'W';
		return state->prev;
	}
	else
	{
		action = 'N';
		return state->north;
	}
}

// AUTHOR: Ethan Puschell
// CREATION DATE: 12-2-20
// LAST MODIFIED: 12-2-20
// INPUT: The state Node and the action char.
// OUTPUT: Returns the next state and action.
// DESCRIPTION: Function randomly determines if the action and next state is West or East.
Node* RandomWE(Node* state, char& action)
{
	srand((int)time(0));				//Seed the random function
	int coin = rand() % 2;				//Flip a coin
	if (coin == 0)						//Switch that checks the result of the coin flip and whether the next state and action is West or East.
	{
		action = 'W';
		return state->prev;
	}
	else
	{
		action = 'E';
		return state->next;
	}
}

// AUTHOR: Ethan Puschell
// CREATION DATE: 12-2-20
// LAST MODIFIED: 12-2-20
// INPUT: The state Node and the action char.
// OUTPUT: Returns the next state and action.
// DESCRIPTION: Function randomly determines if the action and next state is West or South.
Node* RandomWS(Node* state, char& action)
{
	srand((int)time(0));				//Seed the random function
	int coin = rand() % 2;				//Flip a coin
	if (coin == 0)						//Switch that checks the result of the coin flip and whether the next state and action is West or South.
	{
		action = 'W';
		return state->prev;
	}
	else
	{
		action = 'S';
		return state->south;
	}
}

// AUTHOR: Ethan Puschell
// CREATION DATE: 12-2-20
// LAST MODIFIED: 12-2-20
// INPUT: The state Node and the action char.
// OUTPUT: Returns the next state and action.
// DESCRIPTION: Function randomly determines if the action and next state is North or East. 
Node* RandomNE(Node* state, char& action)
{
	srand((int)time(0));				//Seed the random function
	int coin = rand() % 2;				//Flip a coin
	if (coin == 0)						//Switch that checks the result of the coin flip and whether the next state and action is North or East.
	{
		action = 'N';
		return state->north;
	}
	else
	{
		action = 'E';
		return state->next;
	}
}

// AUTHOR: Ethan Puschell
// CREATION DATE: 12-2-20
// LAST MODIFIED: 12-2-20
// INPUT: The state Node and the action char.
// OUTPUT: Returns the next state and action.
// DESCRIPTION: Function randomly determines if the action and next state is North or South. 
Node* RandomNS(Node* state, char& action)
{
	srand((int)time(0));				//Seed the random function
	int coin = rand() % 2;				//Flip a coin
	if (coin == 0)						//Switch that checks the result of the coin flip and whether the next state and action is North or South.
	{
		action = 'N';
		return state->north;
	}
	else
	{
		action = 'S';
		return state->south;
	}
}

// AUTHOR: Ethan Puschell
// CREATION DATE: 12-2-20
// LAST MODIFIED: 12-2-20
// INPUT: The state Node and the action char.
// OUTPUT: Returns the next state and action.
// DESCRIPTION: Function randomly determines if the action and next state is East or South. 
Node* RandomES(Node* state, char& action)
{
	srand((int)time(0));				//Seed the random function
	int coin = rand() % 2;				//Flip a coin
	if (coin == 0)						//Switch that checks the result of the coin flip and whether the next state and action is East or South.
	{
		action = 'E';
		return state->next;
	}
	else
	{
		action = 'S';
		return state->south;
	}
}

// AUTHOR: Ethan Puschell
// CREATION DATE: 12-1-20
// LAST MODIFIED: 12-3-20
// INPUT: State and action.
// OUTPUT: Optimal or random tile.
// DESCRIPTION: Function determines the optimal tile, and uses the 95% probability to determine whether it chooses an optimal action or a random action.
Node* OptimalTile(Node* state, char& action)
{
	srand((int)time(0));							//Seed the random function.
	int rng = rand() % 100 + 1;						//Initialize rng that will be in the range of 1-100 for the probability that the optimal or random action is chosen.
	if ((state->W.qValue == state->N.qValue) &&		//If West, North, and South are the optimal actions
		(state->W.qValue == state->S.qValue) &&
		(state->W.qValue > state->E.qValue))
	{
		if (rng <= 95)
			return RandomWNS(state, action);		//95% chance that either West, North, or South are chosen
		else
			return RandomTile(state, action);		//5% chance that a random action is chosen.
	}
	else if ((state->W.qValue == state->N.qValue) &&	//If West, North, and East are the optimal actions
		(state->W.qValue == state->E.qValue) &&
		(state->W.qValue > state->S.qValue))
	{
		if (rng <= 95)
			return RandomWNE(state, action);			//95% chance that either West, North, or East are chosen
		else
			return RandomTile(state, action);			//5% chance that a random action is chosen.
	}
	else if ((state->N.qValue == state->E.qValue) &&	//If North, East, and South are the optimal actions
		(state->N.qValue == state->S.qValue) &&
		(state->N.qValue > state->W.qValue))
	{
		if (rng <= 95)
			return RandomNES(state, action);			//95% chance that North, East, and South are chosen
		else
			return RandomTile(state, action);			//5% chance that a random action is chosen.
	}
	else if ((state->W.qValue == state->E.qValue) &&	//If West, East, and South are the optimal actions
		(state->W.qValue == state->S.qValue) &&
		(state->W.qValue > state->N.qValue))
	{
		if (rng <= 95)
			return RandomWES(state, action);			//95% chance that West, East, or South are chosen
		else
			return RandomTile(state, action);			//5% chance that a random action is chosen.
	}
	else if ((state->W.qValue == state->N.qValue) &&	//If West and North are the optimal actions
		(state->W.qValue > state->E.qValue) &&
		(state->W.qValue > state->S.qValue))
	{
		if (rng <= 95)
			return RandomWN(state, action);				//95% chance that West or North are chosen
		else
			return RandomTile(state, action);			//5% chance that a random action is chosen.
	}
	else if ((state->W.qValue == state->E.qValue) &&	//If West and East are the optimal actions
		(state->W.qValue > state->N.qValue) &&
		(state->W.qValue > state->S.qValue))
	{
		if (rng <= 95)
			return RandomWE(state, action);				//95% chance that West or North are chosen
		else
			return RandomTile(state, action);			//5% chance that a random action is chosen.
	}
	else if ((state->W.qValue == state->S.qValue) &&	//If West and South are optimal actions
		(state->W.qValue > state->N.qValue) &&
		(state->W.qValue > state->E.qValue))
	{
		if (rng <= 95)
			return RandomWS(state, action);				//95% chance that West or South are chosen.
		else
			return RandomTile(state, action);			//5% chance that a random action is chosen.
	}
	else if ((state->N.qValue == state->E.qValue) &&	//If North and East are optimal actions
		(state->N.qValue > state->W.qValue) &&
		(state->N.qValue > state->S.qValue))
	{
		if (rng <= 95)
			return RandomNE(state, action);				//95% chance that North and East are chosen
		else
			return RandomTile(state, action);			//5% chance that a random action is chosen.
	}
	else if ((state->N.qValue == state->S.qValue) &&	//If North and South are optimal actions
		(state->N.qValue > state->W.qValue) &&
		(state->N.qValue > state->E.qValue))
	{
		if (rng <= 95)
			return RandomNS(state, action);				//95% chance that North or South are chosen
		else
			return RandomTile(state, action);			//5% chance that a random action is chosen.
	}
	else if ((state->E.qValue == state->S.qValue) &&	//If East and South are optimal actions
		(state->E.qValue > state->W.qValue) &&
		(state->E.qValue > state->N.qValue))
	{
		if (rng <= 95)
			return RandomES(state, action);				//95% chance that East or South are chosen
		else
			return RandomTile(state, action);			//5% chance that a random action is chosen.
	}
	else if ((state->W.qValue > state->N.qValue) &&		//If West is the optimal action
		(state->W.qValue > state->E.qValue) &&
		(state->W.qValue > state->S.qValue))
	{
		if (rng <= 95)									//95% chance that West is chosen
		{
			action = 'W';
			return state->prev;
		}
		else
			return RandomTile(state, action);			//5% chance a random action is chosen.
	}
	else if ((state->N.qValue > state->W.qValue) &&		//If North is the optimal action
		(state->N.qValue > state->E.qValue) &&
		(state->N.qValue > state->S.qValue))
	{
		if (rng <= 95)									//95% chance that North is chosen
		{
			action = 'N';
			return state->north;
		}
		else
			return RandomTile(state, action);			//5% chance that a random action is chosen.
	}
	else if ((state->E.qValue > state->W.qValue) &&		//If East is the optimal action
		(state->E.qValue > state->N.qValue) &&
		(state->E.qValue > state->S.qValue))
	{
		if (rng <= 95)									//95% chance that East is chosen
		{
			action = 'E';
			return state->next;
		}
		else
			return RandomTile(state, action);			//5% chance that a random action is chosen.
	}
	else if ((state->S.qValue > state->W.qValue) &&		//If South is the optimal action
		(state->S.qValue > state->N.qValue) &&
		(state->S.qValue > state->E.qValue))
	{
		if (rng <= 95)									//95% chance that South is chosen
		{
			action = 'S';
			return state->south;
		}
		else
			return RandomTile(state, action);			//5% chance that a random action is chosen.
	}
	else
	{
		cerr << "ERROR: Optimal action was not found." << endl;		//If no tile is the optimal action, there exists an error.
		system("pause");
		exit(0);
	}
}

// AUTHOR: Ethan Puschell
// CREATION DATE: 12-2-20
// LAST MODIFIED: 12-2-20
// INPUT: State and action.
// OUTPUT: Node and updated action.
// DESCRIPTION: Function calculates the probability of drift to the left or right perpendicular to the action of the E-Greedy algorithm.
Node* TDrift(Node* state, char& action)
{
	srand((int)time(0));					//Seed the random function.
	int rng = rand() % 100 + 1;				//Initialize rng from range 1-100.
	switch (action)							//Switch that checks what the current action is.
	{
	case ('W'):								//If the action is West
		if (rng <= 70)
			return state->prev;				//70% chance the next state remains West
		else if (rng > 70 && rng <= 85)		//15% chance of drifting to the North
		{
			action = 'N';
			return state->north;
		}
		else
		{
			action = 'S';					//15% chance of drifting to the South.
			return state->south;
		}
		break;
	case('N'):								//If the action is North
		if (rng <= 70)						//70% chance the state remains North
			return state->north;
		else if (rng > 70 && rng <= 85)
		{
			action = 'W';					//15% chance of drifting to the West
			return state->prev;
		}
		else
		{
			action = 'E';					//15% chance of drifting to the East.
			return state->next;
		}
		break;
	case('E'):								//If the action is East
		if (rng <= 70)						//70% chance the next state remains East.
			return state->next;
		else if (rng > 70 && rng <= 85)
		{
			action = 'N';					//15% chance of drifting to the North.
			return state->north;
		}
		else
		{
			action = 'S';					//15% chance of drifting to the South.
			return state->south;
		}
		break;
	case('S'):								//If the action is South
		if (rng <= 70)						//70% chance the next state remains South
			return state->south;
		else if (rng > 70 && rng <= 85)
		{
			action = 'W';					//15% chance of drifting to the West.
			return state->prev;
		}
		else
		{
			action = 'E';					//15% chance of drifting to the East.
			return state->next;
		}
		break;
	}
}

// AUTHOR: Ethan Puschell
// CREATION DATE: 12-2-20
// LAST MODIFIED: 12-2-20
// INPUT: Next state node.
// OUTPUT: Maximum Q-value from the next state.
// DESCRIPTION: Function returns the maximum Q-value from the next state to the Q-Learning update.
float MaxQSA(Node* nextState)
{
	float optimalAction[4] = { nextState->W.qValue, nextState->N.qValue, nextState->E.qValue, nextState->S.qValue };	//Create an array that contains the Q-values from each direction of the next state.
	float* optimalCost = max_element(optimalAction, optimalAction + 4);													//Use max_element to return the largest element of the array of Q-values.
	if (*optimalCost == nextState->W.qValue)																			//If the Max Q-value is West, return it to the Q-Learning update function.
		return nextState->W.qValue;
	else if (*optimalCost == nextState->N.qValue)																			//If the Max Q-value is North, return it to the Q-Learning update function.
		return nextState->N.qValue;
	else if (*optimalCost == nextState->E.qValue)																			//If the Max Q-value is East, return it to the Q-Learning update function.
		return nextState->E.qValue;
	else if (*optimalCost == nextState->S.qValue)																			//If the Max Q-value is South, return it to the Q-Learning update function.
		return nextState->S.qValue;
}

// AUTHOR: Ethan Puschell
// CREATION DATE: 12-2-20
// LAST MODIFIED: 12-2-20
// INPUT: state, next state, and action.
// OUTPUT: New Q-value and N-value for the state.
// DESCRIPTION: Function uses Q-learning to update the Q-value and N-value for the current state.
void UpdateNQ(Node* state, Node* nextState, char action)
{
	float nsa, qsa;																			//Declare the N-value and Q-value for calculations.				
	switch (action)																			//Switch for the action
	{
	case ('W'):																				//If the action is West, it's R(s,a) = -2.
		state->W.nValue++;																	//N(s,a) <- N(s,a) + 1 
		nsa = (float)state->W.nValue;														//Cast the N-value as a float into the temp variable
		qsa = (float)state->W.qValue;														//Cast the Q-value as a float into the temp variable
		state->W.qValue = qsa + ((1.0 / nsa) * (-2.0 + (0.9 * MaxQSA(nextState)) - qsa));	//Q(s,a) <- Q(s,a) + 1 / N(s,a) * [R(s,a) + ymaxQ(s',a') - Q(s,a)].
		break;
	case ('N'):																				//If the action is North, it's R(s,a) = -3.
		state->N.nValue++;																	//N(s,a) <- N(s,a) + 1 
		nsa = (float)state->N.nValue;														//Cast the N-value as a float into the temp variable
		qsa = (float)state->N.qValue;														//Cast the Q-value as a float into the temp variable
		state->N.qValue = qsa + ((1.0 / nsa) * (-3.0 + (0.9 * MaxQSA(nextState)) - qsa));	//Q(s,a) <- Q(s,a) + 1 / N(s,a) * [R(s,a) + ymaxQ(s',a') - Q(s,a)].
		break;
	case ('E'):																				//If the action is East it's R(s,a) = -2.
		state->E.nValue++;																	//N(s,a) <- N(s,a) + 1 
		nsa = (float)state->E.nValue;														//Cast the N-value as a float into the temp variable
		qsa = (float)state->E.qValue;														//Cast the Q-value as a float into the temp variable
		state->E.qValue = qsa + ((1.0 / nsa) * (-2.0 + (0.9 * MaxQSA(nextState)) - qsa));	//Q(s,a) <- Q(s,a) + 1 / N(s,a) * [R(s,a) + ymaxQ(s',a') - Q(s,a)].
		break;
	case('S'):																				//If the action is South, it's R(s,a) = -1.
		state->S.nValue++;																	//N(s,a) <- N(s,a) + 1 
		nsa = (float)state->S.nValue;														//Cast the N-value as a float into the temp variable
		qsa = (float)state->S.qValue;														//Cast the Q-value as a float into the temp variable
		state->S.qValue = qsa + ((1.0 / nsa) * (-1.0 + (0.9 * MaxQSA(nextState)) - qsa));	//Q(s,a) <- Q(s,a) + 1 / N(s,a) * [R(s,a) + ymaxQ(s',a') - Q(s,a)].
		break;
	}
}

// AUTHOR: Ethan Puschell
// CREATION DATE: 12-1-20
// LAST MODIFIED: 12-2-20
// INPUT: State and counter.
// OUTPUT: Location of goal state with updated Q-values and N-values, or 100 unsuccessful attempts.
// DESCRIPTION: Function uses the Epsilon-greedy approach to find the goal state.
void EGreedy(Node* state, int& counter)
{
	if (state->data == 100 || counter == 100)		//Base case for recursive function: If the goal state is found or if there were 100 attempts made, stop the function.
		return;
	counter++;										//Else, use a postfix operator to increment the counter.
	Node* nextState;								//Declaration of the next state that the action will lead into.
	char action;									//Declaration of the action that will determine the next direction and reward.
	if ((state->N.qValue == state->W.qValue) &&		//If all directions are the optimal action
		(state->N.qValue == state->E.qValue) &&
		(state->N.qValue == state->S.qValue) &&
		(state->S.qValue == state->E.qValue) &&
		(state->W.qValue == state->E.qValue) &&
		(state->W.qValue == state->S.qValue))
		nextState = RandomTile(state, action);		//Set the next state and action equal to a random Tile and direction.
	else
		nextState = OptimalTile(state, action);		//Otherwise, set the next state and action equal to the optimal action (with a 95% probability).
	if (nextState == NULL || nextState->data == -1 || state->key.find('1') != string::npos && action == 'W' || state->key.find('5') != string::npos && action == 'E')
		nextState = state;							//If the action would cause a collision with an obstacle or edge, set the next state equal to the current state.
	nextState = TDrift(state, action);				//Calculate the 70% probability that the state keeps its current action or if there is a drift to the left or right.
	if (nextState == NULL || nextState->data == -1 || state->key.find('1') != string::npos && action == 'W' || state->key.find('5') != string::npos && action == 'E')
		nextState = state;							//Check again if there is a collision with the action, and set the next state equal to the current state if there is.
	UpdateNQ(state, nextState, action);				//Use Q-Learning to update the Q-value and N-value of the state.
	EGreedy(nextState, counter);					//Recursively call E-Greedy approach with the next state to continue searching for the goal state.
}

// AUTHOR: Christopher Bourn
// CREATION DATE: 12-2-20
// LAST MODIFIED: 12-3-20
// INPUT: the head node
// OUTPUT: a string called "msg" that has arrows that represents the direction of the optimal path at that state
// DESCRIPTION: this function finds the maximum N-Value of a state and outputs arrows corresponding to its respective direction
string optimalPath(struct Node* state) {
	string msg = "";
	float optimalDirection = 0;
	float optimalPath[4] = { state->W.nValue, state->N.nValue, state->E.nValue, state->S.nValue };

	if (state->W.nValue == state->N.nValue == state->E.nValue == state->S.nValue) {
		msg = "N/A";
		return msg;
	}

	for (int i = 0; i < 4; i++) {
		if (optimalPath[i] > optimalDirection) {
			optimalDirection = optimalPath[i];
		}
	}

	if (optimalDirection == state->W.nValue)
	{
		msg = "<<<<";
	}
	else if (optimalDirection == state->N.nValue)
	{
		msg = "^^^^";
	}
	else if (optimalDirection == state->E.nValue)
	{
		msg = ">>>>";
	}
	else if (optimalDirection == state->S.nValue)
	{
		msg = "vvvv";
	}
	else
	{
		cerr << "ERROR: Optimal path was not correctly found!" << endl;
		system("pause");
		exit(0);
	}
	return msg;
}

// AUTHOR: Christopher Bourn
// CREATION DATE: 12-3-20
// LAST MODIFIED: 12-3-20
// INPUT: the head node
// OUTPUT: the grid of arrows
// DESCRIPTION: simple print function that calls the OptimalPath function to find the optimal path at each state.
void printPath(struct Node* node) {
	cout << "Optimal path" << endl << left;
	int counter = 0;
	struct Node* last{};
	while (node != NULL) {
		if (counter == 5 || counter == 10 || counter == 15 || counter == 20 || counter == 25) {
			cout << endl;
		}
		if (node->data == -1) {
			cout << setw(8) << "####" << " ";
		}
		else
			cout << setw(8) << optimalPath(node) << " ";
		last = node;
		node = node->next;
		counter++;
	}
	cout << endl;
}

int main() {
	srand((int)time(0));						//Seed the random function.
	Node* head = NULL;							//Initialize the head Node to NULL.
	int counter = 0;							//Initialize the counter to 0.
	fillGrid((&head));							//Use the head node to fill the grid and create the maze.
	printKey(head);								//Function outputs the keys of each node for programming and troubleshooting.
	cout << endl << endl;

	for (int i = 0; i < 10000; i++)				//Run the E-greedy function for 10,000 trials.
	{
		counter = 0;
		EGreedy(RandomStart(head), counter);
	}
	printNList(head);							//Print the N-values of the maze.
	printQList(head);							//Print the Q-values of the maze.
	printPath(head);							//Print the optimal path of the maze.

	system("pause");
	return 0;
}
