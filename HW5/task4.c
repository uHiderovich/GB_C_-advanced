/*
Задание 4. Удаление элементов
Напишите программу, которая удаляет из списка все элементы с нечетным
порядковым номером.
Пример №1
Данные на входе: aa ss dd ff gg hh jj kk ll.
Данные на выходе: aa dd gg jj ll
Пример №2
Данные на входе: aa ss dd ff gg hh jj kk ll tt.
Данные на выходе: aa dd gg jj ll
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define WORD_SIZE 1000

typedef struct Node {
  char* value;
  struct Node* next;
} Node;

Node* createNode(const char* value) {
  Node* newNode = (Node*)malloc(sizeof(Node));
  newNode->value = strdup(value);
  newNode->next = NULL;
  return newNode;
}

void insert(Node** root, const char* value) {
  Node* newNode = createNode(value);
  if (*root == NULL) {
    *root = newNode;
  }
  else {
    Node* current = *root;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = newNode;
  }
}

void printList(Node* list) {
  if (list == NULL) {
    printf("List is empty.\n");
    return;
  }

  Node* current = list;

  while (current->next != NULL) {
    printf("%s ", current->value);
    current = current->next;
  }
  printf("%s.\n", current->value);
}

Node* extractEvenElements(Node** root) {
  if (*root == NULL) {
    return NULL;
  }

  Node* current = *root;
  Node* prev = NULL;
  Node* newList = NULL;
  int index = 1;

  while (current != NULL) {
    if (index % 2 != 0) {
      Node* temp = current;

      if (prev == NULL) {
        *root = current->next;
      }
      else {
        prev->next = current->next;
      }

      current = current->next;
      temp->next = NULL;
      insert(&newList, temp->value);
      free(temp->value);
      free(temp);
    }
    else {
      prev = current;
      current = current->next;
    }
    index++;
  }

  return newList;
}


int main() {
  Node* list = NULL;
  Node* evenList = NULL;
  char word[WORD_SIZE] = "";

  char ch = ' ';
  int i = 0;

  while (EOF != (ch = getchar())) {
    switch (ch) {
    case '.':
    case ' ':
      word[i] = '\0';
      insert(&list, word);
      i = 0;
      break;
    default:
      word[i++] = ch;
      if (i >= WORD_SIZE) {
        i = 0;
      }
      break;
    }

    if (ch == '.') {
      break;
    }
  }

  printf("\n");

  evenList = extractEvenElements(&list);
  printf("Only odd: \n");
  printList(evenList);
  printf("\n");

  printf("The remaining even: \n");
  printList(list);

  return 0;
}