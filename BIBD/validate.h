#include <stdbool.h>
#include "matrix.h"

// Validate a BIBD by definition
bool validate1(const struct Matrix *matrix);

// Validate a BIBD by theorem 1.13
bool validate2(const struct Matrix *matrix);
