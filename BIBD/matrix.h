#include <stdbool.h>

struct Matrix;

struct Matrix *matrix_init(int rows, int cols);

void matrix_print(const struct Matrix *m);

void matrix_insert(const struct Matrix *m, int i, int j, int value);

void matrix_add(const struct Matrix *m, int i, int j, int value);

int get_rows(const struct Matrix *m);

int get_cols(const struct Matrix *m);

int get_value(const struct Matrix *m, int i, int j);

struct Matrix *matrix_transpose(const struct Matrix *m);

struct Matrix *matrix_multiply(const struct Matrix *m1, const struct Matrix *m2);

struct Matrix *matrix_times_constant(const struct Matrix *m, int k);

struct Matrix *get_row_basis(const struct Matrix *m);

bool matrix_equal(const struct Matrix *m1, const struct Matrix *m2);
