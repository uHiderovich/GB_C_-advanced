/*
Задание 2. Обратная польская запись
Написать алгоритм перевода из инфиксной записи в обратную польскую. Для
его реализации нужны данные по приоритетам операций. Реализовать
алгоритм, используя побитовые операции (&, |, ^).
Пример №1
Данные на вход: ( 3 & 5 ) ^ 4 | 9
Данные на выход: 3 5 & 4 ^ 9 |
Пример №2
Данные на вход: 8 ^ 2 & 5 | ( 6 & 1 )
Данные на выход: 8 2 5 & ^ 6 1 & |
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>

#define MAX 100

int precedence(char operator) {
  switch (operator) {
  case '|': return 1;
  case '^': return 2;
  case '&': return 3;
  default: return 0;
  }
}

void infixToPostfix(const char* expression, char* output) {
  char stack[MAX];
  int top = -1;
  int i, k = 0;

  for (i = 0; expression[i]; i++) {
    if (isspace(expression[i])) continue;

    if (isdigit(expression[i])) {
      output[k++] = expression[i++];
      output[k++] = ' ';
      i--;
    }
    else if (expression[i] == '(') {
      stack[++top] = expression[i];
    }
    else if (expression[i] == ')') {
      while (top >= 0 && stack[top] != '(') {
        output[k++] = stack[top--];
        output[k++] = ' ';
      }
      top--;
    }
    else {
      while (top >= 0 && precedence(stack[top]) >= precedence(expression[i])) {
        output[k++] = stack[top--];
        output[k++] = ' ';
      }
      stack[++top] = expression[i];
    }
  }

  while (top >= 0) {
    output[k++] = stack[top--];
    output[k++] = ' ';
  }

  output[k - 1] = '\0';
}

int main() {
  setlocale(LC_ALL, "Rus");

  char expression[MAX];
  char output[MAX];

  fgets(expression, MAX, stdin);

  infixToPostfix(&expression, &output);

  printf("Результат: %s\n", output);

  return 0;
}

