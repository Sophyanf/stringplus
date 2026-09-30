#include <stdint.h>
#include <stdlib.h>

#include "s21_string.h"
typedef char (*s21_char_transform_fn)(char c);
static char s21_upper_char(char c) {
  char out = c;
  if (c >= 'a' && c <= 'z') {
    out = (char)(c - 'a' + 'A');
  }
  return out;
}
static char s21_lower_char(char c) {
  char out = c;
  if (c >= 'A' && c <= 'Z') {
    out = (char)(c - 'A' + 'a');
  }
  return out;
}
static s21_size_t s21_len(const char *str) {
  s21_size_t len = 0;
  while (str[len] != '\0') {
    len++;
  }
  return len;
}
static int s21_char_in_set(char c, const char *set) {
  int found = 0;
  s21_size_t i = 0;
  while (set[i] != '\0' && found == 0) {
    if (set[i] == c) {
      found = 1;
    }
    i++;
  }
  return found;
}
static void *s21_apply_char_fn(const char *str, s21_char_transform_fn fn) {
  char *result = S21_NULL;
  if (str != S21_NULL) {
    s21_size_t len = s21_len(str);
    if (len < (s21_size_t)-1) {
      result = malloc((len + 1) * sizeof(*result));
      if (result != S21_NULL) {
        s21_size_t i = 0;
        while (i < len) {
          result[i] = fn(str[i]);
          i++;
        }
        result[len] = '\0';
      }
    }
  }
  return result;
}
void *s21_to_upper(const char *str) {
  void *result = S21_NULL;
  result = s21_apply_char_fn(str, s21_upper_char);
  return result;
}
void *s21_to_lower(const char *str) {
  void *result = S21_NULL;
  result = s21_apply_char_fn(str, s21_lower_char);
  return result;
}
void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  char *result = S21_NULL;
  if (src != S21_NULL && str != S21_NULL) {
    s21_size_t src_len = s21_len(src);
    s21_size_t str_len = s21_len(str);
    if (start_index <= src_len && str_len <= (s21_size_t)-1 - src_len - 1) {
      result = malloc((src_len + str_len + 1) * sizeof(*result));
      if (result != S21_NULL) {
        s21_size_t i = 0;
        while (i < start_index) {
          result[i] = src[i];
          i++;
        }
        i = 0;
        while (i < str_len) {
          result[start_index + i] = str[i];
          i++;
        }
        i = start_index;
        while (i < src_len) {
          result[str_len + i] = src[i];
          i++;
        }
        result[src_len + str_len] = '\0';
      }
    }
  }
  return result;
}
void *s21_trim(const char *src, const char *trim_chars) {
  char *result = S21_NULL;
  static const char kDefaultTrimSet[] = " \t\n\v\f\r";
  if (src != S21_NULL) {
    const char *set = (trim_chars != S21_NULL) ? trim_chars : kDefaultTrimSet;
    s21_size_t src_len = s21_len(src);
    s21_size_t start = 0;
    s21_size_t end = src_len;
    while (start < end && s21_char_in_set(src[start], set)) {
      start++;
    }
    while (end > start && s21_char_in_set(src[end - 1], set)) {
      end--;
    }
    if ((end - start) < (s21_size_t)-1) {
      s21_size_t out_len = end - start;
      result = malloc((out_len + 1) * sizeof(*result));
      if (result != S21_NULL) {
        s21_size_t i = 0;
        while (i < out_len) {
          result[i] = src[start + i];
          i++;
        }
        result[out_len] = '\0';
      }
    }
  }
  return result;
}