/*
Задание 2. Сумма всех элементов
Напишите программу, которая возвращает сумму всех элементов в дереве.
Данные на выходе: 78
*/

#include <stdio.h>
#include <stdlib.h>


typedef struct Node {
  int value;
  struct Node* left;
  struct Node* right;
} Node;

Node* createNode(int data) {
  Node* newNode = (Node*)malloc(sizeof(Node));
  newNode->value = data;
  newNode->left = NULL;
  newNode->right = NULL;
  return newNode;
}

int sumTree(Node* root) {
  if (root == NULL) {
    return 0;
  }
  return root->value + sumTree(root->left) + sumTree(root->right);
}

int main() {
  Node* root = createNode(10);
  root->left = createNode(5);
  root->right = createNode(15);
  root->left->left = createNode(3);
  root->left->right = createNode(7);
  root->right->left = createNode(13);
  root->right->right = createNode(18);
  root->left->left->left = createNode(1);
  root->left->right->left = createNode(6);

  int sum = sumTree(root);
  printf("Sum: %d\n", sum);

  return 0;
}
