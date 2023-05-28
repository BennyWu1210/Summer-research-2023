// The purpose for this program is to validate if a given matrix
// is a valid BIBD (using 2 unique approaches - by definition & by
// a specific theorem (1.13))

#include <matrix.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Check by definiton
bool validate1(const struct Matrix *matrix)
{

    int v = get_rows(matrix);        // number of points
    int b = get_cols(matrix);        // number of blocks
    int k = -1, lambda = -1, r = -1; // the other 3 parameters

    // iterate through each row to get 'r'
    for (int i = 0; i < v; i++)
    {
        int count = 0;
        for (int j = 0; j < b; j++)
        {
            count += get_value(matrix, i, j);
        }

        // if 'r' is not consistent across all points
        if (r != -1 && r != count)
        {
            return false;
        }

        r = count;
    }

    // iterate through each column to get 'k'
    for (int i = 0; i < b; i++)
    {
        int count = 0;
        for (int j = 0; j < v; j++)
        {
            count += get_value(matrix, j, i);
        }

        // if 'k' is not consistent across all blocks
        if (k != -1 && k != count)
        {
            return false;
        }

        k = count;
    }

    // by definition
    if (!(v > k && k >= 2))
    {
        return false;
    }

    // check if 'lambda' is consistent
    for (int i = 0; i < v; i++)
    {
        for (int j = i + 1; j < v; j++)
        {
            // printf("pair: %d, %d\n", i, j);
            int count = 0;
            for (int block = 0; block < b; block++)
            {
                // both have to be '1' to be added
                count += get_value(matrix, i, block) & get_value(matrix, j, block);
            }

            // printf("count: %d\n", count);

            if (lambda != -1 && lambda != count)
            {
                return false;
            }
        }
    }

    return true;
}

// check by Theorem 1.13
bool validate2(const struct Matrix *m)
{
    const struct Matrix *transpose = matrix_transpose(m);
    const struct Matrix *multiplied = matrix_multiply(m, transpose);

    printf("resulting matrix:\n");
    matrix_print(multiplied);

    int len = get_rows(m);
    int r = -1;
    int lambda = -1;

    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len; j++)
        {
            if (i == j)
            {
                if (r != -1 && get_value(multiplied, i, j) != r)
                    return false;
                r = get_value(multiplied, i, j);
            }
            else
            {
                if (lambda != -1 && get_value(multiplied, i, j) != lambda)
                    return false;
                lambda = get_value(multiplied, i, j);
            }
        }
    }

    // Second part for Theorem 1.13

    int k = 0;
    for (int i = 0; i < get_rows(m); i++)
    {
        k += get_value(m, 0, i);
    }

    struct Matrix *u_v = matrix_init(1, get_rows(m));
    struct Matrix *u_b = matrix_init(1, get_cols(m));

    for (int i = 0; i < get_rows(m); i++)
        matrix_insert(u_v, 0, i, 1);
    for (int i = 0; i < get_cols(m); i++)
        matrix_insert(u_b, 0, i, 1);

    struct Matrix *left_side = matrix_multiply(u_v, m);
    struct Matrix *right_side = matrix_times_constant(u_b, k);

    return matrix_equal(left_side, right_side);
}

int main()
{
    // the dimension of the matrix
    int rows, cols;
    scanf("%d", &rows);
    scanf("%d", &cols);

    struct Matrix *matrix = matrix_init(rows, cols);

    // fill the matrix with input values
    int num;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            scanf("%d", &num);
            matrix_insert(matrix, i, j, num);
        }
    }

    // for testing purposes
    printf("Initial incidence matrix:\n");
    matrix_print(matrix);

    // first method
    printf("====================\n");

    printf("method 1 (by definition):\n");
    if (validate1(matrix))
    {
        printf("IS VALID\n");
    }
    else
    {
        printf("NOT VALID\n");
    }
    printf("====================\n");

    // second method
    printf("====================\n");

    printf("method 2 (by Theorem 1.13):\n");
    if (validate2(matrix))
    {
        printf("IS VALID\n");
    }
    else
    {
        printf("NOT VALID\n");
    }
    printf("====================\n");

    free(matrix);
    matrix = NULL;
}
