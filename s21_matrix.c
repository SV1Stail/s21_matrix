
#include "s21_matrix.h"

// 0 - 0
// 1 - Ошибка, некорректная матрица
// 2 - Ошибка вычисления (несовпадающие размеры матриц; матрица, для которой
// нельзя провести вычисления и т.д.)
void s21_fill_matrix(matrix_t *val, double step) {
  double n = 1.0;
  for (int i = 0; i < val->rows; ++i) {
    for (int k = 0; k < val->columns; ++k) {
      val->matrix[i][k] = n;
      n += step;
    }
  }
}

int check_mat(matrix_t *result) {
  int status = 0;
  if (result == NULL || result->columns < 1 || result->rows < 1 ||
      result->matrix == NULL)
    status = 1;
  return status;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int status = 1;
  if (check_mat(A) == 0) {
    status = 0;
    double det = 0;
    s21_determinant(A, &det);
    if (A->rows != A->columns || det == 0.0) {
      status = 2;
    } else {
      matrix_t comp;
      matrix_t transp;
      int x1 = s21_calc_complements(A, &comp);
      int x2 = s21_transpose(&comp, &transp);
      if (x2 == 0 && x1 == 0) {
        status = s21_mult_number(&transp, 1 / det, result);
        s21_remove_matrix(&comp);
        s21_remove_matrix(&transp);
      }
    }
  }
  return status;
}

int s21_determinant(matrix_t *A, double *result) {
  *result = 0;
  int status = 0;
  if (check_mat(A) == 1)
    status = 1;
  else if (A->rows != A->columns)
    status = 2;
  else
    *result = recurse(A);
  return status;
}

double recurse(matrix_t *A) {
  double det = 0;
  matrix_t B = {0};
  if (A->columns == 1) {
    det = A->matrix[0][0];
  } else if (A->columns == 2) {
    det = A->matrix[0][0] * A->matrix[1][1] - A->matrix[1][0] * A->matrix[0][1];
  } else if (A->columns > 2) {
    for (int j = 0; j < A->columns; j++) {
      min_or(A, 0, j, &B);
      double res = recurse(&B);
      det += A->matrix[0][j] * pow(-1, 2 + j) * res;
      s21_remove_matrix(&B);
    }
  }
  return det;
}
void min_or(matrix_t *A, int rows, int columns, matrix_t *result) {
  s21_create_matrix(A->rows - 1, A->columns - 1, result);
  int minor_i = 0;
  for (int i = 0; i < A->rows; i++) {
    int minor_j = 0;
    if (i == rows) {
      continue;
    }
    for (int j = 0; j < A->columns; j++) {
      if (j != columns) {
        result->matrix[minor_i][minor_j] = A->matrix[i][j];
        minor_j++;
      }
    }
    minor_i++;
  }
}
int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int status = 0;
  if (check_mat(A) == 1 || (A->rows == 1 && A->columns == 1))
    status = 1;
  else if (A->rows != A->columns)
    status = 2;
  else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < result->rows; i++) {
      for (int j = 0; j < result->columns; j++) {
        matrix_t minor;
        double a;
        min_or(A, i, j, &minor);
        s21_determinant(&minor, &a);
        result->matrix[i][j] = pow((-1), i + j) * a;
        s21_remove_matrix(&minor);
      }
    }
  }
  return status;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  if (good_matrix(*A) == 0) return 1;

  int flag = s21_create_matrix(A->columns, A->rows, result);
  if (flag == 0) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[j][i] = A->matrix[i][j];
      }
    }
  }
  return flag;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  //  из А беру колво строк
  // ИЗ B беру колво столбцов

  if (good_matrix(*A) == 0 || good_matrix(*B) == 0) return 1;
  // у В должно совпадать колво строк с колвом столбцов А
  else if (A->columns != B->rows)
    return 2;

  int flag = s21_create_matrix(A->rows, B->columns, result);

  if (flag == 0) {
    for (int sr0a = 0; sr0a < A->rows; sr0a++) {
      for (int stolb = 0; stolb < B->columns; stolb++) {
        result->matrix[sr0a][stolb] = mult(A, B, sr0a, stolb);
      }
    }
  }

  return flag;
}

// передаю ячейку матрицы результата
double mult(matrix_t *A, matrix_t *B, int a_str0a, int b_stolb) {
  double res = 0;
  for (int i = 0; i < A->columns; i++) {
    res += A->matrix[a_str0a][i] * B->matrix[i][b_stolb];
  }
  return res;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  if (good_matrix(*A) == 0) {
    return 1;
  }
  int flag = 0;
  flag = s21_create_matrix(A->rows, A->columns, result);
  if (flag == 0) {  // 0 ? work : not work
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++)
        result->matrix[i][j] = A->matrix[i][j] * number;
    }
  } else {
    s21_remove_matrix(result);
  }

  return flag;
}
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (good_matrix(*A) == 0 || good_matrix(*B) == 0) {
    return 1;
  }  // right matrices ?
  else if (eq_size(A, B) == 0) {
    return 2;
  }  // equil ?

  int flag = 0;
  s21_create_matrix(A->rows, A->columns, result);

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
    }
  }
  return flag;
}
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (good_matrix(*A) == 0 || good_matrix(*B) == 0) {
    return 1;
  }  // right matrices ?
  else if (eq_size(A, B) == 0) {
    return 2;
  }  // equil ?

  int flag = 0;
  s21_create_matrix(A->rows, A->columns, result);

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
    }
  }
  return flag;
}

// сравнивает размеры матриц
// ret 1 = good
// ret 0 = BAD
int eq_size(matrix_t *A, matrix_t *B) {
  int flag = 0;

  if ((A->columns == B->columns) && (A->rows == B->rows)) {
    flag = 1;
  }
  return flag;
}

// проверка матрицы на заполненость
// ret 1 = good
// ret 0 = BAD
int good_matrix(matrix_t A) {
  int flag = 0;

  if (A.matrix != NULL && A.columns > 0 && A.rows > 0) {
    flag = 1;
  }
  return flag;
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int flag = SUCCESS;
  if ((A->columns == B->columns) && (A->rows == B->rows) && A->matrix &&
      B->matrix && A->columns > 0 && B->columns > 0 && A->rows > 0 &&
      B->rows > 0) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-7) {
          flag = FAILURE;
          break;
        }
      }
      if (flag == FAILURE) break;
    }
  } else {
    flag = FAILURE;
  }
  return flag;
}
void s21_remove_matrix(matrix_t *A) {
  if (A->matrix != NULL && A != NULL) {
    for (int i = 0; i < A->rows; i++) {
      free(A->matrix[i]);
    }
    free(A->matrix);
    A->matrix = NULL;
    A->columns = 0;
    A->rows = 0;
  }
}

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  if (rows < 1 || columns < 1) {
    return 1;
  }
  int flag = 0;
  result->rows = rows;
  result->columns = columns;
  result->matrix = calloc(rows, sizeof(double *));
  if (result->matrix == NULL) {
    flag = 2;
  } else {
    for (int i = 0; i < rows; i++) {
      result->matrix[i] = calloc(columns, sizeof(double));
      if (result->matrix[i] == NULL) {
        flag = 2;
        for (int j = 0; j < i; j++) free(result->matrix[j]);
        break;
      }
    }
  }
  return flag;
}
/*
void print_matrix(matrix_t matrix) {
  if (matrix.matrix != NULL) {
    for (int i = 0; i < matrix.rows; i++) {
      for (int j = 0; j < matrix.columns; j++) {
        printf("%lf\n", matrix.matrix[i][j]);
      }
    }
  }
  printf("%d\n", matrix.rows);
  printf("%d\n", matrix.columns);
  printf("----------\n");
}*/
