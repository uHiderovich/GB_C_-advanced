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

long long binomialCoefficient(int n, int k) {
  if (k > n - k) {
    k = n - k;
  }

  long long result = 1;
  for (int i = 0; i < k; ++i) {
    result = result * (n - i) / (i + 1);
  }
  return result;
}

int main() {
  setlocale(LC_ALL, "Rus");

  int N, M;
  scanf("Введите два целых числа N и K:  %d %d", &N, &M);

  long long result = binomialCoefficient(N, M);

  printf("Результат: %lld\n", result);

  return 0;
}