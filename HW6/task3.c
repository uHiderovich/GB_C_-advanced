/*
Задание 3. Разница между элементами
Напишите программу, которая находит разницу между максимальным и
минимальным элементом в дереве.
Данные на выходе: 17
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h> 

typedef struct Node {
  int data;
  struct Node* left;
  struct Node* right;
} Node;

Node* createNode(int data) {
  Node* newNode = (Node*)malloc(sizeof(Node));
  newNode->data = data;
  newNode->left = NULL;
  newNode->right = NULL;
  return newNode;
}

int findMin(Node* root) {
  if (root == NULL) {
    return INT_MAX;
  }
  int minLeft = findMin(root->left);
  int minRight = findMin(root->right);
  return root->data < minLeft && root->data < minRight ? root->data : (minLeft < minRight ? minLeft : minRight);
}

int findMax(Node* root) {
  if (root == NULL) {
    return INT_MIN;
  }
  int maxLeft = findMax(root->left);
  int maxRight = findMax(root->right);
  return root->data > maxLeft && root->data > maxRight ? root->data : (maxLeft > maxRight ? maxLeft : maxRight);
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

  int min = findMin(root);
  int max = findMax(root);

  int difference = max - min;

  printf("Min: %d\n", min);
  printf("Max: %d\n", max);
  printf("Diff: %d\n", difference);

  return 0;
}
