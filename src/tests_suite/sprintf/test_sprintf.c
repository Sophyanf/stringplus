#include <check.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "../../s21_string.h"

START_TEST(test_sprintf_d_basic) {
  char s21_buf[1024] = {0};
  char std_buf[1024] = {0};
  int s21_ret = s21_sprintf(s21_buf, "%d", 42);
  int std_ret = sprintf(std_buf, "%d", 42);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_d_negative) {
  char s21_buf[1024] = {0};
  char std_buf[1024] = {0};
  int s21_ret = s21_sprintf(s21_buf, "%d", -42);
  int std_ret = sprintf(std_buf, "%d", -42);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_d_width) {
  char s21_buf[1024] = {0};
  char std_buf[1024] = {0};
  int s21_ret = s21_sprintf(s21_buf, "%10d", 42);
  int std_ret = sprintf(std_buf, "%10d", 42);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_d_plus) {
  char s21_buf[1024] = {0};
  char std_buf[1024] = {0};
  int s21_ret = s21_sprintf(s21_buf, "%+d", 42);
  int std_ret = sprintf(std_buf, "%+d", 42);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_d_space) {
  char s21_buf[1024] = {0};
  char std_buf[1024] = {0};
  int s21_ret = s21_sprintf(s21_buf, "% d", 42);
  int std_ret = sprintf(std_buf, "% d", 42);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_d_precision) {
  char s21_buf[1024] = {0};
  char std_buf[1024] = {0};
  int s21_ret = s21_sprintf(s21_buf, "%.5d", 42);
  int std_ret = sprintf(std_buf, "%.5d", 42);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_d_precision_zero) {
  char s21_buf[1024] = {0};
  char std_buf[1024] = {0};
  int s21_ret = s21_sprintf(s21_buf, "%.0d", 0);
  int std_ret = sprintf(std_buf, "%.0d", 0);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_f_basic) {
  char s21_buf[1024] = {0};
  char std_buf[1024] = {0};
  int s21_ret = s21_sprintf(s21_buf, "%f", 3.14);
  int std_ret = sprintf(std_buf, "%f", 3.14);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_f_precision) {
  char s21_buf[1024] = {0};
  char std_buf[1024] = {0};
  int s21_ret = s21_sprintf(s21_buf, "%.2f", 3.14159);
  int std_ret = sprintf(std_buf, "%.2f", 3.14159);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_f_width) {
  char s21_buf[1024] = {0};
  char std_buf[1024] = {0};
  int s21_ret = s21_sprintf(s21_buf, "%10.2f", 3.14);
  int std_ret = sprintf(std_buf, "%10.2f", 3.14);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_s_basic) {
  char s21_buf[1024] = {0};
  char std_buf[1024] = {0};
  int s21_ret = s21_sprintf(s21_buf, "%s", "hello");
  int std_ret = sprintf(std_buf, "%s", "hello");
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_s_width) {
  char s21_buf[1024] = {0};
  char std_buf[1024] = {0};
  int s21_ret = s21_sprintf(s21_buf, "%10s", "hi");
  int std_ret = sprintf(std_buf, "%10s", "hi");
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_s_precision) {
  char s21_buf[1024] = {0};
  char std_buf[1024] = {0};
  int s21_ret = s21_sprintf(s21_buf, "%.3s", "hello");
  int std_ret = sprintf(std_buf, "%.3s", "hello");
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_c_basic) {
  char s21_buf[1024] = {0};
  char std_buf[1024] = {0};
  int s21_ret = s21_sprintf(s21_buf, "%c", 'A');
  int std_ret = sprintf(std_buf, "%c", 'A');
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_c_width) {
  char s21_buf[1024] = {0};
  char std_buf[1024] = {0};
  int s21_ret = s21_sprintf(s21_buf, "%5c", 'X');
  int std_ret = sprintf(std_buf, "%5c", 'X');
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_percent) {
  char s21_buf[1024] = {0};
  char std_buf[1024] = {0};
  int s21_ret = s21_sprintf(s21_buf, "100%%");
  int std_ret = sprintf(std_buf, "100%%");
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_mixed) {
  char s21_buf[1024] = {0};
  char std_buf[1024] = {0};
  int s21_ret = s21_sprintf(s21_buf, "%-10s %+d %.2f", "test", 42, 3.14);
  int std_ret = sprintf(std_buf, "%-10s %+d %.2f", "test", 42, 3.14);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_no_format) {
  char s21_buf[1024] = {0};
  char std_buf[1024] = {0};
  int s21_ret = s21_sprintf(s21_buf, "no format specifiers here");
  int std_ret = sprintf(std_buf, "no format specifiers here");
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_x_basic) {
  char s21_buf[1024] = {0};
  char std_buf[1024] = {0};
  int s21_ret = s21_sprintf(s21_buf, "%x", 255);
  int std_ret = sprintf(std_buf, "%x", 255);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_X_basic) {
  char s21_buf[1024] = {0};
  char std_buf[1024] = {0};
  int s21_ret = s21_sprintf(s21_buf, "%X", 255);
  int std_ret = sprintf(std_buf, "%X", 255);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_x_hash) {
  char s21_buf[1024] = {0};
  char std_buf[1024] = {0};
  int s21_ret = s21_sprintf(s21_buf, "%#x", 255);
  int std_ret = sprintf(std_buf, "%#x", 255);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_o_basic) {
  char s21_buf[1024] = {0};
  char std_buf[1024] = {0};
  int s21_ret = s21_sprintf(s21_buf, "%o", 8);
  int std_ret = sprintf(std_buf, "%o", 8);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_p_basic) {
  char s21_buf[1024] = {0};
  char std_buf[1024] = {0};
  int x = 42;
  void *ptr = &x;
  int s21_ret = s21_sprintf(s21_buf, "%p", ptr);
  int std_ret = sprintf(std_buf, "%p", ptr);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

Suite *sprintf_suite(void) {
  Suite *s = suite_create("sprintf");
  TCase *tc = tcase_create("core");
  tcase_add_test(tc, test_sprintf_d_basic);
  tcase_add_test(tc, test_sprintf_d_negative);
  tcase_add_test(tc, test_sprintf_d_width);
  tcase_add_test(tc, test_sprintf_d_plus);
  tcase_add_test(tc, test_sprintf_d_space);
  tcase_add_test(tc, test_sprintf_d_precision);
  tcase_add_test(tc, test_sprintf_d_precision_zero);
  tcase_add_test(tc, test_sprintf_f_basic);
  tcase_add_test(tc, test_sprintf_f_precision);
  tcase_add_test(tc, test_sprintf_f_width);
  tcase_add_test(tc, test_sprintf_s_basic);
  tcase_add_test(tc, test_sprintf_s_width);
  tcase_add_test(tc, test_sprintf_s_precision);
  tcase_add_test(tc, test_sprintf_c_basic);
  tcase_add_test(tc, test_sprintf_c_width);
  tcase_add_test(tc, test_sprintf_percent);
  tcase_add_test(tc, test_sprintf_mixed);
  tcase_add_test(tc, test_sprintf_no_format);
  tcase_add_test(tc, test_sprintf_x_basic);
  tcase_add_test(tc, test_sprintf_X_basic);
  tcase_add_test(tc, test_sprintf_x_hash);
  tcase_add_test(tc, test_sprintf_o_basic);
  tcase_add_test(tc, test_sprintf_p_basic);
  suite_add_tcase(s, tc);
  return s;
}