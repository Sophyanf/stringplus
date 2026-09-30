#include "strerror_suite.h"

#include <string.h>

#include "../../s21_string.h"

START_TEST(test_strerror_valid) {
  for (int i = 0; i < 10; i++) {
    ck_assert_str_eq(s21_strerror(i), strerror(i));
  }
}
END_TEST

START_TEST(test_strerror_invalid) {
  ck_assert_str_eq(s21_strerror(999), strerror(999));
}
END_TEST

Suite *strerror_suite(void) {
  Suite *s = suite_create("strerror");
  TCase *tc = tcase_create("core");
  tcase_add_test(tc, test_strerror_valid);
  tcase_add_test(tc, test_strerror_invalid);
  suite_add_tcase(s, tc);
  return s;
}