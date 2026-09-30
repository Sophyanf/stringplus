/*
 * s21_helpers.c — реализация внутренних вспомогательных функций.
 *
 * Не компилируется напрямую пользователем — включается через Makefile
 * как часть статической библиотеки s21_string.a.
 */

#include "s21_helpers.h"

/* ── Символы ─────────────────────────────────────────────────────────── */

/* Возвращает 1, если символ является десятичной цифрой '0'..'9'. */
int s21_is_digit(char c) { return c >= '0' && c <= '9'; }

/* Возвращает 1, если символ является пробельным по стандарту C. */
int s21_is_space(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' ||
         c == '\v';
}

/* ── Числа → строки ──────────────────────────────────────────────────── */

/*
 * s21_uint_to_str — перевод беззнакового числа в строку.
 *
 * Алгоритм: делим `value` на `base`, записываем остатки в обратном порядке,
 * затем разворачиваем строку.
 *
 * Параметры:
 *   value     — число для преобразования
 *   base      — основание (8, 10, 16)
 *   uppercase — 1 → 'A'-'F', 0 → 'a'-'f'
 *   buf       — выходной буфер (минимум 66 байт)
 *
 * Возвращает длину результирующей строки.
 */
int s21_uint_to_str(unsigned long long value, int base, int uppercase,
                    char *buf) {
  /* Набор цифр для нужного регистра */
  const char *digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";

  int len = 0;

  /* Особый случай: само число равно 0 */
  if (value == 0) {
    buf[len++] = '0';
    buf[len] = '\0';
    return len;
  }

  /* Записываем цифры в обратном порядке */
  while (value > 0) {
    buf[len++] = digits[value % (unsigned)base];
    value /= (unsigned)base;
  }
  buf[len] = '\0';

  /* Разворачиваем строку (reverse) */
  for (int i = 0, j = len - 1; i < j; i++, j--) {
    char tmp = buf[i];
    buf[i] = buf[j];
    buf[j] = tmp;
  }

  return len;
}

/*
 * s21_int_to_str — перевод знакового числа (base 10) в строку.
 *
 * Обрабатывает LLONG_MIN отдельно, чтобы избежать переполнения при
 * взятии унарного минуса.
 *
 * Возвращает длину результирующей строки.
 */
int s21_int_to_str(long long value, char *buf) {
  int len = 0;

  if (value < 0) {
    buf[len++] = '-';

    /* Для LLONG_MIN (-9223372036854775808) -value переполняется,
     * поэтому первую цифру снимаем до смены знака. */
    unsigned long long uval;
    uval = (unsigned long long)(-(value + 1)) + 1ULL;

    /* Пишем цифры беззнаковой части начиная с позиции len */
    int digits_len = s21_uint_to_str(uval, 10, 0, buf + len);
    len += digits_len;
  } else {
    len = s21_uint_to_str((unsigned long long)value, 10, 0, buf);
  }

  return len;
}

/* ── Строковые примитивы ─────────────────────────────────────────────── */

/*
 * s21_copy_n — копирует ровно `n` байт из `src` в `dst`.
 * '\0' не добавляется. Безопасна при n == 0.
 * Возвращает `n`.
 */
int s21_copy_n(char *dst, const char *src, int n) {
  for (int i = 0; i < n; i++) dst[i] = src[i];
  return n;
}

/*
 * s21_fill_n — заполняет `dst` символом `c` ровно `n` раз.
 * Возвращает `n`.
 */
int s21_fill_n(char *dst, char c, int n) {
  for (int i = 0; i < n; i++) dst[i] = c;
  return n;
}

/* ── Парсинг ─────────────────────────────────────────────────────────── */

/*
 * s21_parse_uint — читает десятичное неотрицательное целое из строки.
 *
 * Останавливается при первом нецифровом символе.
 * Записывает результат в *out (может быть S21_NULL, тогда только
 * считаются символы).
 *
 * Возвращает количество прочитанных символов (0 если цифр не было).
 */
int s21_parse_uint(const char *s, int *out) {
  int count = 0;
  int result = 0;

  while (s21_is_digit(s[count])) {
    result = result * 10 + (s[count] - '0');
    count++;
  }

  if (out != S21_NULL) *out = result;
  return count;
}
