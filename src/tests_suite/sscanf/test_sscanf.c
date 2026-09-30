#include <check.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "../../s21_string.h"

START_TEST(test_sscanf_d_basic) {
  const char *src = "42";
  const char *fmt = "%d";
  int a = -1, b = -2;
  int ra = sscanf(src, fmt, &a);
  int rb = s21_sscanf(src, fmt, &b);
  ck_assert_int_eq(ra, rb);
  if (ra > 0) ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_sscanf_d_negative) {
  const char *src = "-2147483648";
  const char *fmt = "%d";
  int a = 0, b = 1;
  int ra = sscanf(src, fmt, &a);
  int rb = s21_sscanf(src, fmt, &b);
  ck_assert_int_eq(ra, rb);
  if (ra > 0) ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_sscanf_d_width) {
  const char *src = "123456";
  const char *fmt = "%3d";
  int a = 0, b = 0;
  int ra = sscanf(src, fmt, &a);
  int rb = s21_sscanf(src, fmt, &b);
  ck_assert_int_eq(ra, rb);
  if (ra > 0) ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_sscanf_u_basic) {
  const char *src = "4294967295";
  const char *fmt = "%u";
  unsigned int a = 0, b = 0;
  int ra = sscanf(src, fmt, &a);
  int rb = s21_sscanf(src, fmt, &b);
  ck_assert_int_eq(ra, rb);
  if (ra > 0) ck_assert_uint_eq(a, b);
}
END_TEST

START_TEST(test_sscanf_i_hex) {
  const char *src = "0x1f";
  const char *fmt = "%i";
  int a = 0, b = 0;
  int ra = sscanf(src, fmt, &a);
  int rb = s21_sscanf(src, fmt, &b);
  ck_assert_int_eq(ra, rb);
  if (ra > 0) ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_sscanf_o_basic) {
  const char *src = "377";
  const char *fmt = "%o";
  unsigned int a = 0, b = 0;
  int ra = sscanf(src, fmt, &a);
  int rb = s21_sscanf(src, fmt, &b);
  ck_assert_int_eq(ra, rb);
  if (ra > 0) ck_assert_uint_eq(a, b);
}
END_TEST

START_TEST(test_sscanf_x_basic) {
  const char *src = "deadBEEF";
  const char *fmt = "%x";
  unsigned int a = 0, b = 0;
  int ra = sscanf(src, fmt, &a);
  int rb = s21_sscanf(src, fmt, &b);
  ck_assert_int_eq(ra, rb);
  if (ra > 0) ck_assert_uint_eq(a, b);
}
END_TEST

START_TEST(test_sscanf_p_basic) {
  const char *src = "0x1234";
  const char *fmt = "%p";
  void *a = NULL, *b = NULL;
  int ra = sscanf(src, fmt, &a);
  int rb = s21_sscanf(src, fmt, &b);
  ck_assert_int_eq(ra, rb);
  if (ra > 0) ck_assert_ptr_eq(a, b);
}
END_TEST

START_TEST(test_sscanf_p_nil) {
  const char *src = "(nil)";
  const char *fmt = "%p";
  void *a = (void *)1, *b = (void *)2;
  int ra = sscanf(src, fmt, &a);
  int rb = s21_sscanf(src, fmt, &b);
  ck_assert_int_eq(ra, rb);
  if (ra > 0) ck_assert_ptr_eq(a, b);
}
END_TEST

START_TEST(test_sscanf_f_basic) {
  const char *src = "3.14159";
  const char *fmt = "%f";
  float a = 0, b = 0;
  int ra = sscanf(src, fmt, &a);
  int rb = s21_sscanf(src, fmt, &b);
  ck_assert_int_eq(ra, rb);
  if (ra > 0) ck_assert_float_eq(a, b);
}
END_TEST

START_TEST(test_sscanf_lf_exp) {
  const char *src = "-1.23e+10";
  const char *fmt = "%lf";
  double a = 0, b = 0;
  int ra = sscanf(src, fmt, &a);
  int rb = s21_sscanf(src, fmt, &b);
  ck_assert_int_eq(ra, rb);
  if (ra > 0) ck_assert_double_eq(a, b);
}
END_TEST

START_TEST(test_sscanf_Lf) {
  const char *src = "3.141592653589793238";
  const char *fmt = "%Lf";
  long double a = 0, b = 0;
  int ra = sscanf(src, fmt, &a);
  int rb = s21_sscanf(src, fmt, &b);
  ck_assert_int_eq(ra, rb);
  if (ra > 0) ck_assert_ldouble_eq(a, b);
}
END_TEST

START_TEST(test_sscanf_s_basic) {
  const char *src = "hello world";
  const char *fmt = "%s";
  char a[64] = {0};
  char b[64] = {0};
  int ra = sscanf(src, fmt, a);
  int rb = s21_sscanf(src, fmt, b);
  ck_assert_int_eq(ra, rb);
  if (ra > 0) ck_assert_str_eq(a, b);
}
END_TEST

START_TEST(test_sscanf_s_width) {
  const char *src = "abcdef";
  const char *fmt = "%3s";
  char a[64] = {0};
  char b[64] = {0};
  int ra = sscanf(src, fmt, a);
  int rb = s21_sscanf(src, fmt, b);
  ck_assert_int_eq(ra, rb);
  if (ra > 0) ck_assert_str_eq(a, b);
}
END_TEST

START_TEST(test_sscanf_c_basic) {
  const char *src = "XYZ";
  const char *fmt = "%c";
  char a = 0, b = 0;
  int ra = sscanf(src, fmt, &a);
  int rb = s21_sscanf(src, fmt, &b);
  ck_assert_int_eq(ra, rb);
  if (ra > 0) ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_sscanf_c_width) {
  const char *src = "ABCD";
  const char *fmt = "%3c";
  char a[4] = {0}, b[4] = {0};
  int ra = sscanf(src, fmt, a);
  int rb = s21_sscanf(src, fmt, b);
  ck_assert_int_eq(ra, rb);
  if (ra > 0) ck_assert_mem_eq(a, b, 3);
}
END_TEST

START_TEST(test_sscanf_n) {
  const char *src = "123 abc";
  const char *fmt = "%d %n%s";
  int a1 = 0, a2 = 0;
  int n1 = -1, n2 = -2;
  char s1[16] = {0}, s2[16] = {0};
  int ra = sscanf(src, fmt, &a1, &n1, s1);
  int rb = s21_sscanf(src, fmt, &a2, &n2, s2);
  ck_assert_int_eq(ra, rb);
  if (ra >= 0) {
    ck_assert_int_eq(a1, a2);
    ck_assert_int_eq(n1, n2);
    ck_assert_str_eq(s1, s2);
  }
}
END_TEST

START_TEST(test_sscanf_percent) {
  const char *src = "% 77";
  const char *fmt = "%% %d";
  int a = 0, b = 0;
  int ra = sscanf(src, fmt, &a);
  int rb = s21_sscanf(src, fmt, &b);
  ck_assert_int_eq(ra, rb);
  if (ra > 0) ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_sscanf_suppress) {
  const char *src = "10 20";
  const char *fmt = "%*d %d";
  int a = 0, b = 0;
  int ra = sscanf(src, fmt, &a);
  int rb = s21_sscanf(src, fmt, &b);
  ck_assert_int_eq(ra, rb);
  if (ra > 0) ck_assert_int_eq(a, b);
}
END_TEST

START_TEST(test_sscanf_mixed) {
  const char *src = "-42 0xff 3.14 word";
  const char *fmt = "%d %x %lf %s";
  int d1 = 0, d2 = 0;
  unsigned int x1 = 0, x2 = 0;
  double f1 = 0, f2 = 0;
  char s1[32] = {0}, s2[32] = {0};
  int ra = sscanf(src, fmt, &d1, &x1, &f1, s1);
  int rb = s21_sscanf(src, fmt, &d2, &x2, &f2, s2);
  ck_assert_int_eq(ra, rb);
  if (ra >= 0) {
    ck_assert_int_eq(d1, d2);
    ck_assert_uint_eq(x1, x2);
    ck_assert_double_eq(f1, f2);
    ck_assert_str_eq(s1, s2);
  }
}
END_TEST

START_TEST(test_sscanf_eof) {
  const char *src = "";
  const char *fmt = "%d";
  int a = 123, b = 456;
  int ra = sscanf(src, fmt, &a);
  int rb = s21_sscanf(src, fmt, &b);
  ck_assert_int_eq(ra, rb);
}
END_TEST

Suite *sscanf_suite(void) {
  Suite *s = suite_create("sscanf");
  TCase *tc = tcase_create("core");
  tcase_add_test(tc, test_sscanf_d_basic);
  tcase_add_test(tc, test_sscanf_d_negative);
  tcase_add_test(tc, test_sscanf_d_width);
  tcase_add_test(tc, test_sscanf_u_basic);
  tcase_add_test(tc, test_sscanf_i_hex);
  tcase_add_test(tc, test_sscanf_o_basic);
  tcase_add_test(tc, test_sscanf_x_basic);
  tcase_add_test(tc, test_sscanf_p_basic);
  tcase_add_test(tc, test_sscanf_p_nil);
  tcase_add_test(tc, test_sscanf_f_basic);
  tcase_add_test(tc, test_sscanf_lf_exp);
  tcase_add_test(tc, test_sscanf_Lf);
  tcase_add_test(tc, test_sscanf_s_basic);
  tcase_add_test(tc, test_sscanf_s_width);
  tcase_add_test(tc, test_sscanf_c_basic);
  tcase_add_test(tc, test_sscanf_c_width);
  tcase_add_test(tc, test_sscanf_n);
  tcase_add_test(tc, test_sscanf_percent);
  tcase_add_test(tc, test_sscanf_suppress);
  tcase_add_test(tc, test_sscanf_mixed);
  tcase_add_test(tc, test_sscanf_eof);
  suite_add_tcase(s, tc);
  return s;
}