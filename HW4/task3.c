/*
Задание 3. Префикс и суффикс
Реализовать программу 3-го практического задания более эффективным
способом со сложностью О(n) (см. код в лекции).
Пример №1
Данные на вход:
don't_panic
nick_is_a_mastodon
Данные на выход: 3
Пример №2
Данные на вход:
monty_python
python_has_list_comprehensions
Данные на выход: 6
*/

#include <stdio.h>
#include <string.h>

#define MAXLENGTH 10001

void ComputePrefix(const char* str, int* pi, int n) {
  int k = 0;
  pi[0] = 0;

  for (int i = 1; i < n; i++) {
    while (k > 0 && str[k] != str[i]) {
      k = pi[k - 1];
    }
    if (str[k] == str[i]) {
      k++;
    }
    pi[i] = k;
  }
}

int SearchPrefix(const char* s1, const char* s2, int isPrefix) {
  int l1 = strlen(s1);
  int l2 = strlen(s2);
  int pi[MAXLENGTH * 2];
  char combined[MAXLENGTH * 2 + 1];

  if (isPrefix) {
    snprintf(combined, sizeof(combined), "%s#%s", s1, s2);
  }
  else {
    snprintf(combined, sizeof(combined), "%s#%s", s2, s1);
  }

  ComputePrefix(combined, pi, l1 + l2 + 1);

  return pi[l1 + l2];
}

int main() {
  char s1[MAXLENGTH], s2[MAXLENGTH];
  scanf("%10000s", s1);
  scanf("%10000s", s2);

  printf("%d %d\n", SearchPrefix(s1, s2, 1), SearchPrefix(s1, s2, 0));

  return 0;
}

