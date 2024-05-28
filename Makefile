CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 

LIBS = -lcheck
OS = $(shell uname)
GCOV=-coverage

ifeq ($(OS), Darwin)
	LIBS = -lcheck
else
	LIBS = -lcheck -lsubunit -pthread -lrt -lm
endif

all: clean s21_matrix.a 

s21_matrix.a: 
	$(CC) $(CFLAGS) -c s21_matrix.c
	ar rcs s21_matrix.a s21_*.o
	rm -rf ./*.o

test: s21_matrix.a
	$(CC) $(CFLAGS) -c check.c -o check.o
	$(CC) check.o s21_matrix.a $(LIBS) -o test
	./test

valgrind: 
	make clean
	make test
	valgrind --tool=memcheck --leak-check=full -s --keep-debuginfo=yes --sigill-diagnostics=yes --undef-value-errors=yes --track-origins=yes ./test


format:
	clang-format -n *.c *.h -style=Google

gcov_report: s21_matrix.a
	$(CC) $(GCOV) -c s21_*.c 
	$(CC) -c check.c $(LIBS)
	$(CC) $(GCOV) -o matrix_test check.o s21_*.o $(LIBS)
	./matrix_test

	lcov -t "matrix_tests" -o matrix_test.info -c -d .
	genhtml -o report/ matrix_test.info
	open ./report/index.html

clean:
	rm -rf ./*.o ./*.a test ./report ./*.gcno ./*.gcda ./*.info matrix_test
