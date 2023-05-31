#include <matrix.h>
#include <stdio.h>
#include <stdlib.h>

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

    struct Matrix *basis = get_row_basis(matrix);
    printf("The basis is: \n");
    matrix_print(basis);

    free(matrix);
    free(basis);
}