#ifndef S21_SPRINTF_H
#define S21_SPRINTF_H
#include <stdarg.h>

#include "s21_helpers.h"  // FormatSpec берём только отсюда
#include "s21_string.h"
#ifdef __cplusplus
extern "C" {
#endif
int s21_sprintf(char *str, const char *format, ...);
int s21_process_format_token(char *str, int *written, const char **fmt,
                             va_list *args);
const char *s21_parse_format_spec(const char *fmt, FormatSpec *spec,
                                  va_list *args);
int s21_apply_width(char *out, const char *value_str, int vlen,
                    const FormatSpec *spec, char sign_char);
int s21_apply_width_prefix(char *out, const char *value_str, int vlen,
                           const FormatSpec *spec, char sign_char,
                           const char *prefix, int plen);
int s21_fmt_d(char *out, const FormatSpec *spec, va_list *args);
int s21_fmt_u(char *out, const FormatSpec *spec, va_list *args);
int s21_fmt_f(char *out, const FormatSpec *spec, va_list *args);
int s21_fmt_s(char *out, const FormatSpec *spec, va_list *args);
int s21_fmt_c(char *out, const FormatSpec *spec, va_list *args);
int s21_fmt_x(char *out, const FormatSpec *spec, va_list *args);
int s21_fmt_o(char *out, const FormatSpec *spec, va_list *args);
int s21_fmt_p(char *out, const FormatSpec *spec, va_list *args);
int s21_fmt_e(char *out, const FormatSpec *spec, va_list *args);
int s21_fmt_g(char *out, const FormatSpec *spec, va_list *args);
#ifdef __cplusplus
}
#endif
#endif  // S21_SPRINTF_H