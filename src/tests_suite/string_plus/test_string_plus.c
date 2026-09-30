#include <check.h>
#include <stdlib.h>
#include <string.h>

#include "../../s21_string.h"

START_TEST(test_to_upper_basic) {
  char *src = "hello";
  char *result = s21_to_upper(src);
  ck_assert_str_eq(result, "HELLO");
  free(result);
}
END_TEST

START_TEST(test_to_upper_empty) {
  char *src = "";
  char *result = s21_to_upper(src);
  ck_assert_str_eq(result, "");
  free(result);
}
END_TEST

START_TEST(test_to_lower_basic) {
  char *src = "HELLO";
  char *result = s21_to_lower(src);
  ck_assert_str_eq(result, "hello");
  free(result);
}
END_TEST

START_TEST(test_to_lower_empty) {
  char *src = "";
  char *result = s21_to_lower(src);
  ck_assert_str_eq(result, "");
  free(result);
}
END_TEST

START_TEST(test_insert_basic) {
  char *src = "Hello!";
  char *str = "world ";
  char *result = s21_insert(src, str, 6);
  ck_assert_str_eq(result, "Hello!world ");
  free(result);
}
END_TEST

START_TEST(test_insert_start) {
  char *src = "world";
  char *str = "Hello ";
  char *result = s21_insert(src, str, 0);
  ck_assert_str_eq(result, "Hello world");
  free(result);
}
END_TEST

START_TEST(test_insert_end) {
  char *src = "Hello";
  char *str = " world";
  char *result = s21_insert(src, str, 5);
  ck_assert_str_eq(result, "Hello world");
  free(result);
}
END_TEST

START_TEST(test_trim_basic) {
  char *src = "***Hello***";
  char *trim_chars = "*";
  char *result = s21_trim(src, trim_chars);
  ck_assert_str_eq(result, "Hello");
  free(result);
}
END_TEST

START_TEST(test_trim_default) {
  char *src = "  Hello  ";
  char *result = s21_trim(src, S21_NULL);
  ck_assert_str_eq(result, "Hello");
  free(result);
}
END_TEST

START_TEST(test_trim_all) {
  char *src = "***";
  char *trim_chars = "*";
  char *result = s21_trim(src, trim_chars);
  ck_assert_str_eq(result, "");
  free(result);
}
END_TEST

Suite *string_plus_suite(void) {
  Suite *s = suite_create("string_plus");
  TCase *tc = tcase_create("core");
  tcase_add_test(tc, test_to_upper_basic);
  tcase_add_test(tc, test_to_upper_empty);
  tcase_add_test(tc, test_to_lower_basic);
  tcase_add_test(tc, test_to_lower_empty);
  tcase_add_test(tc, test_insert_basic);
  tcase_add_test(tc, test_insert_start);
  tcase_add_test(tc, test_insert_end);
  tcase_add_test(tc, test_trim_basic);
  tcase_add_test(tc, test_trim_default);
  tcase_add_test(tc, test_trim_all);
  suite_add_tcase(s, tc);
  return s;
}