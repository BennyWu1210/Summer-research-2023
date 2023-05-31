// The purpose of this program (as of yet) is to generate all possible codewords
// from an incidence matrix of a BIBD.

#include <validate.h>
#include <matrix.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

int main()
{
    // the number of points, size of each block, and occurrences of every pair respectively
    int v, k, lambda;

    scanf("%d", &v);
    scanf("%d", &k);
    scanf("%d", &lambda);

    // more information about the BIBD (number of occurrences of every point + number of blocks)
    int r = lambda * (v - 1) / (k - 1);
    int b = v * r / k;

    struct Matrix *original_matrix = matrix_init(v, b);

    for (int i = 0; i < v; i++)
    {
        for (int j = 0; j < b; j++)
        {
            int value;
            scanf("%d", &value);
            matrix_insert(original_matrix, i, j, value);
        }
    }

    // Ensure that the input matrix is a valid BIBD
    // TODO: Clarify - if I were to augment the matrix at the beginning, would I still need to check for this? If so, how?
    matrix_print(original_matrix);
    assert(validate1(original_matrix) == validate2(original_matrix));

    if (!validate1(original_matrix))
    {
        printf("Not a valid BIBD\n");
        return 0;
    }

    // augment the matrix when r is odd
    bool augmented = r % 2;
    b += augmented;
    struct Matrix *matrix = matrix_init(v, b);

    for (int i = 0; i < v; i++)
    {
        for (int j = 0; j < b; j++)
        {
            if (augmented && j == b - 1)
            {
                matrix_insert(matrix, i, j, 1);
            }
            else
            {
                matrix_insert(matrix, i, j, get_value(original_matrix, i, j));
            }
        }
    }

    // get the basis of the matrix
    struct Matrix *basis = get_row_basis(matrix);

    matrix_print(basis);
    // find all possible codewords by taking its linear combinations
    int basis_rows = get_rows(basis);
    int basis_cols = get_cols(basis);
    int mask_max = (1 << basis_rows);

    for (int mask = 0; mask < mask_max; mask++)
    {
        struct Matrix *codeword = matrix_init(1, b);
        for (int i = 0; i < basis_rows; i++)
        {
            if (mask & (1 << i))
            {

                for (int j = 0; j < basis_cols; j++)
                {
                    matrix_add(codeword, 0, j, get_value(basis, i, j));
                }
            }
        }

        free(codeword);
    }

    // release memory
    free(original_matrix);
    free(matrix);
    free(basis);
}