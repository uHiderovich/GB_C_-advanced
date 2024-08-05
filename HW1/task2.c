/*
Задание 2. Инверсия старших

На вход программе подается беззнаковое 32-битное целое число N. Требуется изменить значения всех битов старшего байта числа на противоположные и вывести полученное таким образом число.

Данные на входе: Беззнаковое 32-битное целое число N
Данные на выходе: Одно целое число.

Пример №1
Данные на входе: 1
Данные на выходе: 4278190081

Пример №2
Данные на входе: 4278190081
Данные на выходе: 1
*/

#include <stdio.h>
#include <stdint.h>

#define BITS_COUNT 8
#define DIFF 32 - BITS_COUNT

int main() {
  uint32_t N, res, invertedHighByte, restOfNumber;

  scanf("%u", &N);

  invertedHighByte = (~(N >> DIFF) << DIFF);
  restOfNumber = ((N << BITS_COUNT) >> BITS_COUNT);

  res = (invertedHighByte | restOfNumber);

  printf("%u\n", res);

  return 0;
}
