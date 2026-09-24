#include <stdio.h>
#include <stdlib.h>
#include "double_linkedlist.h"


Node* Create_Node(int value) {
  Node* n = (Node*)malloc(sizeof(Node));
  n->val = value;
  n->prev = NULL;
  n->next = NULL;

  return n;
}

void Insert_Tail_Node(Node **head, int value) {
  Node* new_node = Create_Node(value);
  if(*head == NULL) {
    new_node->next = new_node;
    new_node->prev = new_node;
    *head = new_node;
    // printf("added %d\n", value);
    return;
  }

  Node* tmp = *head;
  while(tmp->next != NULL && tmp->next != *head) {
    tmp = tmp->next;
  }
  tmp->next = new_node;
  new_node->prev = tmp;

  // make it cyclic
  new_node->next = *head;
  (*head)->prev = new_node;
  // printf("added %d\n", value);
}

void Delete_Node_Position(Node** head, int position) {
  // printf("START: deletion of POSITION %d\n", position);
  if(*head == NULL) {
    printf("delete node failed: alrady empty list\n");
    return;
  }

  Node* tmp = *head;
  for(int i = 0; tmp != NULL && i < position; i++) {
    //looped one way. I guess it can be a feature but seems too risky
    if(i != 0 && tmp == *head) {
      printf("error: position is greater than list length\n");
    }
    tmp = tmp->next;

  }
  // printf("im at position %d and I got value %d\n", position, tmp->val);
  // printf("my prev is %d and my next is %d\n", tmp->prev->val, tmp->next->val);

  if(tmp->next == tmp) { // when removing only last value
    *head = NULL;
  } else {  // when values still exist after deletion
    tmp->prev->next = tmp->next;
    tmp->next->prev = tmp->prev;
    if(tmp == *head) {
      *head = tmp->next;
    }
  }
  // printf("removed pos %d\n", position);
  free(tmp);
}

void Delete_Node_Value(Node **head, int value) {
  // printf("START: deletion of VALUE %d\n", value);
  if(*head == NULL) {
    printf("delete node failed: alrady empty list\n");
    return;
  }

  Node* tmp = *head;
  for(int i = 0; tmp != NULL; i++) {
    //looped one way. I guess it can be a feature but seems too risky
    if(i != 0 && tmp == *head) {
      printf("error: value not found in list\n");
      break;
    }
    tmp = tmp->next;

    if(tmp->val == value) {
      break;
    }
  }

  // printf("I got value %d\n", tmp->val);
  // printf("my prev is %d and my next is %d\n", tmp->prev->val, tmp->next->val);

  if(tmp->next == tmp) { // when removing only last value
    *head = NULL;
  } else {  // when values still exist after deletion
    tmp->prev->next = tmp->next;
    tmp->next->prev = tmp->prev;
    if(tmp == *head) {
      *head = tmp->next;
    }
  }

  // printf("removed val %d\n", value);
  free(tmp);
}


void Print_List(Node* head) {
  int length = 0;
  if(head == NULL) {
    printf("length %d\n", length);
    return;
  }
  length++;
  printf("%d ", head->val);
  Node* temp = head->next;
  while(temp != NULL && temp != head) {
    printf("%d ", temp->val);
    temp = temp->next;
    length++;
  }
  printf("\n");
  if(temp == NULL) {
    printf("reached NULL Node\n");
  } else if(temp == head) {
    // printf("reached head with value %d\n", temp->val);
  }
  printf("length %d\n", length);
}
