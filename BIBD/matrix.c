#include <matrix.h>
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
    m->data[i][j] += value;
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

// get_basis(m) finds the basis for the column space of m
struct Matrix *get_basis(const struct Matrix *m)
{
    struct Matrix *new_m = matrix_dup(m);
    // basically finding the RREF here (almost)

    int pivots = 0;
    for (int i = 0; i < new_m->rows && i < new_m->cols; i++)
    {
        int pivot_row = -1;

        for (int j = i; j < new_m->rows; j++)
        {
            if (get_value(new_m, j, i) != 0)
            {
                pivot_row = j;
                pivots++;
                break;
            }
        }

        if (pivot_row == -1)
            continue;

        // swap the rows if necessary
        if (pivot_row != i)
        {
            for (int j = 0; j < new_m->cols; j++)
            {
                int temp = get_value(new_m, i, j);
                matrix_insert(new_m, i, j, get_value(new_m, pivot_row, j));
                matrix_insert(new_m, pivot_row, j, temp);
            }
        }

        // row reduce the current row
        for (int j = 0; j < new_m->rows; j++)
        {
            if (i == j)
                continue;

            if (get_value(new_m, j, i) != 0)
            {
                // this works for the general case (despite only working in mod 2)
                int multiple = lcm(get_value(new_m, j, i), get_value(new_m, i, i));
                int factor1 = multiple / get_value(new_m, i, i);
                int factor2 = multiple / get_value(new_m, j, i);
                for (int k = i; k < new_m->cols; k++)
                {
                    matrix_insert(new_m, j, k, get_value(new_m, j, k) * factor2 - get_value(new_m, i, k) * factor1);
                }
            }
        }
    }

    struct Matrix *result = matrix_init(m->rows, pivots);
    int count = 0;
    for (int row = 0; row < m->rows; row++)
    {
        for (int col = 0; col < m->rows; col++)
        {
            if (get_value(new_m, row, col) != 0)
            {
                for (int i = 0; i < m->rows; i++)
                {
                    matrix_insert(result, i, count, get_value(m, i, col));
                }
                count++;
                break;
            }
        }
    }

    free(new_m);

    return result;
}