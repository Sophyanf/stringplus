#include <check.h>

Suite *memchr_suite(void);
Suite *memcmp_suite(void);
Suite *memcpy_suite(void);
Suite *memset_suite(void);
Suite *strerror_suite(void);
Suite *sprintf_suite(void);
Suite *sscanf_suite(void);
Suite *string_plus_suite(void);

int main(void) {
  int failed = 0;
  SRunner *sr;

  sr = srunner_create(memchr_suite());
  srunner_run_all(sr, CK_VERBOSE);
  failed += srunner_ntests_failed(sr);
  srunner_free(sr);

  sr = srunner_create(memcmp_suite());
  srunner_run_all(sr, CK_VERBOSE);
  failed += srunner_ntests_failed(sr);
  srunner_free(sr);

  sr = srunner_create(memcpy_suite());
  srunner_run_all(sr, CK_VERBOSE);
  failed += srunner_ntests_failed(sr);
  srunner_free(sr);

  sr = srunner_create(memset_suite());
  srunner_run_all(sr, CK_VERBOSE);
  failed += srunner_ntests_failed(sr);
  srunner_free(sr);

  sr = srunner_create(strerror_suite());
  srunner_run_all(sr, CK_VERBOSE);
  failed += srunner_ntests_failed(sr);
  srunner_free(sr);

  sr = srunner_create(sprintf_suite());
  srunner_run_all(sr, CK_VERBOSE);
  failed += srunner_ntests_failed(sr);
  srunner_free(sr);

  sr = srunner_create(sscanf_suite());
  srunner_run_all(sr, CK_VERBOSE);
  failed += srunner_ntests_failed(sr);
  srunner_free(sr);

  sr = srunner_create(string_plus_suite());
  srunner_run_all(sr, CK_VERBOSE);
  failed += srunner_ntests_failed(sr);
  srunner_free(sr);

  return (failed == 0) ? 0 : 1;
}