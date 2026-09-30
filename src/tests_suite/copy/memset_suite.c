#include "memset_suite.h"

#include <string.h>

#include "../../s21_string.h"

START_TEST(test_memset_normal) {
  char str1[20] = "Hello";
  char str2[20] = "Hello";
  s21_memset(str1, '*', 3);
  memset(str2, '*', 3);
  ck_assert_str_eq(str1, str2);
}
END_TEST

Suite *memset_suite(void) {
  Suite *s = suite_create("memset");
  TCase *tc = tcase_create("core");
  tcase_add_test(tc, test_memset_normal);
  suite_add_tcase(s, tc);
  return s;
}