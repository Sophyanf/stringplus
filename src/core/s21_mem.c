#include "../s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  const unsigned char *s = (const unsigned char *)str;
  unsigned char uc = (unsigned char)c;
  for (s21_size_t i = 0; i < n; i++) {
    if (s[i] == uc) return (void *)(s + i);
  }
  return S21_NULL;
}

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  const unsigned char *s1 = (const unsigned char *)str1;
  const unsigned char *s2 = (const unsigned char *)str2;
  for (s21_size_t i = 0; i < n; i++) {
    if (s1[i] != s2[i]) return s1[i] - s2[i];
  }
  return 0;
}

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  unsigned char *d = (unsigned char *)dest;
  const unsigned char *s = (const unsigned char *)src;
  for (s21_size_t i = 0; i < n; i++) d[i] = s[i];
  return dest;
}

void *s21_memset(void *str, int c, s21_size_t n) {
  unsigned char *s = (unsigned char *)str;
  unsigned char uc = (unsigned char)c;
  for (s21_size_t i = 0; i < n; i++) s[i] = uc;
  return str;
}