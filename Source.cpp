/*
*Authors: Christopher Bourn & Ethan Puschell
*Creation Date: 11-24-20
*Modification Date: 12-1-20
*Purpose:
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
// INPUT: 
// OUTPUT: 
// DESCRIPTION:
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
// LAST MODIFIED:
// INPUT: 
// OUTPUT: 
// DESCRIPTION:
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
// LAST MODIFIED:
// INPUT: 
// OUTPUT: 
// DESCRIPTION: 2nd function finds the node above or below a node
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
// LAST MODIFIED:
// INPUT: 
// OUTPUT: 
// DESCRIPTION:
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
// INPUT: 
// OUTPUT: 
// DESCRIPTION: 
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
// OUTPUT: Q-values for the maze's tiles and directions
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
// OUTPUT: N-values for the maze's tiles and directions
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
// LAST MODIFIED:
// INPUT:
// OUTPUT:
// DESCRIPTION:
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
	srand((int)time(0));
	struct Node* temp = locate(head, "b2");
	while (temp->data == -1)
	{
		int letter = rand() % 6;
		int column = rand() % 5 + 1;
		string row, startKey;
		switch (letter)
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
		temp = locate(head, row + to_string(column));
	}
	//cout << endl << "Randomly generated key: " << temp->key << endl;
	return temp;
}

// AUTHOR: Ethan Puschell
// CREATION DATE: 12-1-20
// LAST MODIFIED: 12-2-20
// INPUT: 
// OUTPUT: 
// DESCRIPTION: 
Node* RandomTile(Node* state, char& action)
{
	srand((int)time(0));
	int d4 = rand() % 4;
	switch (d4)
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
	default:
		cerr << "ERROR: Tile was not randomly chosen." << endl;
		system("pause");
		exit(0);
	}
}

// AUTHOR: Ethan Puschell
// CREATION DATE: 12-2-20
// LAST MODIFIED: 12-2-20
// INPUT: 
// OUTPUT: 
// DESCRIPTION: 
Node* RandomWNS(Node* state, char& action)
{
	srand((int)time(0));
	int d3 = rand() % 3;
	switch (d3)
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
// INPUT: 
// OUTPUT: 
// DESCRIPTION: 
Node* RandomWNE(Node* state, char& action)
{
	srand((int)time(0));
	int d3 = rand() % 3;
	switch (d3)
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
// INPUT: 
// OUTPUT: 
// DESCRIPTION: 
Node* RandomNES(Node* state, char& action)
{
	srand((int)time(0));
	int d3 = rand() % 3;
	switch (d3)
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
// INPUT: 
// OUTPUT: 
// DESCRIPTION: 
Node* RandomWES(Node* state, char& action)
{
	srand((int)time(0));
	int d3 = rand() % 3;
	switch (d3)
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
// INPUT: 
// OUTPUT: 
// DESCRIPTION: 
Node* RandomWN(Node* state, char& action)
{
	srand((int)time(0));
	int coin = rand() % 2;
	if (coin == 0)
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
// INPUT: 
// OUTPUT: 
// DESCRIPTION: 
Node* RandomWE(Node* state, char& action)
{
	srand((int)time(0));
	int coin = rand() % 2;
	if (coin == 0)
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
// INPUT: 
// OUTPUT: 
// DESCRIPTION: 
Node* RandomWS(Node* state, char& action)
{
	srand((int)time(0));
	int coin = rand() % 2;
	if (coin == 0)
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
// INPUT: 
// OUTPUT: 
// DESCRIPTION: 
Node* RandomNE(Node* state, char& action)
{
	srand((int)time(0));
	int coin = rand() % 2;
	if (coin == 0)
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
// INPUT: 
// OUTPUT: 
// DESCRIPTION: 
Node* RandomNS(Node* state, char& action)
{
	srand((int)time(0));
	int coin = rand() % 2;
	if (coin == 0)
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
// INPUT: 
// OUTPUT: 
// DESCRIPTION: 
Node* RandomES(Node* state, char& action)
{
	srand((int)time(0));
	int coin = rand() % 2;
	if (coin == 0)
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
// INPUT: 
// OUTPUT: 
// DESCRIPTION: 
Node* OptimalTile(Node* state, char& action)
{
	srand((int)time(0));
	int rng = rand() % 100 + 1;
	if ((state->W.qValue == state->N.qValue) &&
		(state->W.qValue == state->S.qValue) &&
		(state->W.qValue > state->E.qValue))
	{
		if (rng <= 95)
			return RandomWNS(state, action);
		else
			return RandomTile(state, action);
	}
	else if ((state->W.qValue == state->N.qValue) &&
		(state->W.qValue == state->E.qValue) &&
		(state->W.qValue > state->S.qValue))
	{
		if (rng <= 95)
			return RandomWNE(state, action);
		else
			return RandomTile(state, action);
	}
	else if ((state->N.qValue == state->E.qValue) &&
		(state->N.qValue == state->S.qValue) &&
		(state->N.qValue > state->W.qValue))
	{
		if (rng <= 95)
			return RandomNES(state, action);
		else
			return RandomTile(state, action);
	}
	else if ((state->W.qValue == state->E.qValue) &&
		(state->W.qValue == state->S.qValue) &&
		(state->W.qValue > state->N.qValue))
	{
		if (rng <= 95)
			return RandomWES(state, action);
		else
			return RandomTile(state, action);
	}
	else if ((state->W.qValue == state->N.qValue) &&
		(state->W.qValue > state->E.qValue) &&
		(state->W.qValue > state->S.qValue))
	{
		if (rng <= 95)
			return RandomWN(state, action);
		else
			return RandomTile(state, action);
	}
	else if ((state->W.qValue == state->E.qValue) &&
		(state->W.qValue > state->N.qValue) &&
		(state->W.qValue > state->S.qValue))
	{
		if (rng <= 95)
			return RandomWE(state, action);
		else
			return RandomTile(state, action);
	}
	else if ((state->W.qValue == state->S.qValue) &&
		(state->W.qValue > state->N.qValue) &&
		(state->W.qValue > state->E.qValue))
	{
		if (rng <= 95)
			return RandomWS(state, action);
		else
			return RandomTile(state, action);
	}
	else if ((state->N.qValue == state->E.qValue) &&
		(state->N.qValue > state->W.qValue) &&
		(state->N.qValue > state->S.qValue))
	{
		if (rng <= 95)
			return RandomNE(state, action);
		else
			return RandomTile(state, action);
	}
	else if ((state->N.qValue == state->S.qValue) &&
		(state->N.qValue > state->W.qValue) &&
		(state->N.qValue > state->E.qValue))
	{
		if (rng <= 95)
			return RandomNS(state, action);
		else
			return RandomTile(state, action);
	}
	else if ((state->E.qValue == state->S.qValue) &&
		(state->E.qValue > state->W.qValue) &&
		(state->E.qValue > state->N.qValue))
	{
		if (rng <= 95)
			return RandomES(state, action);
		else
			return RandomTile(state, action);
	}
	else if ((state->W.qValue > state->N.qValue) &&
		(state->W.qValue > state->E.qValue) &&
		(state->W.qValue > state->S.qValue))
	{
		if (rng <= 95)
		{
			action = 'W';
			return state->prev;
		}
		else
			return RandomTile(state, action);
	}
	else if ((state->N.qValue > state->W.qValue) &&
		(state->N.qValue > state->E.qValue) &&
		(state->N.qValue > state->S.qValue))
	{
		if (rng <= 95)
		{
			action = 'N';
			return state->north;
		}
		else
			return RandomTile(state, action);
	}
	else if ((state->E.qValue > state->W.qValue) &&
		(state->E.qValue > state->N.qValue) &&
		(state->E.qValue > state->S.qValue))
	{
		if (rng <= 95)
		{
			action = 'E';
			return state->next;
		}
		else
			return RandomTile(state, action);
	}
	else if ((state->S.qValue > state->W.qValue) &&
		(state->S.qValue > state->N.qValue) &&
		(state->S.qValue > state->E.qValue))
	{
		if (rng <= 95)
		{
			action = 'S';
			return state->south;
		}
		else
			return RandomTile(state, action);
	}
	else
	{
		cerr << "ERROR: Optimal action was not correctly found!" << endl;
		system("pause");
		exit(0);
	}
}

// AUTHOR: Ethan Puschell
// CREATION DATE: 12-2-20
// LAST MODIFIED: 12-2-20
// INPUT: 
// OUTPUT: 
// DESCRIPTION:
Node* TDrift(Node* state, char& action)
{
	srand((int)time(0));
	int rng = rand() % 100 + 1;
	switch (action)
	{
	case ('W'):
		if (rng <= 70)
			return state->prev;
		else if (rng > 70 && rng <= 85)
		{
			action = 'N';
			return state->north;
		}
		else
		{
			action = 'S';
			return state->south;
		}
		break;
	case('N'):
		if (rng <= 70)
			return state->north;
		else if (rng > 70 && rng <= 85)
		{
			action = 'W';
			return state->prev;
		}
		else
		{
			action = 'E';
			return state->next;
		}
		break;
	case('E'):
		if (rng <= 70)
			return state->next;
		else if (rng > 70 && rng <= 85)
		{
			action = 'N';
			return state->north;
		}
		else
		{
			action = 'S';
			return state->south;
		}
		break;
	case('S'):
		if (rng <= 70)
			return state->south;
		else if (rng > 70 && rng <= 85)
		{
			action = 'W';
			return state->prev;
		}
		else
		{
			action = 'E';
			return state->next;
		}
		break;
	}
}

// AUTHOR: Ethan Puschell
// CREATION DATE: 12-2-20
// LAST MODIFIED: 12-2-20
// INPUT: 
// OUTPUT: 
// DESCRIPTION:
float MaxQSA(Node* nextState)
{
	float optimalAction[4] = { nextState->W.qValue, nextState->N.qValue, nextState->E.qValue, nextState->S.qValue };
	float* optimalCost = max_element(optimalAction, optimalAction + 4);
	if (*optimalCost == nextState->W.qValue)
		return nextState->W.qValue;
	else if (*optimalCost == nextState->N.qValue)
		return nextState->N.qValue;
	else if (*optimalCost == nextState->E.qValue)
		return nextState->E.qValue;
	else if (*optimalCost == nextState->S.qValue)
		return nextState->S.qValue;
}

// AUTHOR: Ethan Puschell
// CREATION DATE: 12-2-20
// LAST MODIFIED: 12-2-20
// INPUT: 
// OUTPUT: 
// DESCRIPTION:
void UpdateNQ(Node* state, Node* nextState, char action)
{
	float nsa, qsa, test;
	switch (action)
	{
	case ('W'):
		state->W.nValue++;
		nsa = (float)state->W.nValue;
		qsa = (float)state->W.qValue;
		state->W.qValue = qsa + ((1.0 / nsa) * (-2.0 + (0.9 * MaxQSA(nextState)) - qsa));
		break;
	case ('N'):
		state->N.nValue++;
		nsa = (float)state->N.nValue;
		qsa = (float)state->N.qValue;
		state->N.qValue = qsa + ((1.0 / nsa) * (-3.0 + (0.9 * MaxQSA(nextState)) - qsa));
		break;
	case ('E'):
		state->E.nValue++;
		nsa = (float)state->E.nValue;
		qsa = (float)state->E.qValue;
		test = MaxQSA(nextState);
		state->E.qValue = qsa + ((1.0 / nsa) * (-2.0 + (0.9 * MaxQSA(nextState)) - qsa));
		break;
	case('S'):
		state->S.nValue++;
		nsa = (float)state->S.nValue;
		qsa = (float)state->S.qValue;
		state->S.qValue = qsa + ((1.0 / nsa) * (-1.0 + (0.9 * MaxQSA(nextState)) - qsa));
		break;
	}
}

// AUTHOR: Ethan Puschell
// CREATION DATE: 12-1-20
// LAST MODIFIED: 12-2-20
// INPUT: 
// OUTPUT: 
// DESCRIPTION: 
void EGreedy(Node* state, int& counter)
{
	if (state->data == 100 || counter == 100)
		return;
	counter++;
	Node* nextState;
	char action;
	//if (state->W.qValue == state->N.qValue == state->E.qValue == state->S.qValue)
	if ((state->N.qValue == state->W.qValue) &&
		(state->N.qValue == state->E.qValue) &&
		(state->N.qValue == state->S.qValue) &&
		(state->S.qValue == state->E.qValue) &&
		(state->W.qValue == state->E.qValue) &&
		(state->W.qValue == state->S.qValue))
		nextState = RandomTile(state, action);
	else
		nextState = OptimalTile(state, action);
	if (nextState == NULL || nextState->data == -1 || state->key.find('1') != string::npos && action == 'W' || state->key.find('5') != string::npos && action == 'E')
		nextState = state;
	nextState = TDrift(state, action);
	if (nextState == NULL || nextState->data == -1 || state->key.find('1') != string::npos && action == 'W' || state->key.find('5') != string::npos && action == 'E')
		nextState = state;
	UpdateNQ(state, nextState, action);
	EGreedy(nextState, counter);
}

// AUTHOR: Christopher Bourn
// CREATION DATE: 12-2-20
// LAST MODIFIED: 12-3-20
// INPUT: 
// OUTPUT: 
// DESCRIPTION:
string optimalPath(struct Node* state) {
	string msg = "";
	float optimalDirection = 0;
	float optimalPath[4] = { state->W.nValue, state->N.nValue, state->E.nValue, state->S.nValue };
	
	if (state->key == "c3") {
		msg = "+100";
		return msg;
	}
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
// INPUT: 
// OUTPUT: 
// DESCRIPTION:
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
	srand((int)time(0));
	Node* head = NULL;
	int counter = 0;
	fillGrid((&head));
	printKey(head);
	cout << endl << endl;

	for (int i = 0; i < 10000; i++)
	{
		counter = 0;
		EGreedy(RandomStart(head), counter);
	}
	printNList(head);
	printQList(head);
	printPath(head);

	system("pause");
	return 0;
}
