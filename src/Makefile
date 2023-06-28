.PHONY: s21_matrix.a

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror
TESTFLAGS = -lcheck -coverage -lpthread -pthread -L.

C_FILES = s21_*.c
O_FILES = s21_*.o

all:
	$(MAKE) s21_matrix.a

s21_matrix.a:
	$(MAKE) clean
	$(CC) $(CFLAGS) -c $(C_FILES)
	ar rcs s21_matrix.a $(O_FILES)

test:
	$(CC) $(CFLAGS) $(TESTFLAGS) run_tests.c test_matrix.c s21_matrix.a -o test_s21_matrix
	./test_s21_matrix

gcov_report:
	$(CC) $(CFLAGS) $(TESTFLAGS) $(C_FILES) run_tests.c test_matrix.c -o gcov_report_s21_matrix
	./gcov_report_s21_matrix
	lcov -t "gcovreport" -o gcovreport.info -c -d .
	genhtml -o report gcovreport.info

clean:
	rm -f *.a
	rm -f *.o
	rm -f *.gcno
	rm -f *.gcda
	rm -f *.gcov
	rm -f *.info
	rm -f test_s21_decimal
	rm -f gcov_report
	rm -rf report

style:
	cppcheck *.c *.h
	clang-format -style=Google -n *.c *.h

leaks:
	leaks --atExit -- ./test_s21_matrix

valgrind:
	valgrind --leak-check=full ./test_s21_matrix