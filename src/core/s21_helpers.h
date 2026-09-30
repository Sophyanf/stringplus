#ifndef S21_HELPERS_H
#define S21_HELPERS_H
#include <stdint.h>

#include "s21_string.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Структура для разбора спецификатора формата */
typedef struct {
  int flag_minus;
  int flag_plus;
  int flag_space;
  int flag_hash;
  int flag_zero;
  int width;
  int has_precision;
  int precision;
  char length;
  char specifier;
} FormatSpec;

/* ── Символы ─────────────────────────────────────────────────────────── */

int s21_is_digit(char c);
int s21_is_space(char c);

/* ── Числа → строки ──────────────────────────────────────────────────── */

int s21_uint_to_str(unsigned long long value, int base, int uppercase,
                    char *buf);
int s21_int_to_str(long long value, char *buf);

/* ── Строковые примитивы ─────────────────────────────────────────────── */

int s21_copy_n(char *dst, const char *src, int n);
int s21_fill_n(char *dst, char c, int n);

/* ── Парсинг ─────────────────────────────────────────────────────────── */

int s21_parse_uint(const char *s, int *out);

#ifdef __cplusplus
}
#endif

#endif /* S21_HELPERS_H */
