#include "s21_sprintf.h"

#include <math.h>

static void s21_init_format_spec(FormatSpec *spec);

/* =========================
 * 1. Разбор спецификатора
 * ========================= */

int s21_sprintf(char *str, const char *format, ...) {
  va_list args;
  int written = 0;
  const char *fmt = format;
  int keep_running = 1;

  va_start(args, format);

  while (*fmt != '\0' && keep_running) {
    keep_running = s21_process_format_token(str, &written, &fmt, &args);
    str[written] = '\0';  // ФИКС 6: терминация на лету
  }

  str[written] = '\0';
  va_end(args);
  return written;
}

static int s21_handle_percent_literal(char *out, const FormatSpec *spec) {
  FormatSpec local = *spec;
  int delta = 0;

  local.flag_plus = 0;
  local.flag_space = 0;
  local.flag_hash = 0;
  local.has_precision = 0;

  delta = s21_apply_width(out, "%", 1, &local, '\0');
  return delta;
}

static int s21_handle_unknown_spec(char *out, const FormatSpec *spec) {
  out[0] = '%';
  int delta = 1;

  if (spec->specifier != '\0') {
    out[1] = spec->specifier;
    delta = 2;
  }

  return delta;
}

static int s21_dispatch_spec(char *out, const FormatSpec *spec, va_list *args) {
  switch (spec->specifier) {
    case '%':
      return s21_handle_percent_literal(out, spec);
    case 'd':
      return s21_fmt_d(out, spec, args);
    case 'u':
      return s21_fmt_u(out, spec, args);
    case 'f':
      return s21_fmt_f(out, spec, args);
    case 's':
      return s21_fmt_s(out, spec, args);
    case 'c':
      return s21_fmt_c(out, spec, args);
    case 'x':
    case 'X':
      return s21_fmt_x(out, spec, args);
    case 'o':
      return s21_fmt_o(out, spec, args);
    case 'p':
      return s21_fmt_p(out, spec, args);
    case 'e':
    case 'E':
      return s21_fmt_e(out, spec, args);
    case 'g':
    case 'G':
      return s21_fmt_g(out, spec, args);
    default:
      return s21_handle_unknown_spec(out, spec);
  }
}

int s21_process_format_token(char *str, int *written, const char **fmt,
                             va_list *args) {
  int keep_running = 1;

  if (**fmt != '%') {
    str[*written] = **fmt;
    (*written)++;
    (*fmt)++;
  } else {
    (*fmt)++;

    if (**fmt == '\0') {
      keep_running = 0;
    } else {
      FormatSpec spec;
      // ФИКС 4: инициализация структуры
      s21_init_format_spec(&spec);

      int delta = 0;

      *fmt = s21_parse_format_spec(*fmt, &spec, args);
      delta = s21_dispatch_spec(str + *written, &spec, args);
      *written += delta;

      if (spec.specifier == '\0') {
        keep_running = 0;
      } else {
        (*fmt)++;
      }
    }
  }

  str[*written] = '\0';  // ФИКС 6: терминация на лету
  return keep_running;
}

static void s21_init_format_spec(FormatSpec *spec) {
  spec->flag_minus = 0;
  spec->flag_plus = 0;
  spec->flag_space = 0;
  spec->flag_hash = 0;
  spec->flag_zero = 0;
  spec->width = 0;
  spec->has_precision = 0;
  spec->precision = 0;
  spec->length = '\0';
  spec->specifier = '\0';
}

static const char *s21_parse_flags(const char *fmt, FormatSpec *spec) {
  int reading_flags = 1;
  while (reading_flags) {
    switch (*fmt) {
      case '-':
        spec->flag_minus = 1;
        fmt++;
        break;
      case '+':
        spec->flag_plus = 1;
        fmt++;
        break;
      case ' ':
        spec->flag_space = 1;
        fmt++;
        break;
      case '#':
        spec->flag_hash = 1;
        fmt++;
        break;
      case '0':
        spec->flag_zero = 1;
        fmt++;
        break;
      default:
        reading_flags = 0;
        break;
    }
  }
  return fmt;
}

static const char *s21_parse_width(const char *fmt, FormatSpec *spec,
                                   va_list *args) {
  const char *result = fmt;
  if (*fmt == '*') {
    int w = va_arg(*args, int);
    if (w < 0) {
      spec->flag_minus = 1;
      w = -w;
    }
    spec->width = w;
    result = fmt + 1;
  } else {
    int consumed = s21_parse_uint(fmt, &spec->width);
    result = fmt + consumed;
  }
  return result;
}

static const char *s21_parse_precision(const char *fmt, FormatSpec *spec,
                                       va_list *args) {
  const char *result = fmt;

  if (*fmt == '.') {
    fmt++;
    spec->has_precision = 1;

    if (*fmt == '*') {
      int p = va_arg(*args, int);
      if (p < 0) {
        spec->has_precision = 0;
        spec->precision = 0;
      } else {
        spec->precision = p;
      }
      result = fmt + 1;
    } else {
      int consumed = s21_parse_uint(fmt, &spec->precision);
      result = fmt + consumed;
    }
  }

  return result;
}

static const char *s21_parse_length(const char *fmt, FormatSpec *spec) {
  const char *result = fmt;
  if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
    spec->length = *fmt;
    result = fmt + 1;
  }
  return result;
}

const char *s21_parse_format_spec(const char *fmt, FormatSpec *spec,
                                  va_list *args) {
  s21_init_format_spec(spec);
  fmt = s21_parse_flags(fmt, spec);
  fmt = s21_parse_width(fmt, spec, args);
  fmt = s21_parse_precision(fmt, spec, args);
  fmt = s21_parse_length(fmt, spec);
  spec->specifier = *fmt;
  return fmt;
}

/* =========================
 * 2. Ширина и паддинг
 * ========================= */

int s21_apply_width(char *out, const char *value_str, int vlen,
                    const FormatSpec *spec, char sign_char) {
  return s21_apply_width_prefix(out, value_str, vlen, spec, sign_char, S21_NULL,
                                0);
}

int s21_apply_width_prefix(char *out, const char *value_str, int vlen,
                           const FormatSpec *spec, char sign_char,
                           const char *prefix, int plen) {
  int written = 0;
  int has_sign = (sign_char != '\0') ? 1 : 0;
  int content_len = has_sign + plen + vlen;
  int pad = (spec->width > content_len) ? (spec->width - content_len) : 0;

  // ФИКС 3: флаг 0 для f
  int use_zero = spec->flag_zero && !spec->flag_minus &&
                 !(spec->has_precision && spec->specifier != 'f');
  char pad_char = use_zero ? '0' : ' ';

  if (!spec->flag_minus && pad_char == ' ') {
    written += s21_fill_n(out + written, ' ', pad);
  }
  if (has_sign) out[written++] = sign_char;
  if (prefix && plen > 0) {
    written += s21_copy_n(out + written, prefix, plen);
  }
  if (!spec->flag_minus && pad_char == '0') {
    written += s21_fill_n(out + written, '0', pad);
  }
  written += s21_copy_n(out + written, value_str, vlen);
  if (spec->flag_minus) {
    written += s21_fill_n(out + written, ' ', pad);
  }
  return written;
}

/* =========================
 * 3. Вспомогательные функции
 * ========================= */

static char get_sign(int is_negative, const FormatSpec *spec) {
  char result = '\0';
  if (is_negative) {
    result = '-';
  } else if (spec->flag_plus) {
    result = '+';
  } else if (spec->flag_space) {
    result = ' ';
  }
  return result;
}

static int apply_int_precision(char *out, const char *value_str, int value_len,
                               int precision) {
  int written = 0;

  if (!(precision == 0 && value_len == 1 && value_str[0] == '0')) {
    int zeros = (precision > value_len) ? (precision - value_len) : 0;
    written += s21_fill_n(out + written, '0', zeros);
    written += s21_copy_n(out + written, value_str, value_len);
  }

  return written;
}

static long long read_signed_arg_by_length(const FormatSpec *spec,
                                           va_list *args) {
  if (spec->length == 'l') return (long long)va_arg(*args, long);
  if (spec->length == 'h') return (long long)(short)va_arg(*args, int);
  return (long long)va_arg(*args, int);
}

static unsigned long long read_unsigned_arg_by_length(const FormatSpec *spec,
                                                      va_list *args) {
  unsigned long long result;

  if (spec->length == 'l') {
    result = (unsigned long long)va_arg(*args, unsigned long);
  } else if (spec->length == 'h') {
    result = (unsigned long long)(unsigned short)va_arg(*args, unsigned int);
  } else {
    result = (unsigned long long)va_arg(*args, unsigned int);
  }

  return result;
}

static int s21_build_exp_str(char *buf, int exp_val, int upper) {
  int written = 0;
  buf[written++] = upper ? 'E' : 'e';
  buf[written++] = (exp_val < 0) ? '-' : '+';
  if (exp_val < 0) exp_val = -exp_val;
  char e_buf[16];
  int e_len = s21_uint_to_str((unsigned long long)exp_val, 10, 0, e_buf);
  if (e_len < 2) buf[written++] = '0';
  written += s21_copy_n(buf + written, e_buf, e_len);
  return written;
}

static long double s21_decompose_long_double(long double value, int *exp_out) {
  long double result = value;

  // ФИКС 1: защита от NaN/Inf
  if (isnan(value)) {
    *exp_out = 0;
    return 0.0L;
  }
  if (isinf(value)) {
    *exp_out = 0;
    return value;
  }

  if (value == 0.0L) {
    *exp_out = 0;
    result = 0.0L;
  } else {
    int exp = 0;
    while (value >= 10.0L) {
      value /= 10.0L;
      exp++;
    }
    while (value < 1.0L) {
      value *= 10.0L;
      exp--;
    }
    *exp_out = exp;
    result = value;
  }

  return result;
}

/* =========================
 * 4. Форматтеры
 * ========================= */

int s21_fmt_d(char *out, const FormatSpec *spec, va_list *args) {
  long long value = read_signed_arg_by_length(spec, args);
  char sign = get_sign(value < 0, spec);
  char num_buf[32];
  int num_len = 0;
  if (value < 0) {
    unsigned long long uval = (unsigned long long)(-(value + 1)) + 1ULL;
    num_len = s21_uint_to_str(uval, 10, 0, num_buf);
  } else {
    num_len = s21_uint_to_str((unsigned long long)value, 10, 0, num_buf);
  }
  char prec_buf[64];
  int prec_len =
      spec->has_precision
          ? apply_int_precision(prec_buf, num_buf, num_len, spec->precision)
          : s21_copy_n(prec_buf, num_buf, num_len);
  return s21_apply_width(out, prec_buf, prec_len, spec, sign);
}

int s21_fmt_u(char *out, const FormatSpec *spec, va_list *args) {
  unsigned long long value = read_unsigned_arg_by_length(spec, args);
  char num_buf[32];
  int num_len = s21_uint_to_str(value, 10, 0, num_buf);
  char prec_buf[64];
  int prec_len =
      spec->has_precision
          ? apply_int_precision(prec_buf, num_buf, num_len, spec->precision)
          : s21_copy_n(prec_buf, num_buf, num_len);
  return s21_apply_width(out, prec_buf, prec_len, spec, '\0');
}

static long double s21_read_float_arg(const FormatSpec *spec, va_list *args) {
  long double value = 0.0L;
  if (spec->length == 'L') {
    value = va_arg(*args, long double);
  } else {
    value = va_arg(*args, double);
  }
  return value;
}

static int s21_resolve_f_precision(const FormatSpec *spec) {
  int precision = 6;
  if (spec->has_precision) {
    precision = spec->precision;
  }
  return precision;
}

static long double s21_abs_and_sign(long double value, const FormatSpec *spec,
                                    char *sign_out) {
  long double abs_value = value;
  char sign = '\0';

  if (value < 0.0L) {
    sign = '-';
    abs_value = -value;
  } else {
    sign = get_sign(0, spec);
  }

  *sign_out = sign;
  return abs_value;
}

static long double s21_pow10_ld_local(int precision) {
  long double multiplier = 1.0L;
  for (int i = 0; i < precision; i++) {
    multiplier *= 10.0L;
  }
  return multiplier;
}

static unsigned long long s21_pow10_ull_local(int precision) {
  unsigned long long threshold = 1ULL;
  for (int i = 0; i < precision; i++) {
    threshold *= 10ULL;
  }
  return threshold;
}

static void s21_split_rounded_f(long double abs_value, int precision,
                                unsigned long long *int_part_out,
                                unsigned long long *frac_part_out) {
  unsigned long long int_part = (unsigned long long)abs_value;
  long double frac = abs_value - (long double)int_part;

  long double multiplier = s21_pow10_ld_local(precision);
  unsigned long long frac_part = (unsigned long long)(frac * multiplier + 0.5L);

  unsigned long long threshold = s21_pow10_ull_local(precision);
  if (frac_part >= threshold) {
    frac_part -= threshold;
    int_part++;
  }

  *int_part_out = int_part;
  *frac_part_out = frac_part;
}

static int s21_build_f_result(char *result_buf, unsigned long long int_part,
                              unsigned long long frac_part, int precision,
                              int force_dot) {
  int rlen = 0;

  char int_buf[32];
  int int_len = s21_uint_to_str(int_part, 10, 0, int_buf);
  rlen += s21_copy_n(result_buf + rlen, int_buf, int_len);

  if (precision > 0 || force_dot) {
    result_buf[rlen++] = '.';

    if (precision > 0) {
      char frac_buf[32];
      int frac_len = s21_uint_to_str(frac_part, 10, 0, frac_buf);
      int leading = precision - frac_len;
      rlen += s21_fill_n(result_buf + rlen, '0', leading);
      rlen += s21_copy_n(result_buf + rlen, frac_buf, frac_len);
    }
  }

  return rlen;
}

// ФИКС 1: защита от NaN/Inf в f
int s21_fmt_f(char *out, const FormatSpec *spec, va_list *args) {
  int written = 0;

  long double value = s21_read_float_arg(spec, args);

  // ФИКС 1: обработка NaN/Inf
  if (isnan(value)) {
    char *nan_str = "nan";
    return s21_apply_width(out, nan_str, 3, spec, '\0');
  }
  if (isinf(value)) {
    char *inf_str = (value < 0.0L) ? "-inf" : "inf";
    int sign = (value < 0.0L) ? '-' : '\0';
    return s21_apply_width(out, inf_str + (sign == '-'), 3, spec, sign);
  }

  int precision = s21_resolve_f_precision(spec);

  char sign = '\0';
  long double abs_value = s21_abs_and_sign(value, spec, &sign);

  unsigned long long int_part = 0;
  unsigned long long frac_part = 0;
  s21_split_rounded_f(abs_value, precision, &int_part, &frac_part);

  char result_buf[128];
  int rlen = s21_build_f_result(result_buf, int_part, frac_part, precision,
                                spec->flag_hash);

  FormatSpec width_spec = *spec;
  width_spec.has_precision = 0;

  written = s21_apply_width_prefix(out, result_buf, rlen, &width_spec, sign,
                                   S21_NULL, 0);
  return written;
}

int s21_fmt_s(char *out, const FormatSpec *spec, va_list *args) {
  const char *str = va_arg(*args, const char *);
  if (str == S21_NULL) str = "(null)";
  int slen = 0;
  while (str[slen] != '\0') slen++;
  if (spec->has_precision && spec->precision < slen) {
    slen = spec->precision;
  }
  FormatSpec local = *spec;
  local.flag_plus = 0;
  local.flag_space = 0;
  return s21_apply_width(out, str, slen, &local, '\0');
}

int s21_fmt_c(char *out, const FormatSpec *spec, va_list *args) {
  char ch = (char)va_arg(*args, int);
  FormatSpec local = *spec;
  local.flag_plus = 0;
  local.flag_space = 0;
  return s21_apply_width(out, &ch, 1, &local, '\0');
}

int s21_fmt_x(char *out, const FormatSpec *spec, va_list *args) {
  unsigned long long value = read_unsigned_arg_by_length(spec, args);
  int upper = (spec->specifier == 'X');
  char num_buf[32];
  int num_len = s21_uint_to_str(value, 16, upper, num_buf);
  char prec_buf[64];
  int prec_len =
      spec->has_precision
          ? apply_int_precision(prec_buf, num_buf, num_len, spec->precision)
          : s21_copy_n(prec_buf, num_buf, num_len);
  const char *prefix = S21_NULL;
  int plen = 0;
  if (spec->flag_hash && value != 0) {
    prefix = upper ? "0X" : "0x";
    plen = 2;
  }
  return s21_apply_width_prefix(out, prec_buf, prec_len, spec, '\0', prefix,
                                plen);
}

int s21_fmt_o(char *out, const FormatSpec *spec, va_list *args) {
  unsigned long long value = read_unsigned_arg_by_length(spec, args);
  char num_buf[32];
  int num_len = s21_uint_to_str(value, 8, 0, num_buf);
  char prec_buf[64];
  int prec_len =
      spec->has_precision
          ? apply_int_precision(prec_buf, num_buf, num_len, spec->precision)
          : s21_copy_n(prec_buf, num_buf, num_len);
  const char *prefix = S21_NULL;
  int plen = 0;
  if (spec->flag_hash && (prec_len == 0 || prec_buf[0] != '0')) {
    prefix = "0";
    plen = 1;
  }
  return s21_apply_width_prefix(out, prec_buf, prec_len, spec, '\0', prefix,
                                plen);
}

// ФИКС 5: платформозависимость %p
int s21_fmt_p(char *out, const FormatSpec *spec, va_list *args) {
  int written = 0;
  void *ptr = va_arg(*args, void *);

  if (ptr == S21_NULL) {
#if defined(__APPLE__)
    written = s21_apply_width(out, "0x0", 3, spec, '\0');
#else
    written = s21_apply_width(out, "(nil)", 5, spec, '\0');
#endif
  } else {
    unsigned long long addr = (unsigned long long)(uintptr_t)ptr;
    char num_buf[32];
    int num_len = s21_uint_to_str(addr, 16, 0, num_buf);
    written =
        s21_apply_width_prefix(out, num_buf, num_len, spec, '\0', "0x", 2);
  }
  return written;
}

/* =========================
 * 5. Форматтеры e, E, g, G
 * ========================= */

static void s21_prepare_e_value(const FormatSpec *spec, va_list *args,
                                long double *abs_value, char *sign,
                                int *precision, int *upper) {
  long double value = 0.0L;

  if (spec->length == 'L') {
    value = va_arg(*args, long double);
  } else {
    value = va_arg(*args, double);
  }

  *upper = (spec->specifier == 'E');
  *precision = spec->has_precision ? spec->precision : 6;

  if (value < 0.0L) {
    *sign = '-';
    value = -value;
  } else {
    *sign = get_sign(0, spec);
  }

  *abs_value = value;
}

static void s21_calc_e_parts(long double abs_value, int precision,
                             unsigned long long *int_digit,
                             unsigned long long *frac_digits, int *exp_val) {
  long double mantissa = s21_decompose_long_double(abs_value, exp_val);

  // ФИКС 2: ограничение точности
  if (precision > 18) precision = 18;

  long double multiplier = 1.0L;
  for (int i = 0; i < precision; i++) {
    multiplier *= 10.0L;
  }

  unsigned long long scaled =
      (unsigned long long)(mantissa * multiplier + 0.5L);

  unsigned long long overflow = (unsigned long long)(multiplier * 10.0L + 0.5L);
  if (scaled >= overflow) {
    scaled /= 10ULL;
    (*exp_val)++;
  }

  unsigned long long div = (unsigned long long)(multiplier + 0.5L);
  // ФИКС 2: защита от деления на ноль
  if (div == 0) div = 1;

  *int_digit = scaled / div;
  *frac_digits = scaled % div;
}

static int s21_build_e_result(char *result_buf, int precision, int flag_hash,
                              unsigned long long int_digit,
                              unsigned long long frac_digits, int exp_val,
                              int upper) {
  int rlen = 0;

  char int_buf[8];
  int int_len = s21_uint_to_str(int_digit, 10, 0, int_buf);
  rlen += s21_copy_n(result_buf + rlen, int_buf, int_len);

  if (precision > 0 || flag_hash) {
    result_buf[rlen++] = '.';

    if (precision > 0) {
      char frac_buf[64];
      int frac_len = s21_uint_to_str(frac_digits, 10, 0, frac_buf);
      int leading = precision - frac_len;
      rlen += s21_fill_n(result_buf + rlen, '0', leading);
      rlen += s21_copy_n(result_buf + rlen, frac_buf, frac_len);
    }
  }

  rlen += s21_build_exp_str(result_buf + rlen, exp_val, upper);
  return rlen;
}

// ФИКС 1: защита от NaN/Inf в e
int s21_fmt_e(char *out, const FormatSpec *spec, va_list *args) {
  long double value = s21_read_float_arg(spec, args);

  if (isnan(value)) {
    return s21_apply_width(out, "nan", 3, spec, '\0');
  }
  if (isinf(value)) {
    char *inf_str = (value < 0.0L) ? "-inf" : "inf";
    int sign = (value < 0.0L) ? '-' : '\0';
    return s21_apply_width(out, inf_str + (sign == '-'), 3, spec, sign);
  }

  long double abs_value = value;
  char sign = '\0';
  int precision = 0;
  int upper = 0;

  s21_prepare_e_value(spec, args, &abs_value, &sign, &precision, &upper);

  unsigned long long int_digit = 0;
  unsigned long long frac_digits = 0;
  int exp_val = 0;
  s21_calc_e_parts(abs_value, precision, &int_digit, &frac_digits, &exp_val);

  char result_buf[128];
  int rlen = s21_build_e_result(result_buf, precision, spec->flag_hash,
                                int_digit, frac_digits, exp_val, upper);

  FormatSpec width_spec = *spec;
  width_spec.has_precision = 0;

  int written = s21_apply_width_prefix(out, result_buf, rlen, &width_spec, sign,
                                       S21_NULL, 0);
  return written;
}

static long double s21_pow10_ld(int n) {
  long double p = 1.0L;
  for (int i = 0; i < n; i++) p *= 10.0L;
  return p;
}

static unsigned long long s21_pow10_ull(int n) {
  unsigned long long p = 1ULL;
  for (int i = 0; i < n; i++) p *= 10ULL;
  return p;
}

static int s21_trim_fraction_tail(char *buf, int len) {
  while (len > 0 && buf[len - 1] == '0') len--;
  if (len > 0 && buf[len - 1] == '.') len--;
  return len;
}

static int s21_g_adjusted_exp(long double value, int precision) {
  int exp_val = 0;
  s21_decompose_long_double(value, &exp_val);
  int shift = exp_val - precision + 1;
  long double scale = 1.0L;
  if (shift >= 0) {
    scale = s21_pow10_ld(shift);
  } else {
    scale = 1.0L / s21_pow10_ld(-shift);
  }
  unsigned long long sig = (unsigned long long)(value / scale + 0.5L);
  unsigned long long pow10p = s21_pow10_ull(precision);
  if (sig >= pow10p) exp_val++;
  return exp_val;
}

static int s21_g_format_fixed(char *buf, long double value, int precision,
                              int exp_val, int keep_zeros) {
  int frac_prec = precision - 1 - exp_val;
  if (frac_prec < 0) frac_prec = 0;
  // ФИКС 2: ограничение точности
  if (frac_prec > 18) frac_prec = 18;

  long double mult = s21_pow10_ld(frac_prec);
  unsigned long long rounded = (unsigned long long)(value * mult + 0.5L);
  unsigned long long div = (unsigned long long)(mult + 0.5L);
  if (div == 0) div = 1;  // ФИКС 2

  unsigned long long int_part = (frac_prec > 0) ? (rounded / div) : rounded;
  unsigned long long frac_part = (frac_prec > 0) ? (rounded % div) : 0ULL;

  int rlen = 0;
  char int_buf[32];
  int int_len = s21_uint_to_str(int_part, 10, 0, int_buf);
  rlen += s21_copy_n(buf + rlen, int_buf, int_len);

  if (frac_prec > 0 || keep_zeros) {
    buf[rlen++] = '.';
    if (frac_prec > 0) {
      char frac_buf[64];
      int frac_len = s21_uint_to_str(frac_part, 10, 0, frac_buf);
      rlen += s21_fill_n(buf + rlen, '0', frac_prec - frac_len);
      rlen += s21_copy_n(buf + rlen, frac_buf, frac_len);
      if (!keep_zeros) rlen = s21_trim_fraction_tail(buf, rlen);
    }
  }
  return rlen;
}

static int s21_g_format_exp(char *buf, long double value, int precision,
                            int upper, int keep_zeros) {
  int exp_val = 0;
  long double mantissa = s21_decompose_long_double(value, &exp_val);
  int e_prec = precision - 1;
  if (e_prec < 0) e_prec = 0;

  long double mult = s21_pow10_ld(e_prec);
  unsigned long long scaled = (unsigned long long)(mantissa * mult + 0.5L);
  unsigned long long ov = (unsigned long long)(mult * 10.0L + 0.5L);
  if (scaled >= ov) {
    scaled /= 10ULL;
    exp_val++;
  }
  unsigned long long div = (unsigned long long)(mult + 0.5L);
  if (div == 0) div = 1;  // ФИКС 2

  unsigned long long int_digit = (e_prec > 0) ? (scaled / div) : scaled;
  unsigned long long frac_digits = (e_prec > 0) ? (scaled % div) : 0ULL;

  int rlen = 0;
  char int_buf[8];
  int int_len = s21_uint_to_str(int_digit, 10, 0, int_buf);
  rlen += s21_copy_n(buf + rlen, int_buf, int_len);

  if (e_prec > 0 || keep_zeros) {
    buf[rlen++] = '.';
    if (e_prec > 0) {
      char frac_buf[64];
      int frac_len = s21_uint_to_str(frac_digits, 10, 0, frac_buf);
      rlen += s21_fill_n(buf + rlen, '0', e_prec - frac_len);
      rlen += s21_copy_n(buf + rlen, frac_buf, frac_len);
      if (!keep_zeros) rlen = s21_trim_fraction_tail(buf, rlen);
    }
  }
  rlen += s21_build_exp_str(buf + rlen, exp_val, upper);
  return rlen;
}

// ФИКС 1: защита от NaN/Inf в g
int s21_fmt_g(char *out, const FormatSpec *spec, va_list *args) {
  long double value = (spec->length == 'L') ? va_arg(*args, long double)
                                            : va_arg(*args, double);

  if (isnan(value)) {
    return s21_apply_width(out, "nan", 3, spec, '\0');
  }
  if (isinf(value)) {
    char *inf_str = (value < 0.0L) ? "-inf" : "inf";
    int sign = (value < 0.0L) ? '-' : '\0';
    return s21_apply_width(out, inf_str + (sign == '-'), 3, spec, sign);
  }

  int upper = (spec->specifier == 'G');
  int precision = spec->has_precision ? spec->precision : 6;
  if (precision == 0) precision = 1;
  char sign = '\0';
  if (value < 0.0L) {
    sign = '-';
    value = -value;
  } else {
    sign = get_sign(0, spec);
  }
  int exp_val = 0;
  if (value > 0.0L) exp_val = s21_g_adjusted_exp(value, precision);
  char result_buf[256];
  int rlen = 0;
  if (exp_val >= -4 && exp_val < precision) {
    rlen = s21_g_format_fixed(result_buf, value, precision, exp_val,
                              spec->flag_hash);
  } else {
    rlen =
        s21_g_format_exp(result_buf, value, precision, upper, spec->flag_hash);
  }
  return s21_apply_width(out, result_buf, rlen, spec, sign);
}