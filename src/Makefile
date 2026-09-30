CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c11 -I.
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

CORE_SRC = $(wildcard core/*.c)
ALL_OBJ = $(notdir $(CORE_SRC:.c=.o))

TEST_SRC = $(wildcard tests_suite/*.c) $(wildcard tests_suite/*/*.c)
TEST_BIN = s21_test_string
LIB = s21_string.a
LIBS = -lcheck -lsubunit -lm -pthread -lrt

.PHONY: all clean test gcov_report style leaks

all: $(LIB)

$(LIB): $(ALL_OBJ)
	ar rc $@ $^
	ranlib $@

%.o: core/%.c s21_string.h
	$(CC) $(CFLAGS) -c $< -o $@

test: $(LIB)
	$(CC) $(CFLAGS) $(TEST_SRC) -L. -l:$(LIB) -o $(TEST_BIN) $(LIBS)
	./$(TEST_BIN)

gcov_report: $(LIB)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(TEST_SRC) -L. -l:$(LIB) -o $(TEST_BIN) $(LIBS) -lgcov
	./$(TEST_BIN)
	lcov -t "$(TEST_BIN)" -o report.info -c -d . --no-external
	genhtml -o report report.info
	xdg-open report/index.html
	rm -rf *.gcda *.gcno

clean:
	rm -rf $(ALL_OBJ) $(LIB) $(TEST_BIN) *.gcda *.gcno *.info report
	rm -rf $(TEST_BIN).dSYM

style:
	clang-format -n -style=google s21_string.h core/*.c tests_suite/*.c tests_suite/*/*.c

leaks: test
	valgrind --leak-check=full --track-origins=yes ./$(TEST_BIN)