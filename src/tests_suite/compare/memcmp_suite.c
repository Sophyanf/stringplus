#include "memcmp_suite.h"

#include <string.h>

#include "../../s21_string.h"

START_TEST(test_memcmp_equal) {
  ck_assert_int_eq(s21_memcmp("abc", "abc", 3), memcmp("abc", "abc", 3));
}
END_TEST

START_TEST(test_memcmp_different) {
  ck_assert_int_eq(s21_memcmp("abc", "abd", 3), memcmp("abc", "abd", 3));
}
END_TEST

START_TEST(test_memcmp_zero_n) {
  ck_assert_int_eq(s21_memcmp("abc", "def", 0), memcmp("abc", "def", 0));
}
END_TEST

Suite *memcmp_suite(void) {
  Suite *s = suite_create("memcmp");
  TCase *tc = tcase_create("core");
  tcase_add_test(tc, test_memcmp_equal);
  tcase_add_test(tc, test_memcmp_different);
  tcase_add_test(tc, test_memcmp_zero_n);
  suite_add_tcase(s, tc);
  return s;
}