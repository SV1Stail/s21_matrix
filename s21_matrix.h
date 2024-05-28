#define S21_MATRIX_H
#ifdef S21_MATRIX_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 0
#define MATRIX_ERR 1
#define CALC_ERR 2

#define SUCCESS 1
#define FAILURE 0

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

// void print_matrix(matrix_t matrix);
int good_matrix(matrix_t A);
int eq_size(matrix_t *A, matrix_t *B);
double mult(matrix_t *A, matrix_t *B, int a_stroka, int b_stolb);
int check_mat(matrix_t *result);
double recurse(matrix_t *A);
void min_or(matrix_t *A, int rows, int columns, matrix_t *result);
void s21_fill_matrix(matrix_t *val, double step);

int s21_create_matrix(int rows, int columns, matrix_t *result);

void s21_remove_matrix(matrix_t *A);

int s21_eq_matrix(matrix_t *A, matrix_t *B);

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

int s21_mult_number(matrix_t *A, double number, matrix_t *result);
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

int s21_transpose(matrix_t *A, matrix_t *result);

int s21_calc_complements(matrix_t *A, matrix_t *result);

int s21_determinant(matrix_t *A, double *result);

int s21_inverse_matrix(matrix_t *A, matrix_t *result);

#endif
