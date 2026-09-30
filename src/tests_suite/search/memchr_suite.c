#include "memchr_suite.h"

#include <string.h>

#include "../../s21_string.h"

START_TEST(test_memchr_basic) {
  char str[] = "Hello, world!";
  ck_assert_ptr_eq(s21_memchr(str, 'o', 13), memchr(str, 'o', 13));
  ck_assert_ptr_eq(s21_memchr(str, 'z', 13), memchr(str, 'z', 13));
  ck_assert_ptr_eq(s21_memchr(str, 'H', 1), memchr(str, 'H', 1));
}
END_TEST

START_TEST(test_memchr_zero_n) {
  char str[] = "test";
  ck_assert_ptr_eq(s21_memchr(str, 't', 0), memchr(str, 't', 0));
}
END_TEST

Suite *memchr_suite(void) {
  Suite *s = suite_create("memchr");
  TCase *tc = tcase_create("core");
  tcase_add_test(tc, test_memchr_basic);
  tcase_add_test(tc, test_memchr_zero_n);
  suite_add_tcase(s, tc);
  return s;
}