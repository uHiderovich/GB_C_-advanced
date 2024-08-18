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

#define MAXLENGTH 104

void ZFunction(const char* s, int* z, int n) {
  int l = 0, r = 0;
  z[0] = n;
  for (int i = 1; i < n; i++) {
    if (i <= r) {
      z[i] = (r - i + 1) < z[i - l] ? (r - i + 1) : z[i - l];
    }
    while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
      z[i]++;
    }
    if (i + z[i] - 1 > r) {
      l = i;
      r = i + z[i] - 1;
    }
  }
}

int SearchPrefixSuffix(const char* s1, const char* s2, int isPrefix) {
  int l1 = strlen(s1);
  int l2 = strlen(s2);
  int z[MAXLENGTH * 2 + 1];

  char combined[MAXLENGTH * 2 + 1];

  if (isPrefix) {
    snprintf(combined, sizeof(combined), "%s#%s", s2, s1);
    ZFunction(combined, z, l1 + l2 + 1);

    return z[l2 + 1];
  }
  else {
    snprintf(combined, sizeof(combined), "%s#%s", s1, s2);
    ZFunction(combined, z, l1 + l2 + 1);

    int maxZ = 0;

    for (int i = l1 + 1; i < l1 + l2 + 1; i++) {
      if (z[i] == l1 + l2 - i + 1) {
        maxZ = z[i];
      }
    }
    return maxZ;
  }
}

int main() {
  char s1[MAXLENGTH], s2[MAXLENGTH];

  scanf("%s", s1);
  scanf("%s", s2);

  printf("%d %d\n", SearchPrefixSuffix(s1, s2, 1), SearchPrefixSuffix(s1, s2, 0));

  return 0;
}

