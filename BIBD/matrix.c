#include "matrix.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

struct Matrix
{
    int rows;
    int cols;
    bool **data; // Note that the matrix works in mod 2
};

int get_rows(const struct Matrix *m)
{
    return m->rows;
}

int get_cols(const struct Matrix *m)
{
    return m->cols;
}

int get_value(const struct Matrix *m, int i, int j)
{
    assert(i < m->rows);
    assert(j < m->cols);
    return m->data[i][j];
}

struct Matrix *matrix_init(int rows, int cols)
{
    struct Matrix *new_matrix = malloc(sizeof(struct Matrix));
    new_matrix->data = malloc(rows * sizeof(int *));
    new_matrix->rows = rows;
    new_matrix->cols = cols;

    for (int i = 0; i < rows; i++)
    {
        new_matrix->data[i] = malloc(cols * sizeof(int));
    }

    return new_matrix;
}

struct Matrix *matrix_dup(const struct Matrix *m)
{
    struct Matrix *new_m = matrix_init(m->rows, m->cols);
    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->cols; j++)
        {
            matrix_insert(new_m, i, j, get_value(m, i, j));
        }
    }

    return new_m;
}

void matrix_print(const struct Matrix *m)
{
    printf("rows: %d\n", m->rows);
    printf("columns: %d\n", m->cols);

    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->cols; j++)
        {
            printf("%d ", m->data[i][j]);
        }
        printf("\n");
    }
}

void matrix_insert(const struct Matrix *m, int i, int j, int value)
{
    m->data[i][j] = value;
}

void matrix_add(const struct Matrix *m, int i, int j, int value)
{
    m->data[i][j] ^= value;
}

struct Matrix *matrix_transpose(const struct Matrix *m)
{
    int rows = m->cols;
    int cols = m->rows;

    struct Matrix *result = matrix_init(rows, cols);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            matrix_insert(result, i, j, get_value(m, j, i));
        }
    }

    return result;
}

struct Matrix *matrix_multiply(const struct Matrix *m1, const struct Matrix *m2)
{
    assert(m1->cols == m2->rows);
    int rows = m1->rows;
    int cols = m2->cols;

    struct Matrix *result = matrix_init(rows, cols);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            matrix_insert(result, i, j, 0);
            for (int k = 0; k < m1->cols; k++)
            {
                matrix_add(result, i, j, get_value(m1, i, k) * get_value(m2, k, j));
            }
        }
    }
    return result;
}

struct Matrix *matrix_times_constant(const struct Matrix *m, int k)
{
    int rows = get_rows(m);
    int cols = get_cols(m);
    struct Matrix *result = matrix_init(rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            matrix_insert(result, i, j, get_value(m, i, j) * k);
        }
    }

    return result;
}

bool matrix_equal(const struct Matrix *m1, const struct Matrix *m2)
{
    if (!(m1->rows == m2->rows && m1->cols == m2->cols))
    {
        return false;
    }

    for (int i = 0; i < m1->rows; i++)
    {
        for (int j = 0; j < m1->cols; j++)
        {
            if (get_value(m1, i, j) != get_value(m2, i, j))
                return false;
        }
    }

    return true;
}

int gcd(int a, int b)
{
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

int lcm(int a, int b)
{
    return a * b / abs(gcd(a, b));
}

// get_row_basis(m) finds the basis for the column space of m
struct Matrix *get_row_basis(const struct Matrix *m)
{
    struct Matrix *new_m = matrix_dup(m);
    int basis_count = 0;
    int rows = m->rows, cols = m->cols;

    for (int i = 0; i < cols; i++)
    {
        // See if a pivot element is found in this row
        int pivot_pos = -1;
        for (int j = 0; j < rows; j++)
        {
            if (get_value(new_m, j, i) != 0)
            {
                pivot_pos = j;
                break;
            }
        }

        // If a pivot column is found, elimnate other entries in that column
        if (pivot_pos != -1)
        {
            basis_count++;
            for (int k = 0; k < cols; k++)
            {
                if (k != i && get_value(new_m, pivot_pos, k) != 0)
                {
                    // reduce this current column (k)
                    for (int j = 0; j < rows; j++)
                    {
                        matrix_add(new_m, j, k, get_value(new_m, j, i));
                    }
                }
            }
        }
    }

    // extract the basis
    struct Matrix *basis = matrix_init(basis_count, cols);
    int index = 0;
    for (int i = 0; i < cols; i++)
    {
        int row_index = -1;

        for (int j = 0; j < rows; j++)
        {
            if (get_value(new_m, j, i) != 0)
            {
                row_index = j;
                break;
            }
        }

        if (row_index != -1)
        {
            for (int j = 0; j < cols; j++)
            {
                matrix_insert(basis, index, j, get_value(m, row_index, j));
            }
            index++;
        }
    }

    free(new_m);
    return basis;
}