#include "memcpy_suite.h"

#include <string.h>

#include "../../s21_string.h"

START_TEST(test_memcpy_normal) {
  char dest1[20], dest2[20];
  char src[] = "Copy this!";
  s21_memcpy(dest1, src, 11);
  memcpy(dest2, src, 11);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

Suite *memcpy_suite(void) {
  Suite *s = suite_create("memcpy");
  TCase *tc = tcase_create("core");
  tcase_add_test(tc, test_memcpy_normal);
  suite_add_tcase(s, tc);
  return s;
}