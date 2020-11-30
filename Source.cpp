//Created 11/24/2020
//Test Edit Chris Bourn
//Test Edit number 2 by Chris Bourn

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;


struct Node {
    float data = 0.0;
    string key = "####";
    struct Node* next = NULL;
    struct Node* prev = NULL;
    struct Node* north = NULL;
    struct Node* south = NULL;
    float W, N, E, S = 0.0;
};
void append(struct Node** head_ref, float new_data, string new_key)
{
    struct Node* new_node = new Node;

    struct Node* last = *head_ref;

    new_node->data = new_data;

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
Node* locate(struct Node* node, string s) {
    struct Node* last{};
    while (node != NULL) {

        if (node->key == s)
            return node;

        last = node;
        node = node->next;
    }
}
/*2nd function finds the node above or below a node*/
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
void printList(struct Node* node)
{
    cout << left;
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
            cout << setw(8) << fixed << setprecision(2) << node->data << " ";
        last = node;
        node = node->next;
        counter++;
    }
}
void printKey(struct Node* node) {
    cout << left;
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

int main() {

    Node* head = NULL;
    string msg = "";
    int counter = 1;

    /*Fill grid*/
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


        if (i == 6 || i == 7 || i == 11 || i == 16 || i == 17 || i == 21) {
            append(&head, -1, msg + to_string(counter));
        }
        else
            append(&head, 0.0, msg + to_string(counter));
        if (counter == 5) {
            counter = 1;
        }
        else
            counter++;
    }
    /*Make the doubly linked list a quadruply linked list by connecting the north and south nodes*/
    reConnectGrid(head, head);
    printKey(head);
    cout << endl << endl;
    printList(head);
}
