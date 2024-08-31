/*
Задание 1. Поиск первого и последнего слова
Используя код задачи 1, написать программу поиска первого и последнего
слова в строке.
Пример №1
Данные на входе: efg abcd zzz.
Данные на выходе: efg zzz.
Пример №2
Данные на входе: fffff kkkkkk a.
Данные на выходе: fffff a.
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

void printFirstAndLast(Node* list) {
  Node* current = list;
  if (list == NULL) {
    printf("List is empty.\n");
    return;
  }

  printf("%s ", current->value);
  while (current->next != NULL) {
    current = current->next;
  }
  printf("%s.\n", current->value);
}

int main() {
  Node* list = NULL;
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
      printFirstAndLast(list);
      break;
    }
  }

  return 0;
}
