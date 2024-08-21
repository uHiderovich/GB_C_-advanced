/*
Задание 1. Биномиальный коэффициент
Реализовать программу 1-го практического задания с помощью
биномиального коэффициента.
Данные на вход: Два целых числа N и M
Данные на выход: Одно целое число – количество вариантов
Пример №1
Данные на вход: 10 5
Данные на выход: 252
Пример №2
Данные на вход: 35 5
Данные на выход: 324632
Пример №3
Данные на вход: 40 10
Данные на выход: 847660528
*/

#include <locale.h>
#include <stdio.h>

int binomialCoefficient(int n, int k) {
  const int maxN = n;
  int C[maxN + 1][maxN + 1];

  for (int i = 0; i <= maxN; i++) {
    C[i][0] = C[i][i] = 1;
    for (int j = 1; j < i; j++) {
      C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
    }
  }

  return C[n][k];
}

int main() {
  setlocale(LC_ALL, "Rus");

  int n, k;
  scanf("%d %d", &n, &k);

  printf("Результат: %d\n", binomialCoefficient(n, k));

  return 0;
}