#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int err = OK;

  result->rows = rows;
  result->columns = columns;

  if (rows <= 0 || columns <= 0) {
    err = WRONGMAT;
    return err;
  }

  result->matrix =
      (double **)malloc(result->rows * sizeof(double *) +
                        result->rows * result->columns * sizeof(double));

  if (result->matrix == NULL) {
    err = WRONGMAT;
    free(result->matrix);
  } else {
    result->matrix[0] = (double *)(result->matrix + result->rows);
    for (int i = 1; i < result->rows; i++) {
      result->matrix[i] = result->matrix[0] + i * result->columns;
    }
  }

  return err;
}

void s21_remove_matrix(matrix_t *A) {
  if (A->rows > 0 && A->columns > 0) {
    if (A->matrix != NULL) {
      free(A->matrix);
    }
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int err = SUCCESS;
  double epsilon = 0.0000001, rounded_a, rounded_b;

  if (A->matrix == NULL || B->matrix == NULL) {
    err = WRONGMAT;
    return err;
  }

  if (A->rows <= 0 || A->columns <= 0 || B->rows <= 0 || B->columns <= 0) {
    err = WRONGMAT;
    return err;
  }

  if (A->rows == B->rows && A->columns == B->columns) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        rounded_a = round(A->matrix[i][j] * 10000000) / 10000000;
        rounded_b = round(B->matrix[i][j] * 10000000) / 10000000;

        if (fabs(rounded_a - rounded_b) < epsilon) {
          continue;
        } else {
          err = FAILURE;
        }
      }
    }
  } else {
    err = FAILURE;
  }

  return err;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int err;

  if (A->matrix == NULL || B->matrix == NULL) {
    result->rows = 0;
    result->columns = 0;
    err = WRONGMAT;
    return err;
  }

  if (A->rows <= 0 || A->columns <= 0 || B->rows <= 0 || B->columns <= 0) {
    result->rows = 0;
    result->columns = 0;
    err = WRONGMAT;
    return err;
  }

  if (A->rows != B->rows || A->columns != B->columns) {
    result->rows = 0;
    result->columns = 0;
    err = CALCERR;
    return err;
  }

  err = s21_create_matrix(A->rows, A->columns, result);

  if (err == OK) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      }
    }
  }

  return err;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int err;

  if (A->matrix == NULL || B->matrix == NULL) {
    err = WRONGMAT;
    return err;
  }

  if (A->rows <= 0 || A->columns <= 0 || B->rows <= 0 || B->columns <= 0) {
    err = WRONGMAT;
    return err;
  }

  if (A->rows != B->rows || A->columns != B->columns) {
    err = CALCERR;
    return err;
  }

  err = s21_create_matrix(A->rows, A->columns, result);

  if (err == OK) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      }
    }
  }

  return err;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int err;

  if (A->matrix == NULL) {
    err = WRONGMAT;
    return err;
  }

  if (A->rows <= 0 || A->columns <= 0) {
    err = WRONGMAT;
    return err;
  }

  err = s21_create_matrix(A->rows, A->columns, result);

  if (err == OK) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] * number;
      }
    }
  }

  return err;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int err;
  double sum;

  if (A->matrix == NULL || B->matrix == NULL) {
    err = WRONGMAT;
    return err;
  }

  if (A->rows <= 0 || A->columns <= 0 || B->rows <= 0 || B->columns <= 0) {
    err = WRONGMAT;
    return err;
  }

  if (A->columns != B->rows) {
    err = CALCERR;
    return err;
  }

  err = s21_create_matrix(A->rows, B->columns, result);

  if (err == OK) {
    for (int i = 0; i < result->rows; i++) {
      for (int j = 0; j < result->columns; j++) {
        sum = 0;
        for (int k = 0; k < A->columns; k++) {
          sum += A->matrix[i][k] * B->matrix[k][j];
        }
        result->matrix[i][j] = sum;
      }
    }
  }

  return err;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int err;

  if (A->matrix == NULL) {
    err = WRONGMAT;
    return err;
  }

  if (A->rows <= 0 || A->columns <= 0) {
    err = WRONGMAT;
    return err;
  }

  err = s21_create_matrix(A->columns, A->rows, result);

  if (err == OK) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[j][i] = A->matrix[i][j];
      }
    }
  }

  return err;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int err, sub_j, sub_i;
  double det, complement;

  matrix_t submatrix;

  err = s21_create_matrix(A->rows, A->columns, result);

  if (err != OK) {
    return err;
  }

  if (A->matrix == NULL) {
    err = WRONGMAT;
    return err;
  }

  if (A->rows <= 0 || A->columns <= 0) {
    err = WRONGMAT;
    return err;
  }

  if (A->rows != A->columns) {
    err = CALCERR;
    return err;
  }

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      s21_create_matrix(A->rows - 1, A->columns - 1, &submatrix);

      sub_i = 0;
      for (int k = 0; k < A->rows; k++) {
        if (k == i) {
          continue;
        }
        sub_j = 0;
        for (int l = 0; l < A->columns; l++) {
          if (l == j) {
            continue;
          }
          submatrix.matrix[sub_i][sub_j] = A->matrix[k][l];
          sub_j++;
        }
        sub_i++;
      }

      s21_determinant(&submatrix, &det);

      complement = det * pow(-1, i + j + 2);

      result->matrix[i][j] = complement;

      s21_remove_matrix(&submatrix);
    }
  }

  return err;
}

int s21_determinant(matrix_t *A, double *result) {
  int err = OK;
  int i, j, k, sign;
  double det = 0.0;

  matrix_t submatrix;

  if (A->matrix == NULL) {
    err = WRONGMAT;
    return err;
  }

  if (A->rows <= 0 || A->columns <= 0) {
    err = WRONGMAT;
    return err;
  }

  if (A->rows != A->columns) {
    err = CALCERR;
    return err;
  }

  if (A->rows == 1) {
    *result = A->matrix[0][0];
  } else if (A->rows == 2) {
    *result =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
  } else {
    err = s21_create_matrix(A->rows - 1, A->columns - 1, &submatrix);

    for (k = 0; k < A->columns; k++) {
      for (i = 1; i < A->rows; i++) {
        for (j = 0; j < A->columns; j++) {
          if (j != k) {
            submatrix.matrix[i - 1][j < k ? j : j - 1] = A->matrix[i][j];
          }
        }
      }

      if (k % 2 == 0) {
        sign = 1;
      } else {
        sign = -1;
      }

      err = s21_determinant(&submatrix, result);

      if (err != OK) {
        s21_remove_matrix(&submatrix);
        break;
      } else {
        det += sign * A->matrix[0][k] * (*result);
      }
    }

    if (err == OK) {
      *result = det;
      s21_remove_matrix(&submatrix);
    }
  }

  return err;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int err = OK;
  double det = 0.0;

  if (A->rows <= 0 || A->columns <= 0) {
    err = WRONGMAT;
    return err;
  }

  if (A->rows != A->columns) {
    err = CALCERR;
    return err;
  }

  s21_determinant(A, &det);

  if (det == 0) {
    err = CALCERR;
  } else {
    matrix_t subres;

    s21_create_matrix(A->rows, A->columns, &subres);
    s21_calc_complements(A, &subres);
    s21_transpose(&subres, result);

    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] /= det;
      }
    }

    s21_remove_matrix(&subres);
  }

  return err;
}