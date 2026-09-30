// Эти 4 прототипа нужно добавить в существующий s21_string.h
// (туда же, где уже объявлены s21_size_t и S21_NULL из части 1).
// Отдельным файлом не оформляю по условию: "must be placed in
// the s21_string.h library".

#ifndef S21_STRING_PLUS_DECL_H
#define S21_STRING_PLUS_DECL_H
#include "s21_string.h"
void *s21_to_upper(const char *str);
void *s21_to_lower(const char *str);
void *s21_insert(const char *src, const char *str, s21_size_t start_index);
void *s21_trim(const char *src, const char *trim_chars);
#endif
