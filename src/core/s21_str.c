#include "../s21_string.h"

s21_size_t s21_strlen(const char *str) {
  s21_size_t len = 0;
  // Защита от передачи нулевого указателя
  if (str != S21_NULL) {
    while (str[len] != '\0') {
      len++;
    }
  }
  return len;
}

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  s21_size_t i = 0;
  if (src != S21_NULL || dest != S21_NULL) {
    while (i < n && src[i] != '\0') {
      dest[i] = src[i];
      i++;
    }

    // Заполняем оставшиеся байты нулями, если src короче, чем n
    while (i < n) {
      dest[i] = '\0';
      i++;
    }
  }
  return dest;
}

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  if (dest != S21_NULL || src != S21_NULL || n != 0) {
    s21_size_t dest_len = s21_strlen(dest);
    s21_size_t i = 0;

    while (i < n && src[i] != '\0') {
      dest[dest_len + i] = src[i];
      i++;
    }

    dest[dest_len + i] = '\0';
  }

  return dest;
}

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  s21_size_t i = 0;
  int dif = 0;

  if (n != 0) {
    if (str1 == S21_NULL || str2 == S21_NULL) {
      if (str1 != str2) dif = (str1 == S21_NULL) ? -1 : 1;
    } else {
      while (i < n - 1 && str1[i] == str2[i] && str1[i] != '\0') {
        i++;
      }

      dif = (unsigned char)str1[i] - (unsigned char)str2[i];
    }
  }

  return dif;
}

char *s21_strchr(const char *str, int c) {
  char *ch = S21_NULL;

  if (str != S21_NULL) {
    s21_size_t i = 0;
    char target = (char)c;
    int stop = 1;
    do {
      if (str[i] == target) {
        ch = (char *)&str[i];
        stop = 0;
      }
      if (str[i] == '\0') stop = 0;
      if (stop) i++;
    } while (stop);
  }
  return ch;
}

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t i = 0;
  int stop = 0;
  // Защита от передачи пустых указателей
  if (str1 != S21_NULL && str2 != S21_NULL) {
    while (stop != 1 && str1[i] != '\0') {
      s21_size_t j = 0;

      while (stop != 1 && str2[j] != '\0') {
        if (str1[i] == str2[j]) stop = 1;
        j++;
      }
      if (stop != 1) i++;
    }
  }
  return i;
}

char *s21_strpbrk(const char *str1, const char *str2) {
  char *ch = S21_NULL;
  // Защита от передачи пустых указателей
  if (str1 != S21_NULL && str2 != S21_NULL) {
    s21_size_t i = 0;
    int stop = 0;

    while (stop != 1 && str1[i] != '\0') {
      s21_size_t j = 0;

      while (stop != 1 && str2[j] != '\0') {
        if (str1[i] == str2[j]) {
          stop = 1;
          break;
        }
        j++;
      }
      if (stop != 1)
        i++;
      else
        ch = (char *)&str1[i];
    }
  }
  return ch;
}

char *s21_strstr(const char *haystack, const char *needle) {
  char *ch = S21_NULL;
  int stop = 0;
  // Защита от передачи пустых указателей
  if (haystack != S21_NULL && needle != S21_NULL) {
    if (needle[0] == '\0') {
      ch = (char *)haystack;
      stop = 1;
    }

    s21_size_t i = 0;
    while (stop != 1 && haystack[i] != '\0') {
      s21_size_t j = 0;

      while (needle[j] != '\0' && haystack[i + j] != '\0' &&
             haystack[i + j] == needle[j])
        j++;
      if (needle[j] == '\0') {
        ch = (char *)&haystack[i];
        stop = 1;
      }
      i++;
    }
  }

  return ch;
}

s21_size_t s21_strspn(const char *str1, const char *str2) {
  s21_size_t i = 0;
  while (str1[i] != '\0' && s21_strchr(str2, str1[i]) != S21_NULL) i++;
  return i;
}

char *s21_strtok(char *str, const char *delim) {
  static char *last_token = S21_NULL;
  char *token_start = S21_NULL;
  int stop = 0;

  if (delim == S21_NULL) stop = 1;

  if (stop != 1) {
    if (str != S21_NULL)
      last_token = str;
    else if (last_token == S21_NULL)
      stop = 1;
  }

  if (stop != 1) {
    last_token += s21_strspn(last_token, delim);
    if (*last_token == '\0') {
      last_token = S21_NULL;
      stop = 1;
    }
  }

  if (stop != 1) {
    token_start = last_token;
    last_token += s21_strcspn(last_token, delim);
    if (*last_token != '\0') {
      *last_token = '\0';
      last_token++;
    } else
      last_token = S21_NULL;
  }
  return token_start;
}

char *s21_strrchr(const char *str, int c) {
  char *result = S21_NULL;

  if (str != S21_NULL) {
    s21_size_t len = s21_strlen(str);
    int stop = 0;

    while (stop != 1) {
      if (str[len] == (char)c) {
        result = (char *)&str[len];
        stop = 1;
      }
      if (len == 0)
        stop = 1;
      else if (stop != 1)
        len--;
    }
  }
  return result;
}