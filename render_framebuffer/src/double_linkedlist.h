#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H

// thought about using sys/queue.h or ulist.h for this but
// seems to complicate the portability to windows
// all i need is insert and delete and loop through
// should be simple enough to make it myself then

typedef struct Node { //note: Node is written here too to be used in the pointers below
  int val;
  struct Node* prev;
  struct Node* next;
} Node;

Node* Create_Node(int value);
void Insert_Tail_Node(Node** head, int value);
void Delete_Node_Position(Node** head, int position);
void Delete_Node_Value(Node** head, int value); // since no Node will have same value IN THIS EXAMPLE OF USE.
void Print_List(Node* head);

#endif