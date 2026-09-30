#include <limits.h>
#include <math.h>
#include <strings.h>

#include "s21_helpers.h"
#include "s21_string.h"

typedef enum { LEN_NONE, LEN_HH, LEN_H, LEN_L, LEN_LL, LEN_BIG_L } s21_len_t;

typedef struct {
  int suppress;
  int width;
  s21_len_t length;
  char spec;
} s21_scan_spec_t;

static int s21_is_xdigit_local(char c) {
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
         (c >= 'A' && c <= 'F');
}

static int s21_hex_val(char c) {
  int lower = c | 0x20;
  return (c >= '0' && c <= '9')           ? c - '0'
         : (lower >= 'a' && lower <= 'f') ? lower - 'a' + 10
                                          : -1;
}

static int s21_skip_spaces(const char **p) {
  int skipped = 0;
  while (**p != '\0' && s21_is_space(**p)) {
    (*p)++;
    skipped++;
  }
  return skipped;
}

static int s21_parse_scan_spec(const char **fmt, s21_scan_spec_t *sp) {
  const char *f = *fmt;
  int result = 0;
  sp->suppress = 0;
  sp->width = 0;
  sp->length = LEN_NONE;
  sp->spec = '\0';
  if (*f == '*') {
    sp->suppress = 1;
    f++;
  }
  if (s21_is_digit(*f)) {
    int w = 0;
    int n = s21_parse_uint(f, &w);
    sp->width = w;
    f += n;
  }
  if (f[0] == 'h' && f[1] == 'h') {
    sp->length = LEN_HH;
    f += 2;
  } else if (f[0] == 'l' && f[1] == 'l') {
    sp->length = LEN_LL;
    f += 2;
  } else if (*f == 'h') {
    sp->length = LEN_H;
    f++;
  } else if (*f == 'l') {
    sp->length = LEN_L;
    f++;
  } else if (*f == 'L') {
    sp->length = LEN_BIG_L;
    f++;
  }
  sp->spec = *f;
  if (sp->spec != '\0') {
    *fmt = f + 1;
    result = 1;
  }
  return result;
}

#define S21_STORE_BY_LEN(ap, len, value, hh_t, h_t, l_t, ll_t, def_t) \
  do {                                                                \
    if ((len) == LEN_HH) {                                            \
      *va_arg(*(ap), hh_t *) = (hh_t)(value);                         \
    } else if ((len) == LEN_H) {                                      \
      *va_arg(*(ap), h_t *) = (h_t)(value);                           \
    } else if ((len) == LEN_L) {                                      \
      *va_arg(*(ap), l_t *) = (l_t)(value);                           \
    } else if ((len) == LEN_LL) {                                     \
      *va_arg(*(ap), ll_t *) = (ll_t)(value);                         \
    } else {                                                          \
      *va_arg(*(ap), def_t *) = (def_t)(value);                       \
    }                                                                 \
  } while (0)

static void s21_store_signed(va_list *ap, s21_len_t len, long long v) {
  S21_STORE_BY_LEN(ap, len, v, signed char, short, long, long long, int);
}

static void s21_store_unsigned(va_list *ap, s21_len_t len,
                               unsigned long long v) {
  S21_STORE_BY_LEN(ap, len, v, unsigned char, unsigned short, unsigned long,
                   unsigned long long, unsigned int);
}

static void s21_store_n(va_list *ap, s21_len_t len, int n) {
  S21_STORE_BY_LEN(ap, len, n, signed char, short, long, long long, int);
}

typedef struct {
  const char *p;
  const char *start;
  int width;
  int sign;
  int base;
} IntScanState;

static void s21_init_int_state(const s21_scan_spec_t *sp, const char *in,
                               IntScanState *st) {
  st->p = in;
  st->start = in;
  st->width = (sp->width > 0) ? sp->width : INT_MAX;
  st->sign = 1;
  st->base = 10;
}

static void s21_read_optional_sign(IntScanState *st) {
  if (st->width > 0 && (*st->p == '+' || *st->p == '-')) {
    if (*st->p == '-') st->sign = -1;
    st->p++;
    st->width--;
  }
}

static void s21_set_base_by_spec(const s21_scan_spec_t *sp, IntScanState *st) {
  if (sp->spec == 'o') st->base = 8;
  if (sp->spec == 'x' || sp->spec == 'X' || sp->spec == 'p') {
    st->base = 16;
  }
}

static int s21_try_parse_nil_ptr(const s21_scan_spec_t *sp, IntScanState *st,
                                 va_list *ap, int *consumed, int *assigned,
                                 const char **in) {
  int result = 0;
  if (sp->spec == 'p' && st->width >= 5 && st->p[0] == '(' && st->p[1] == 'n' &&
      st->p[2] == 'i' && st->p[3] == 'l' && st->p[4] == ')') {
    if (!sp->suppress) {
      void **dst = va_arg(*ap, void **);
      *dst = NULL;
      (*assigned)++;
    }
    st->p += 5;
    *consumed += (int)(st->p - st->start);
    *in = st->p;
    result = 1;
  }
  return result;
}

static void s21_consume_base_prefix(const s21_scan_spec_t *sp,
                                    IntScanState *st) {
  if (sp->spec == 'i') {
    if (st->width > 0 && *st->p == '0') {
      if (st->width > 1 && (st->p[1] == 'x' || st->p[1] == 'X')) {
        st->base = 16;
        st->p += 2;
        st->width -= 2;
      } else {
        st->base = 8;
      }
    }
    return;
  }
  if ((sp->spec == 'x' || sp->spec == 'X' || sp->spec == 'p') &&
      st->width > 1 && st->p[0] == '0' &&
      (st->p[1] == 'x' || st->p[1] == 'X')) {
    st->p += 2;
    st->width -= 2;
  }
}

static int s21_read_digits(IntScanState *st, unsigned long long *u) {
  int digits = 0;
  *u = 0;
  while (*st->p != '\0' && st->width > 0) {
    int d = -1;
    if (st->base == 10) {
      if (!s21_is_digit(*st->p)) break;
      d = *st->p - '0';
    } else if (st->base == 8) {
      if (*st->p < '0' || *st->p > '7') break;
      d = *st->p - '0';
    } else {
      if (!s21_is_xdigit_local(*st->p)) break;
      d = s21_hex_val(*st->p);
    }
    *u = (*u) * (unsigned)st->base + (unsigned)d;
    st->p++;
    st->width--;
    digits++;
  }
  return digits;
}

static void s21_store_int_result(const s21_scan_spec_t *sp, va_list *ap,
                                 const IntScanState *st, unsigned long long u,
                                 int *assigned) {
  if (sp->suppress) return;
  if (sp->spec == 'u' || sp->spec == 'o' || sp->spec == 'x' ||
      sp->spec == 'X' || sp->spec == 'p') {
    if (sp->spec == 'p') {
      void **dst = va_arg(*ap, void **);
      *dst = (void *)(uintptr_t)u;
    } else {
      s21_store_unsigned(ap, sp->length, u);
    }
  } else {
    long long sv = (st->sign < 0) ? -(long long)u : (long long)u;
    s21_store_signed(ap, sp->length, sv);
  }
  (*assigned)++;
}

static int s21_scan_int_conv(const s21_scan_spec_t *sp, const char **in,
                             va_list *ap, int *consumed, int *assigned) {
  IntScanState st;
  s21_init_int_state(sp, *in, &st);
  s21_read_optional_sign(&st);
  s21_set_base_by_spec(sp, &st);

  int result = 0;

  if (s21_try_parse_nil_ptr(sp, &st, ap, consumed, assigned, in)) {
    result = 1;
  } else {
    s21_consume_base_prefix(sp, &st);
    unsigned long long u = 0;
    int digits = s21_read_digits(&st, &u);

    if (digits > 0) {
      s21_store_int_result(sp, ap, &st, u, assigned);
      *consumed += (int)(st.p - st.start);
      *in = st.p;
      result = 1;
    }
  }

  return result;
}

/* ===================================================================
 * НОВЫЙ РУЧНОЙ ПАРСЕР FLOAT (вместо strtold)
 * =================================================================== */

typedef struct {
  const char *p;
  int rem;
  int bounded;
} s21_float_cur_t;

static int s21_fc_has(const s21_float_cur_t *c) {
  return c->bounded ? (c->rem > 0) : (*c->p != '\0');
}

static char s21_fc_ch(const s21_float_cur_t *c) { return *c->p; }

static void s21_fc_next(s21_float_cur_t *c) {
  c->p++;
  if (c->bounded) c->rem--;
}

static void s21_fc_sign(s21_float_cur_t *c, int *sign) {
  if (s21_fc_has(c) && (s21_fc_ch(c) == '+' || s21_fc_ch(c) == '-')) {
    if (s21_fc_ch(c) == '-') *sign = -1;
    s21_fc_next(c);
  }
}

static int s21_fc_digits(s21_float_cur_t *c, long double *acc) {
  int n = 0;
  while (s21_fc_has(c) && s21_is_digit(s21_fc_ch(c))) {
    *acc = (*acc) * 10.0L + (long double)(s21_fc_ch(c) - '0');
    s21_fc_next(c);
    n++;
  }
  return n;
}

static int s21_fc_frac(s21_float_cur_t *c, long double *frac,
                       long double *div) {
  int n = 0;
  if (s21_fc_has(c) && s21_fc_ch(c) == '.') {
    s21_fc_next(c);
    while (s21_fc_has(c) && s21_is_digit(s21_fc_ch(c))) {
      *frac = (*frac) * 10.0L + (long double)(s21_fc_ch(c) - '0');
      *div *= 10.0L;
      s21_fc_next(c);
      n++;
    }
  }
  return n;
}

static void s21_fc_exp(s21_float_cur_t *c, int *exp10) {
  s21_float_cur_t save = *c;
  int sign = 1;
  int val = 0;
  int digits = 0;
  if (!s21_fc_has(c) || (s21_fc_ch(c) != 'e' && s21_fc_ch(c) != 'E')) return;
  s21_fc_next(c);
  if (s21_fc_has(c) && (s21_fc_ch(c) == '+' || s21_fc_ch(c) == '-')) {
    if (s21_fc_ch(c) == '-') sign = -1;
    s21_fc_next(c);
  }
  while (s21_fc_has(c) && s21_is_digit(s21_fc_ch(c))) {
    val = val * 10 + (s21_fc_ch(c) - '0');
    s21_fc_next(c);
    digits++;
  }
  if (digits == 0) {
    *c = save;
  } else {
    *exp10 = sign * val;
  }
}

static int s21_pow10_int10(long double *v, int exp10) {
  int ok = 1;
  long double base = 10.0L;
  long double mul = 1.0L;
  int e = (exp10 < 0) ? -exp10 : exp10;
  while (e > 0) {
    if (e & 1) mul *= base;
    base *= base;
    e >>= 1;
  }
  if (exp10 < 0) {
    *v /= mul;
  } else {
    *v *= mul;
  }
  return ok;
}

static int s21_parse_float_core(const char *in, int width, int bounded,
                                long double *out, int *used) {
  s21_float_cur_t c = {in, width, bounded};
  long double ip = 0.0L;
  long double fp = 0.0L;
  long double fd = 1.0L;
  int sign = 1;
  int exp10 = 0;
  int digits = 0;
  int result = 0;

  // обработка nan/inf
  if (s21_fc_has(&c)) {
    char buf[4] = {0};
    int i = 0;
    while (s21_fc_has(&c) && i < 3) {
      buf[i++] = s21_fc_ch(&c);
      s21_fc_next(&c);
    }
    if (strcasecmp(buf, "nan") == 0) {
      *out = NAN;
      *used = i;
      return 1;
    }
    if (strcasecmp(buf, "inf") == 0) {
      *out = INFINITY * sign;
      *used = i;
      return 1;
    }
    // откатываем, если не nan/inf
    c = (s21_float_cur_t){in, width, bounded};
  }

  s21_fc_sign(&c, &sign);
  digits += s21_fc_digits(&c, &ip);
  digits += s21_fc_frac(&c, &fp, &fd);

  if (digits > 0) {
    long double v = ip + (fp / fd);
    s21_fc_exp(&c, &exp10);
    s21_pow10_int10(&v, exp10);
    v *= (long double)sign;
    *out = v;
    *used = (int)(c.p - in);
    result = 1;
  }

  return result;
}

static int s21_parse_float_unbounded(const char **in, long double *out,
                                     int *used) {
  int result = s21_parse_float_core(*in, 0, 0, out, used);
  if (result) *in += *used;
  return result;
}

static int s21_parse_float_bounded(const char *in, int width, long double *out,
                                   int *used) {
  return s21_parse_float_core(in, width, 1, out, used);
}

static void s21_store_float_value(const s21_scan_spec_t *sp, va_list *ap,
                                  long double v, int *assigned) {
  if (sp->suppress) return;
  if (sp->length == LEN_BIG_L) {
    long double *dst = va_arg(*ap, long double *);
    *dst = v;
  } else if (sp->length == LEN_L) {
    double *dst = va_arg(*ap, double *);
    *dst = (double)v;
  } else {
    float *dst = va_arg(*ap, float *);
    *dst = (float)v;
  }
  (*assigned)++;
}

static int s21_scan_float_conv(const s21_scan_spec_t *sp, const char **in,
                               va_list *ap, int *consumed, int *assigned) {
  long double v = 0.0L;
  int used = 0;
  int ok = 0;
  if (sp->width <= 0) {
    ok = s21_parse_float_unbounded(in, &v, &used);
  } else {
    ok = s21_parse_float_bounded(*in, sp->width, &v, &used);
    if (ok) *in += used;
  }
  if (ok) {
    s21_store_float_value(sp, ap, v, assigned);
    *consumed += used;
  }
  return ok;
}

static int s21_scan_string_conv(const s21_scan_spec_t *sp, const char **in,
                                va_list *ap, int *consumed, int *assigned) {
  const char *p = *in;
  int width = (sp->width > 0) ? sp->width : INT_MAX;
  int n = 0;
  int result;
  char *dst = S21_NULL;
  if (!sp->suppress) dst = va_arg(*ap, char *);
  while (*p != '\0' && !s21_is_space(*p) && n < width) {
    if (!sp->suppress) dst[n] = *p;
    p++;
    n++;
  }
  result = (n > 0);
  if (result) {
    if (!sp->suppress) {
      dst[n] = '\0';
      (*assigned)++;
    }
    *consumed += n;
    *in = p;
  }
  return result;
}

static int s21_scan_char_conv(const s21_scan_spec_t *sp, const char **in,
                              va_list *ap, int *consumed, int *assigned) {
  const char *p = *in;
  int width = (sp->width > 0) ? sp->width : 1;
  int result = 0;
  if (*p != '\0') {
    char *dst = S21_NULL;
    int i = 0;
    if (!sp->suppress) dst = va_arg(*ap, char *);
    while (i < width && p[i] != '\0') {
      if (!sp->suppress) dst[i] = p[i];
      i++;
    }
    result = (i == width);
    if (result) {
      if (!sp->suppress) (*assigned)++;
      *consumed += i;
      *in = p + i;
    }
  }
  return result;
}

static int s21_is_int_spec(char c) {
  return c == 'd' || c == 'i' || c == 'u' || c == 'o' || c == 'x' || c == 'X' ||
         c == 'p';
}

static int s21_is_float_spec(char c) {
  return c == 'f' || c == 'e' || c == 'E' || c == 'g' || c == 'G';
}

static void s21_skip_fmt_spaces(const char **fmt, const char **in,
                                int *consumed) {
  while (s21_is_space(**fmt)) (*fmt)++;
  *consumed += s21_skip_spaces(in);
}

static int s21_match_literal(const char **fmt, const char **in, int *consumed) {
  int matched = (**in == **fmt);
  if (matched) {
    (*in)++;
    (*fmt)++;
    (*consumed)++;
  }
  return matched;
}

static void s21_skip_input_before_conv(const s21_scan_spec_t *sp,
                                       const char **in, int *consumed) {
  if (sp->spec != 'c' && sp->spec != 'n' && sp->spec != '[') {
    *consumed += s21_skip_spaces(in);
  }
}

static int s21_dispatch_conv(const s21_scan_spec_t *sp, const char **in,
                             va_list *ap, int *consumed, int *assigned) {
  int result = 0;

  if (s21_is_int_spec(sp->spec)) {
    result = s21_scan_int_conv(sp, in, ap, consumed, assigned);
  } else if (s21_is_float_spec(sp->spec)) {
    result = s21_scan_float_conv(sp, in, ap, consumed, assigned);
  } else if (sp->spec == 's') {
    result = s21_scan_string_conv(sp, in, ap, consumed, assigned);
  } else if (sp->spec == 'c') {
    result = s21_scan_char_conv(sp, in, ap, consumed, assigned);
  } else if (sp->spec == 'n') {
    if (!sp->suppress) {
      s21_store_n(ap, sp->length, *consumed);
    }
    result = 1;
  }

  return result;
}

static int s21_fail_result(int assigned, const char *in) {
  int result = assigned;
  if (assigned == 0 && *in == '\0') result = -1;
  return result;
}

static int s21_process_one_token(const char **fmt, const char **in, va_list *ap,
                                 int *consumed, int *assigned, int *result) {
  int ret = 0;

  if (s21_is_space(**fmt)) {
    s21_skip_fmt_spaces(fmt, in, consumed);
    ret = 1;
  } else if (**fmt != '%') {
    ret = s21_match_literal(fmt, in, consumed);
  } else {
    (*fmt)++;
    if (**fmt == '\0') {
      ret = 0;
    } else if (**fmt == '%') {
      ret = s21_match_literal(fmt, in, consumed);
    } else {
      // ФИКС: инициализация структуры
      s21_scan_spec_t sp = {0};
      if (s21_parse_scan_spec(fmt, &sp)) {
        s21_skip_input_before_conv(&sp, in, consumed);
        if (s21_dispatch_conv(&sp, in, ap, consumed, assigned)) {
          ret = 1;
        } else {
          *result = s21_fail_result(*assigned, *in);
          ret = 0;
        }
      } else {
        ret = 0;
      }
    }
  }

  return ret;
}

int s21_sscanf(const char *str, const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  const char *in = str;
  const char *fmt = format;
  int assigned = 0;
  int consumed = 0;
  int result = -1;
  int ok = 1;
  while (*fmt != '\0' && ok) {
    ok = s21_process_one_token(&fmt, &in, &ap, &consumed, &assigned, &result);
  }
  if (ok) {
    result = assigned;
  }
  va_end(ap);
  return result;
}